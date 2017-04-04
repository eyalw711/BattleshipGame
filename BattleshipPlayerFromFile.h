#pragma once

#include "BoardClass.h"
#include "IBattleshipGameAlgo.h"
#include <queue>
using namespace std;

/*This is a player*/
class BattleshipPlayerFromFile : IBattleshipGameAlgo
{
public:
	BattleshipPlayerFromFile();

	/*This ctor gets a path to attack file*/
	BattleshipPlayerFromFile(string);

	/* This function is my API to get my board */
	void setBoard(const char** board, int numRows, int numCols) override;

	~BattleshipPlayerFromFile() override;
	
	std::pair<int, int> attack() override; // ask player for his move
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override; // notify on last move result
private:
	Board *pBrd;
	std::queue<std::pair<int,int>> *attacksQueue;
};