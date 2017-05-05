#include "stdafx.h"
#include "BattleshipNaivePlayer.h"

BattleshipNaivePlayer::BattleshipNaivePlayer() : brd(Board()),ships(vector<Ship>()),current_coord(pair<int,int>(1,1))
{
}

BattleshipNaivePlayer::~BattleshipNaivePlayer()
{
}

bool BattleshipNaivePlayer::init(const std::string & path)
{
	return true;
}

void BattleshipNaivePlayer::setBoard(int player, const char ** board, int numRows, int numCols)
{
	brd = Board(board, numRows, numCols);
	this->brd.findShips(PLAYER_A, ships);
	if (ships.empty())
		this->brd.findShips(PLAYER_B, ships);
}


bool BattleshipNaivePlayer::should_attack_coord(int row, int col) const
{
	/* check that the given coord does
	 * not equal a ship of mine or a coordinate adjecent to my ship  */
	for (vector<Ship>::const_iterator it = ships.begin(); it != ships.end(); ++it)
	{
		for (vector<pair<pair<int, int>, bool>>::const_iterator it2 = it->getCoordinates().begin(); it2 != it->getCoordinates().end(); ++it2)
		{
			if (make_pair(row, col) == it2->first || Ship::isAdjacentCoordinates(make_pair(row, col), it2->first))
				return false;
		}
	}
	/* check that have not attacked the given coord already */
	if (brd(row, col) == Utils::HIT_SIGN)
		return false;
	return true;
}


std::pair<int, int> BattleshipNaivePlayer::attack()
{
	bool should_attack = false;
	int row, col;
	do
	{
		should_attack = should_attack_coord(current_coord.first, current_coord.second);
		row = current_coord.first;
		col = current_coord.second;
		current_coord = brd.getNextCoord(current_coord.first, current_coord.second);
	} while (!should_attack);
	
	return make_pair(row, col);

}

void BattleshipNaivePlayer::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
	//thank you but I am stupid and don't really care about the results of the attack :)
	return;
}
