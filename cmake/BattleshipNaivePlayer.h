#pragma once

#include "BoardClass.h"
#include "IBattleshipGameAlgo.h"
#include "Utils.h"
using namespace std;


/*This is a player*/
class BattleshipNaivePlayer : public IBattleshipGameAlgo
{
public:
	static const int PLAYER_A = 0;
	static const int PLAYER_B = 1;

	/*This ctor gets a path to attack file*/
	BattleshipNaivePlayer();

	~BattleshipNaivePlayer() override;

	bool init(const std::string& path) override; // returns whether the init succeeded or failed

	/* This function is my API to get my board */
	void setBoard(int player, const char** board, int numRows, int numCols) override;

	/* My API to ask my attack */
	std::pair<int, int> attack() override;

	/* My API to receive attack results */
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override; // notify on last move result

	bool BattleshipNaivePlayer::should_attack_coord(int row, int col) const;

private:
	Board brd;
	vector<Ship> ships;
	pair<int, int> current_coord;
};