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
	void findShips(char user);
	bool selfHit(BattleshipPlayerFromFile&, std::pair<int, int>) const;
private:
	int							currPlayer;
	int							numOfPlayers;
	BattleshipPlayerFromFile	*players[2];
	int							scores[2];
	Board						*brd;
	vector<Ship> *a_ships;
	vector<Ship> *b_ships;
};
