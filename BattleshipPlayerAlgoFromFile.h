#pragma once

#include "BoardClass.h"
#include "IBattleshipGameAlgo.h"
#include <queue>
using namespace std;

/*This is a player*/
class BattleshipPlayerAlgoFromFile : IBattleshipGameAlgo
{
public:
	BattleshipPlayerAlgoFromFile();

	/*This ctor gets a path to attack file*/
	BattleshipPlayerAlgoFromFile(string);

	/* This function is my API to get my board */
	void setBoard(const char** board, int numRows, int numCols) override;


private:
	Board *pBrd;
	std::queue<std::pair<int,int>> *attacks;
};