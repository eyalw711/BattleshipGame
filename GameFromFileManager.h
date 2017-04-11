#pragma once
#include "IBattleshipGameAlgo.h"
#include <queue>
#include "BoardClass.h"
#include "BattleshipPlayerFromFile.h"

class GameFromFileManager
{
public:
	//GameFromFileManager(Board *brd);
	GameFromFileManager(Board * brd, string fila_a, string file_b);
	bool isGameOn() const;
	void mainLoop();
	int getNumOfPlayers() const;
	void findShips(char user);
	void revealSurroundings(int row, int col, char ship_type, Board &brd, vector<pair<int, int>> &coords);
	int numOfValidShips(char user) const;
	void setPlayersBoards();
	void printShipsCoordinates(char user);
	bool isValidBoard() const;
	bool selfHit(BattleshipPlayerFromFile&, std::pair<int, int>) const;
private:
	int							currPlayerInx;
	int							numOfPlayers;
	BattleshipPlayerFromFile	*players[2];
	int							scores[2];
	Board						*brd;
	vector<Ship> *a_ships;
	vector<Ship> *b_ships;
};
