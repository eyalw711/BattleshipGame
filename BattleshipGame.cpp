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
	bool success = manager.initialize(argc, argv);
	if (!success)
		return -1;

	manager.setPlayersBoards();
	if (manager.askPlayersToLoadAttacks() == false)
		return -1;

	manager.mainLoop();

	return 0;
}

