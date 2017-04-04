#include "stdafx.h"
#include "BattleshipGameFromFileClass.h"
#include <string>

/*Doesn't do anything*/
BattleshipPlayerAlgoFromFile::BattleshipPlayerAlgoFromFile() : pBrd(nullptr), attacks(nullptr)
{
}

/* initializes the attacksQueue */
BattleshipPlayerAlgoFromFile::BattleshipPlayerAlgoFromFile(string pathToAttackFile) : pBrd(nullptr), attacks(nullptr)
{
	//TODO: unimplemented
}


/* This function is my API to get my board.
 * Here I dont read files, only 
 */
void BattleshipPlayerAlgoFromFile::setBoard(const char ** board, int numRows, int numCols)
{
	//-- Read the board content:

	char *buff = new char(numRows * numCols);
	for (auto i=0; i < numRows; i++) //TODO: is board a pointer to an array of rows? array of columns?
	{
		memcpy(buff + numCols * i, board[i], numCols); //copies one row into a long buffer
	}

	std::string content(reinterpret_cast<char*>(buff), numRows * numCols);
	pBrd = new Board(content, numRows, numCols);
}
