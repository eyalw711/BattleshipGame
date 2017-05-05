// BattleshipGame.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "BoardClass.h"
#include "IBattleshipGameAlgo.h"
#include "BattleshipPlayerFromFile.h"

#include "GameFromFileManager.h"


/*Main is for running games etc...*/
int main(int argc, char *argv[])
{

	GameFromFileManager manager = GameFromFileManager();

	/* The following line does:
	 *  - finds files
	 *  - checks board validity
	 *  - manager finds the players' ships
	 *  - manager initializes players
	 */
	bool success = manager.initialize_file_and_naive(argc, argv);
	if (!success)
		return -1;

	manager.setPlayersBoards();

	manager.mainLoop();

	return 0;
}

