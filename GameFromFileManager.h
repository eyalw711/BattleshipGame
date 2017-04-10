#pragma once
#include "IBattleshipGameAlgo.h"
#include <queue>
#include "BoardClass.h"
#include "BattleshipPlayerFromFile.h"

class GameFromFileManager
{
public:
	GameFromFileManager(Board *brd);
	bool isGameOn() const;
	void mainLoop();
	int getNumOfPlayers() const;
	void findShips(char user);
	void revealSurroundings(int row, int col, char ship_type, Board &brd, vector<pair<int, int>> &coords);
	int numOfValidShips(char user);
	void printShipsCoordinates(char user);
	bool isValidBoard();
	bool selfHit(BattleshipPlayerFromFile&, std::pair<int, int>) const;
private:
	std::queue<std::pair<BattleshipPlayerFromFile, int>> playersAndScores;
	int numOfPlayers;
	Board *brd;
	vector<Ship> *a_ships;
	vector<Ship> *b_ships;
	string *attackFilesPaths;
};
