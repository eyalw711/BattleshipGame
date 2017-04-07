#pragma once
#include "IBattleshipGameAlgo.h"
#include <queue>
#include "BoardClass.h"
#include "BattleshipPlayerFromFile.h"

class GameFromFileManager
{
public:
	GameFromFileManager();
	bool isGameOn() const;
	void mainLoop();
	int getNumOfPlayers() const;
	bool selfHit(BattleshipPlayerFromFile&, std::pair<int, int>) const;
private:
	std::queue<std::pair<BattleshipPlayerFromFile, int>> playersAndScores;
	int numOfPlayers;
	Board *brd;
	string *attackFilesPaths;
};
