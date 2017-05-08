#include "BattleshipPlayerFromFile.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Utils.h"

using namespace  std;

/* initializes the attacksQueue */
BattleshipPlayerFromFile::BattleshipPlayerFromFile() : 
	brd(Board()), attacksQueue(std::queue<std::pair<int, int>>())
{
	DEBUG("@@@@@@@@@@@@@@@@@@@@@@@@@@@player from file");
}

bool BattleshipPlayerFromFile::init(const std::string& path)
{
	string find_file_ret_val = Utils::find_file(path + "\\*.attack", player_id);
	if (find_file_ret_val != FILE_NOT_FOUND)
	{
		string file = path + "\\" + find_file_ret_val;
		return loadAttacksFromFile(file);
	}
	return false;
	
	
}

/* returns true for success and false for error */
bool BattleshipPlayerFromFile::loadAttacksFromFile(string file)
{
	bool retVal = true;

	//-- open path
	string line;
	ifstream attacksFile(file);
	if (attacksFile.is_open())
	{
		//read attacks
		while (safeGetline(attacksFile, line))
		{
			int row, col;
			char c;
			std::stringstream ss(line);
			//--stage0: ignore empty lines
			if (line.empty())
			{
				DEBUG("Invalid empty row!");
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
				DEBUG("Expected a number as row!");
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
					DEBUG(e.what());
				}
			}
			else
			{
				DEBUG("Expected a comma after row!");
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
					DEBUG(e.what());
				}
			}

			//--stage6: parse int or get out
			if (isdigit(ss.peek()))
				ss >> col;
			else
			{
				DEBUG("Expected a number as column!");
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
				DEBUG("Expected nothing after column!");
				continue; //why continue???
			}
			//--stage8: check coords validity
			if (row < 1 || row > brd.getNumOfRows() || col < 1 ||  col > brd.getNumOfCols())
			{
				DEBUG("Coordinates are invalid to board!");
				continue;
			}
			//-- success
			//queue attacks into attacksQueue
			attacksQueue.push(std::pair<int, int>(row, col));
			DEBUG("Entered attack " << row << "," << col);
		}
		attacksFile.close();
	}
	else
	{
		DEBUG("loadAttacksFromFile: Unable to open file");
		retVal = false;
	}
	return retVal;
}

BattleshipPlayerFromFile::~BattleshipPlayerFromFile()
{
}

/* returns next attack from file if there is any, else an invalid attack */
std::pair<int, int> BattleshipPlayerFromFile::attack()
{
	if (attacksQueue.empty())
	{
		return std::pair<int, int>(-1, -1);
	}
	else
	{
		auto retVal = attacksQueue.front();
		attacksQueue.pop();
		return retVal;
	}
}

void BattleshipPlayerFromFile::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
	//thank you but I am stupid and don't really care about the results of the attack :)
	return;
}


/* This function is my API to acquire my board. */
void BattleshipPlayerFromFile::setBoard(int player, const char ** board, int numRows, int numCols)
{
	/*as a "BattleshipPlayerFromFile" I'm really stupid and nothing about the
	 * board interests me other than the numRows and numCols*/
	brd = Board(numRows, numCols);
	player_id = player;
}


ALGO_API IBattleshipGameAlgo* GetAlgorithm()
{
	IBattleshipGameAlgo *algo = new BattleshipPlayerFromFile();
	return algo;
}