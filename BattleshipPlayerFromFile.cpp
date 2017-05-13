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
	bool find_file = true;
	string find_file_ret_val = Utils::find_file(path + "\\*.attack", player_id, false, find_file);
	if (find_file)
	{
		string file = path + "\\" + find_file_ret_val;
		return loadAttacksFromFile(file);
	}
	return false;
	
	
}

void BattleshipPlayerFromFile::tryIgnoreCatchWhat(std::stringstream& ss)
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

void BattleshipPlayerFromFile::ignoreSpaces(std::stringstream& ss)
{
    while (isspace(ss.peek()))
        ss.ignore();
}




#define DEBUG_THEN_CONTINUE(x) DEBUG((x)); \
                            continue 

//read attacks
void BattleshipPlayerFromFile::parseAttacksFile(string line, ifstream& attacksFile) {
    while (safeGetline(attacksFile, line)) {
        int row, col;
        char c;
        std::stringstream ss(line);
        if (line.empty()) {                     //--stage0: ignore empty lines
            DEBUG_THEN_CONTINUE("Invalid empty row!");
        }
        ignoreSpaces(ss);                         //--stage1: ignore starter spaces
        if (isdigit(ss.peek())) {                 //--stage2: parse int or get out
            ss >> row;
        } else {
            DEBUG_THEN_CONTINUE("Expected a number as row!");
        }
        ignoreSpaces(ss);                         //--stage3: ignore spaces after row 
        if (ss.peek() == ',') {                   //--stage4: parse comma or get out
            tryIgnoreCatchWhat(ss);
        } else {
            DEBUG_THEN_CONTINUE("Expected a comma after row!");
        }
        while (isspace(ss.peek())) tryIgnoreCatchWhat(ss);      //--stage5: ignore spaces after comma
        if (isdigit(ss.peek())) {                                 //--stage6: parse int or get out
            ss >> col;
        } else {
            DEBUG_THEN_CONTINUE("Expected a number as column!");
        }
        bool stage7_cont = false;                               //--stage7: ignore spaces until end or get out
        while (ss >> c) {
            if (!isspace(c)) {
                DEBUG("Expected nothing after column!");
                stage7_cont = true;
            }
        }
        if (stage7_cont) continue;
        if (row < 1 || row > brd.getNumOfRows() || col < 1 ||  col > brd.getNumOfCols()) { //--stage8: check coords validity
            DEBUG_THEN_CONTINUE("Coordinates are invalid to board!");
        }
        attacksQueue.push(std::pair<int, int>(row, col)); //-- success,  queue attacks into attacksQueue
        DEBUG("Entered attack " << row << "," << col);
    }
    attacksFile.close();
}
#undef DEBUG_THEN_CONTINUE

/* returns true for success and false for error */
bool BattleshipPlayerFromFile::loadAttacksFromFile(string file)
{
	bool retVal = true;
	//-- open path
	string line;
	ifstream attacksFile(file);
	if (attacksFile.is_open())
	{
		parseAttacksFile(line, attacksFile);
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