#pragma once

#include "BoardClass.h"
#include "IBattleshipGameAlgo.h"
#include "Utils.h"
#include <set>
using namespace std;

class HeatMap
{
public:
    HeatMap() : HeatMap(Board::DEFAULT_BOARD_WIDTH, Board::DEFAULT_BOARD_WIDTH) {}//empty constructor
    HeatMap(int rows, int columns); //non-default
    HeatMap(const Board&); //non-default 2
    HeatMap(const HeatMap& map); //copy ctor

    HeatMap& operator=(const HeatMap&);
    int operator()(int, int) const;
    int operator()(pair<int, int> crd) const;
    void setCoord(int row, int col, int newVal);
    void setCoord(pair<int, int> crd, int newVal);
    void coordPlusX(int row, int col, int X);
    void coordPlusX(pair<int, int> crd, int X);

    //friend std::ostream& operator<<(std::ostream &strm, const HeatMap &brd);
    ~HeatMap();
    pair<int,int> getMaxHeatCoord() const;
    pair<int,int> getMaxHeatCoord_destroy() const;

private:
    int rows_;
    int columns_;
    int *data_;
};



/*This is a smart battleship player which makes choices considering maximal apostriori probability */
class Battleship_MAP_player : public IBattleshipGameAlgo
{
public:
    static const int PLAYER_A = 0;
    static const int PLAYER_B = 1;

    Battleship_MAP_player();

    ~Battleship_MAP_player() override;

    /****************
     *      API     *
     ****************/

    bool init(const std::string& path) override; 
    void setBoard(int player, const char** board, int numRows, int numCols) override;
    std::pair<int, int> attack() override;
    void notifyOnAttackResult(int player, int row, int col, AttackResult result) override; // notify on last move result

private:

    /*******************
    *    Queries       *
    *******************/

    bool myShipAt(int row, int col) const;


    /*******************
    *    Functions     *
    *******************/
    void notifyAttackMiss(int row, int col);
    void notifyAttackHit(int row, int col);

    bool initDestroyModeRightAfterSink_innerLoop_isChanged();
    void initDestroyModeRightAfterSink();

    void notifyAttackSink(int row, int col);
    void notifyOpponentAttackHit(int row, int col);
    void notifyOpponentAttackSink(int row, int col);

    pair<int, int> detectNotifiedSunkShip_innerLoop(bool* is_inner_loop_changed, vector<pair<int,int>>& detected);
    vector<pair<int,int>> detectNotifiedSunkShip(int row, int col);

    void initObstacles();
    void seekHeat_addHeat_tryShipSizes(HeatMap& seekHeatMap, int i, int j, int orientation) const;
    HeatMap computeSeekHeat() const;
    HeatMap computeDestroyHeat() const;

    int player_;
    Board brd;
    vector<Ship> ships;

    HeatMap obstacles; //in this heatmap 1 means obstacle and 0 means possible
    bool isSeek; // true == SeekMode, false == DestroyMode
    //HeatMap heatmap;
    vector<pair<int, int>> destroySession;
    set<pair<int, int>> uncompleteHitSpots;
};


