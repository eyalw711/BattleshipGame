#include "stdafx.h"
#include "Battleship_MAP_player.h"
#include <set>
#include <exception>
#include <ctime>
#include <iso646.h>


/** TODO List:
 *  1. fix ctor
 *  2. finish implementation of all methods
 */

/*************************************
*  Battleship_MAP_player Functions   *
**************************************/

Battleship_MAP_player::Battleship_MAP_player() :
    brd(Board()),
    ships(vector<Ship>())
{
    srand(static_cast<int>(time(nullptr)));
}

Battleship_MAP_player::~Battleship_MAP_player()
{
}

bool Battleship_MAP_player::init(const std::string & path)
{
    return true;
}

void Battleship_MAP_player::setBoard(int player, const char ** board, int numRows, int numCols)
{
    player_ = player;
    brd = Board(board, numRows, numCols);
    this->brd.findShips(PLAYER_A, ships);
    if (ships.empty())
        this->brd.findShips(PLAYER_B, ships);

    initObstacles();
}

std::pair<int, int> Battleship_MAP_player::attack()
{
    HeatMap heatmap;
    pair<int, int> attackCrd;
    if (isSeek)
    {
        heatmap = computeSeekHeat();
        attackCrd = heatmap.getMaxHeatCoord();
    }
    else
    {
        heatmap = computeDestroyHeat();
        attackCrd = heatmap.getMaxHeatCoord_destroy();
    }
    return attackCrd;
}

bool Battleship_MAP_player::myShipAt(int row, int col) const
{
    return brd(row, col) != Board::SEA;
}

void Battleship_MAP_player::notifyAttackMiss(int row, int col)
{
    obstacles.setCoord(row, col, 1);
}

/**
 * \brief: This is called to notify me only 
 * when ***I*** hit the enemy.
 * (Assuming I will never shoot myself - 
 * which I don't intend to do)
 */
void Battleship_MAP_player::notifyAttackHit(int row, int col)
{
    isSeek = false; //going to "Destroy" Mode (or already in this mode)

    // If not first hit then the hit must be adjacent to 
    // a previous hit - meaning same ship.
    auto hitCrd = make_pair(row, col);

    destroySession.push_back(hitCrd);

    // maybe this hit connected another incomplete hit to the destroy session?
    for (auto it_crd = uncompleteHitSpots.begin(); it_crd != uncompleteHitSpots.end(); )
    {
        if (Ship::isAdjacentCoordinates(*it_crd, hitCrd))
        {
            destroySession.push_back(*it_crd);
            it_crd = uncompleteHitSpots.erase(it_crd);
        }
        else {
            ++it_crd;
        }
    }
}

bool Battleship_MAP_player::initDestroyModeRightAfterSink_innerLoop_isChanged()
{
    for (auto const& crd : destroySession)
    {
        for (auto it_possibly_adjacent_crd = uncompleteHitSpots.begin(); it_possibly_adjacent_crd != uncompleteHitSpots.end(); )
        {
            if (Ship::isAdjacentCoordinates(*it_possibly_adjacent_crd, crd))
            {
                destroySession.push_back(*it_possibly_adjacent_crd);
                it_possibly_adjacent_crd = uncompleteHitSpots.erase(it_possibly_adjacent_crd);
                return true;
            }
            ++it_possibly_adjacent_crd;
        }
    }
    return false;
}

void Battleship_MAP_player::initDestroyModeRightAfterSink()
{
    auto new_destroy_core = *(uncompleteHitSpots.begin());
    destroySession.push_back(new_destroy_core);
    uncompleteHitSpots.erase(new_destroy_core);
    while (initDestroyModeRightAfterSink_innerLoop_isChanged()) 
    {   //this loop takes all adjacents to new_destroy_core from
        //uncompleteHitSpots to destroySession
    }
}

/**
 * \brief: This is called to notify me only
 * when ***I*** sunk an enemy ship.
 * (Assuming I will never shoot myself - 
 * which I don't intend to do)
 */
void Battleship_MAP_player::notifyAttackSink(int row, int col)
{
    destroySession.push_back(make_pair(row, col));
    for (auto const& crd : destroySession)
        obstacles.setCoord(crd, 1);
    destroySession.clear();

    // if uncompleteHitSpots is empty then just sink
    // else - I know where to find enemy ships!
    isSeek = uncompleteHitSpots.empty();
    if (!isSeek)
        initDestroyModeRightAfterSink();
}

void Battleship_MAP_player::notifyOpponentAttackHit(int row, int col)
{
    if (!myShipAt(row, col)) // nice! the opponent hit itself and I discovered its location!
    {
        if (isSeek) 
        { //thank you, I was looking all over the place for you
            isSeek = false;
            destroySession.push_back(make_pair(row, col));
        }
        else 
        {//I'm already in the middle of destroying a ship
            bool helps_me_in_destroy_mode = false;
            for (auto const& crd : destroySession)
            {
                if (Ship::isAdjacentCoordinates(make_pair(row, col), crd))
                {
                    //helps me in my destroy mode:
                    helps_me_in_destroy_mode = true;
                    break;
                }
            }
            if (!helps_me_in_destroy_mode)
                uncompleteHitSpots.insert(make_pair(row, col));
            else
                destroySession.push_back(make_pair(row, col));
        }
    }
}

void Battleship_MAP_player::notifyOpponentAttackSink(int row, int col)
{
    auto sinkCrd = make_pair(row, col);
    if (!myShipAt(row, col)) // nice! the opponent sink itself!
    {
        bool helps_me_in_destroy_mode = false;
        for (auto const& crd : destroySession)
        {
            if (Ship::isAdjacentCoordinates(sinkCrd, crd))
            {
                //helps me in my destroy mode:
                helps_me_in_destroy_mode = true;
                break;
            }
        }
        if (helps_me_in_destroy_mode)
        { //sets all destroy session coords to obstacles, turns on seek mode (or destroy),
          //and clears destroySession.
            destroySession.push_back(sinkCrd);
            notifyAttackSink(row, col);
        }
        else
        { // either I can find adjacents in uncompleteHitSpots or it was a 1 coord ship.
            auto shipCrds = detectNotifiedSunkShip(row, col); //removes from uncompleteHitSpots too
            /*if (shipCrds.size() != 1) // sunk something from uncompleteHitSpots
            {
                 for (auto const& crd : shipCrds)
                     uncompleteHitSpots.erase(crd);
            }*/
            for (auto const& crd : shipCrds)
                obstacles.setCoord(crd, 1);
        }
    }
}

pair<int, int> Battleship_MAP_player::detectNotifiedSunkShip_innerLoop(
    bool* is_inner_loop_changed,
    vector<pair<int, int>>& detected
)
{
    for (auto const& crd : detected)
    {
        for (auto const& possibly_adjacent_crd : uncompleteHitSpots)
        {
            if (Ship::isAdjacentCoordinates(crd, possibly_adjacent_crd))
            {
                uncompleteHitSpots.erase(possibly_adjacent_crd);
                *is_inner_loop_changed = true;
                return possibly_adjacent_crd;
            }
        }
    }
    *is_inner_loop_changed = false;
    return make_pair(-1, -1);
}

/**
 * \brief: this method should be called
 * after the enemy sunk its own ship.
 * the function discovers the coords of the ship
 * in uncompleteHitSpots and returns them.
 */
vector<pair<int, int>> Battleship_MAP_player::detectNotifiedSunkShip(int row, int col)
{
    bool isInnerLoopChanged = false;
    auto detectedCrds = vector<pair<int, int>>();
    detectedCrds.push_back(make_pair(row, col));
    do
    {
        pair<int, int> newCrd = detectNotifiedSunkShip_innerLoop(&isInnerLoopChanged, detectedCrds);
        if (newCrd.first != -1 and newCrd.second != -1)
            detectedCrds.push_back(newCrd);
    } while (isInnerLoopChanged);
    return detectedCrds;
}

void Battleship_MAP_player::notifyOnAttackResult(
    int player,
    int row,
    int col,
    AttackResult result
)
{
    /* Whoever made the shot, if missed, I'm interested.*/
    if (result == AttackResult::Miss)
    {
        notifyAttackMiss(row, col);
        return;
    }
    if (player == player_)
    {
        // tell me what happened with my attack:
        if (result == AttackResult::Hit)
            notifyAttackHit(row, col);
        else
            notifyAttackSink(row, col);
    }
    else
    {
        // tell me what happened with opponent's attack:
        if (result == AttackResult::Hit)
            notifyOpponentAttackHit(row, col);
        else
            notifyOpponentAttackSink(row, col);
    }
}

/**
 * \when: Only after board exists, and all ships are init'd
 * \brief: This method sets all of the player's ship 
 * coordinates and their adjacents as obstacles.
 */
void Battleship_MAP_player::initObstacles()
{
    // create heatmap the size of board, init'd to zero (meaning not obstacle)
    obstacles = HeatMap(brd);

    for (auto const& ship : ships) 
    {
        for (auto const& crdHitPair : ship.getCoordinates())
        {
            auto crd = crdHitPair.first;
            obstacles.setCoord(crd, 1);
        }
        for (auto const& crd : ship.getAdjacentCoordinates(brd))
        {
            obstacles.setCoord(crd, 1);
        }
    }
}

void Battleship_MAP_player::seekHeat_addHeat_tryShipSizes(HeatMap& seekHeatMap, int i, int j, int orientation) const
{
    for (int shipSize = 1; shipSize <= Ship::MAXIMUM_SHIP_SIZE; shipSize++)
    {
        int adv_i = orientation;
        int adv_j = 1 - orientation;
        auto enemyCrds = vector<pair<int, int>>(shipSize);
        // finally check all coords are not in obstacles:

        for (int crdIdx = 0; crdIdx < shipSize; crdIdx++)
        {
            auto enemy_crd = make_pair(i + crdIdx * adv_i, j + crdIdx * adv_j);
            enemyCrds.push_back(enemy_crd);
        }

        for (const auto& crd : enemyCrds)
        {
            if (obstacles(crd) == 1)
                // if reached obstacle, we'll reach obstacle in all following shipSizes.
                return;
        }

        // if here means no obstacles:
        for (const auto& crd : enemyCrds)
            seekHeatMap.coordPlusX(crd, 1);
    }
}

/**
 * \brief: This function goes over all of the coords of the board,
 * and if a coord could possible be a top/left edge of an enemy
 * ship, then it increases the heat of all the coords of that ship
 * by 1.
 */
HeatMap Battleship_MAP_player::computeSeekHeat() const
{
    auto seekHeatMap = HeatMap(brd);
    auto& seekHeatMapRef = seekHeatMap;

    for (int i = 1; i <= brd.getNumOfRows(); i++)
    {
        for (int j = 1; j <= brd.getNumOfCols(); j++)
        {
            //i,j is the top/left edge of an enemy ship
            if (obstacles(i,j) == 1)
                continue;

            for (int orientation = 0; orientation < 2; orientation++)
            {
                seekHeat_addHeat_tryShipSizes(seekHeatMapRef, i, j, orientation);
            }
        }
    }
    return seekHeatMap;
}


HeatMap Battleship_MAP_player::computeDestroyHeat() const
{
    //TODO: write this implementation
    /*
     * this method calculates the heat of the destroySession
     * looking for the hottest coord to hit
     * 
     * keep in mind you have the uncompleteHitSpots to think about
     * maybe discover that an uncomplete hit helps?
     */
    
    throw new exception("Unimplemented");
    return HeatMap(brd);
}

/************************
 *  HeatMap Functions   *
 ************************/

//non-default ctor
HeatMap::HeatMap(int rows, int columns) : 
    rows_(rows),
    columns_(columns),
    data_(new int[rows * columns])
{
    DEBUG("non-default HeatMap ctor activated - HeatMap with only zeros created");
    memset(data_, 0, rows*columns * sizeof(int));
}

HeatMap::HeatMap(const Board &brd) :
    HeatMap(brd.getNumOfRows(), brd.getNumOfCols())
{
}

HeatMap::HeatMap(const HeatMap &map) : 
    rows_(map.rows_),
    columns_(map.columns_),
    data_(new int[map.rows_ * map.columns_])
{
    memcpy(data_, map.data_, rows_ * columns_ * sizeof(int));
}

HeatMap& HeatMap::operator=(const HeatMap &other)
{
    DEBUG("operator= of HeatMap was called");
    if (this != &other)
    {
        if (columns_*rows_ != other.columns_*other.rows_) //bummer
        {
            delete[] data_;
            data_ = new int[other.columns_*other.rows_];
        }
        columns_ = other.columns_;
        rows_ = other.rows_;
        memcpy(data_, other.data_, rows_*columns_*sizeof(int));
    }
    //self assignment
    return *this;
}

/*In the game, indexing starts at 1*/
int HeatMap::operator()(int row, int column) const
{
    row -= 1;
    column -= 1;
    if (-1 < row && row < rows_ && -1 < column && column < columns_)
    {
        return data_[columns_*row + column];
    }
    throw std::out_of_range("Trying to access invalid index in the HeatMap.");
}

/*In the game, indexing starts at 1*/
int HeatMap::operator()(pair<int, int> crd) const
{

    int row = crd.first - 1;
    int column = crd.second - 1;
    if (-1 < row && row < rows_ && -1 < column && column < columns_)
    {
        return data_[columns_*row + column];
    }
    throw std::out_of_range("Trying to access invalid index in the HeatMap.");
}

void HeatMap::setCoord(int row, int col, int newVal)
{
    row -= 1;
    col -= 1;
    if (-1 < row && row < rows_ && -1 < col && col < columns_)
    {
        data_[columns_*row + col] = newVal;
        return;
    }
    throw std::out_of_range("Trying to access invalid index in the HeatMap.");
}

void HeatMap::setCoord(const pair<int, int> crd, int newVal)
{
    setCoord(crd.first, crd.second, newVal);
}

void HeatMap::coordPlusX(int row, int col, int X)
{
    row -= 1;
    col -= 1;
    if (-1 < row && row < rows_ && -1 < col && col < columns_)
    {
        data_[columns_*row + col] += X;
        return;
    }
    throw std::out_of_range("Trying to access invalid index in the HeatMap.");
}

void HeatMap::coordPlusX(pair<int, int> crd, int X)
{
    coordPlusX(crd.first, crd.second, X);
}

//dtor
HeatMap::~HeatMap()
{
    DEBUG("HeatMap dtor activated");
    delete[] data_;
}

pair<int, int> HeatMap::getMaxHeatCoord() const
{
    int maxHeat = -1;
    auto maxHeatCrd = vector<pair<int, int>>();
    for (int i = 1; i <= rows_; i++)
    {
        for (int j = 1; j <= columns_; j++)
        {
            if ((*this)(i,j) == maxHeat)
            {
                maxHeatCrd.push_back(make_pair(i, j));
            }
            else if ((*this)(i,j) > maxHeat)
            {
                maxHeatCrd.clear();
                maxHeatCrd.push_back(make_pair(i, j));
                maxHeat = (*this)(i, j);
            }
        }
    }
    if (maxHeatCrd.empty())
    {
        throw new exception("Failed to find max heat coordinate!");
    }
    int randomIndex = rand() % maxHeatCrd.size();
    return maxHeatCrd[randomIndex];
}

pair<int, int> HeatMap::getMaxHeatCoord_destroy() const
{
    throw new exception("Unimplemented");
    return pair<int, int>();
}

