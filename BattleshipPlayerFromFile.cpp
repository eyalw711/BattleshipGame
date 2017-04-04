#include "stdafx.h"
#include "BattleshipPlayerFromFile.h"
#include <string>
#include <iostream>
#include <fstream>


/*Doesn't do anything*/
BattleshipPlayerFromFile::BattleshipPlayerFromFile() : pBrd(nullptr), attacksQueue(nullptr)
{
}

/* initializes the attacksQueue */
BattleshipPlayerFromFile::BattleshipPlayerFromFile(string pathToAttackFile) : pBrd(nullptr), attacksQueue(nullptr)
{
	//TODO: unimplemented
	//open path
	string line;
	ifstream attacksFile(pathToAttackFile);
	if (attacksFile.is_open())
	{
		while (getline(attacksFile, line))
		{
			cout << line << '\n';
		}
		attacksFile.close();
	}

	else cout << "Unable to open file" << endl;
	//read attacks

	//queue attacks into attacksQueue
}


BattleshipPlayerFromFile::~BattleshipPlayerFromFile()
{
	delete pBrd;
	delete attacksQueue;

}

std::pair<int, int> BattleshipPlayerFromFile::attack()
{
	//TODO
	return std::pair<int, int>();
}

void BattleshipPlayerFromFile::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
	//TODO
}

/* This function is my API to get my board.
 * Here I dont read files, only 
 */
void BattleshipPlayerFromFile::setBoard(const char ** board, int numRows, int numCols)
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


