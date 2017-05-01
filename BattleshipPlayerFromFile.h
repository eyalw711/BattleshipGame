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

	~BattleshipPlayerFromFile() override; //TODO: why override?

	bool init(const std::string& path) override; // returns whether the init succeeded or failed

	bool loadAttacksFromFile();

	/* This function is my API to get my board */
	void setBoard(const char** board, int numRows, int numCols) override;
	
	/* My API to ask my attack */
	std::pair<int, int> attack() override; 
	
	/* My API to receive attack results */
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override; // notify on last move result

	char getId() const;
private:
	Board brd;
	std::queue<std::pair<int,int>> attacksQueue;
	char Id;
	string pathToAttacksFile;
};