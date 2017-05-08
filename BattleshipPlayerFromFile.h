#pragma once

#include "BoardClass.h"
#include "IBattleshipGameAlgo.h"
#include <queue>
using namespace std;


/*This is a player*/
class BattleshipPlayerFromFile : public IBattleshipGameAlgo
{
public:

	/*This ctor gets a path to attack file*/
	BattleshipPlayerFromFile();

	~BattleshipPlayerFromFile() override;

	bool init(const std::string& path) override; // returns whether the init succeeded or failed

	bool BattleshipPlayerFromFile::loadAttacksFromFile(string file);

	/* This function is my API to get my board */
	void setBoard(int player, const char** board, int numRows, int numCols) override;
	
	/* My API to ask my attack */
	std::pair<int, int> attack() override; 
	
	/* My API to receive attack results */
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override; // notify on last move result

	
private:
	int player_id;
	Board brd;
	std::queue<std::pair<int,int>> attacksQueue;
	string pathToAttacksFile;
};