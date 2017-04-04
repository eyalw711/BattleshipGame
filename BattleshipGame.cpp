// BattleshipGame.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "BoardClass.h"
#include "IBattleshipGameAlgo.h"
#include "BattleshipPlayerFromFile.h"
#include <iostream>




/*Main is for running games etc...*/
int main()
{
	const char **segmentationFault = nullptr; //TODO: need to make a board. this one is not good obviously.

	BattleshipPlayerFromFile playerA = BattleshipPlayerFromFile(0, string("TestFilesV3\\moves\\dirty_ilegal_movesA.attack-a")); //clean_movesA.attack-a"));
	playerA.setBoard(segmentationFault, 10, 10);
	return 0;
}

