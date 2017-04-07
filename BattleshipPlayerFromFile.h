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
	BattleshipPlayerFromFile(char Id, string PathTOAttacksFile);

	~BattleshipPlayerFromFile() override;

	void loadAttacksFromFile();

	/* This function is my API to get my board */
	void setBoard(const char** board, int numRows, int numCols) override;
	
	/* My API to ask my attack */
	std::pair<int, int> attack() override; 
	
	/* My API to receive attack results */
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override; // notify on last move result

	char getId() const;
private:
	Board *pBrd;
	std::queue<std::pair<int,int>> *attacksQueue;
	char Id;
	string pathToAttacksFile;
};