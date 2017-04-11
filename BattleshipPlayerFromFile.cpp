#include "stdafx.h"
#include "BattleshipPlayerFromFile.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace  std;

/* initializes the attacksQueue */
BattleshipPlayerFromFile::BattleshipPlayerFromFile(char Id, string pathToAttackFile) : 
	pBrd(nullptr), attacksQueue(new std::queue<std::pair<int, int>>), Id(Id), pathToAttacksFile(pathToAttackFile)
{
}

/* returns true for success and false for error */
bool BattleshipPlayerFromFile::loadAttacksFromFile()
{
	bool retVal = true;
	if (pBrd == nullptr)
		return false; //can't do this if I don't have numRows, numCols

	//-- open path
	string line;
	ifstream attacksFile(pathToAttacksFile);
	if (attacksFile.is_open())
	{
		//read attacks
		while (getline(attacksFile, line))
		{
			int row, col;
			char c;
			std::stringstream ss(line);
			//--stage0: ignore empty lines
			if (line.empty())
			{
				cout << "Invalid empty row!" << endl;
				continue;
			}

			//--stage1: ignore starter spaces
			while (isspace(ss.peek()))
				ss.ignore();

			//--stage2: parse int or get out
			if (isdigit(ss.peek()))
				ss >> row;
			else
			{
				cout << "Expected a number as row!" << endl;
				continue;
			}

			//--stage3: ignore spaces after row
			while (isspace(ss.peek()))
				ss.ignore();

			//--stage4: parse comma or get out
			if (ss.peek() == ',')
			{
				try
				{
					ss.ignore();
				}
				catch (exception e)
				{
					cout << e.what() << endl;
				}
			}
			else
			{
				cout << "Expected a comma after row!" << endl;
				continue;
			}

			//--stage5: ignore spaces after comma
			while (isspace(ss.peek()))
			{
				try
				{
					ss.ignore();
				}
				catch (exception e)
				{
					cout << e.what() << endl;
				}
			}

			//--stage6: parse int or get out
			if (isdigit(ss.peek()))
				ss >> col;
			else
			{
				cout << "Expected a number as column!" << endl;
				continue;
			}

			//--stage7: ignore spaces until end or get out
			try
			{
				while (ss >> c)
				{
					if (!isspace(c))
						throw logic_error("Invalid Attack String");
				}
			}
			catch (logic_error le)
			{
				cout << "Expected nothing after column!" << endl;
				continue; //why continue???
			}
			//--stage8: check coords validity
			if (row < 1 || row > pBrd->getNumOfRows() || col < 1 ||  col > pBrd->getNumOfCols())
			{
				cout << "Coordinates are invalid to board!" << endl;
				continue;
			}
			//-- success
			//queue attacks into attacksQueue
			attacksQueue->push(std::pair<int, int>(row, col));
			cout << "Entered attack " << row << "," << col << endl;
		}
		attacksFile.close();
	}
	else
	{
		cout << "Unable to open file" << endl;
		retVal = false;
	}
	return retVal;
}

BattleshipPlayerFromFile::~BattleshipPlayerFromFile()
{
	delete pBrd;
	delete attacksQueue;
}

/* returns next attack from file if there is any, else an invalid attack */
std::pair<int, int> BattleshipPlayerFromFile::attack()
{
	if (attacksQueue->empty())
	{
		return std::pair<int, int>(-1, -1);
	}
	else
	{
		auto retVal = attacksQueue->front();
		attacksQueue->pop();
		return retVal;
	}
}

void BattleshipPlayerFromFile::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
	//thank you but I am stupid and don't really care about the results of the attack :)
	return;
}

char BattleshipPlayerFromFile::getId() const
{
	return Id;
}

/* This function is my API to acquire my board. */
void BattleshipPlayerFromFile::setBoard(const char ** board, int numRows, int numCols)
{
	/*as a "BattleshipPlayerFromFile" I'm really stupid and nothing about the
	 *board interests me other than the numRows and numCols*/
	pBrd = new Board(numRows, numCols);

	// after we have the board we can detect wrong attacks (invalid coords)
	// so this is the correct time to call.
	bool loadingSuccess = loadAttacksFromFile();
}

