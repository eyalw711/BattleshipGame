#pragma once
#include "IBattleshipGameAlgo.h"
#include <queue>
#include "BoardClass.h"
#include "BattleshipPlayerFromFile.h"

class GameFromFileManager
{
public:

	//-- ctors and dtors
	GameFromFileManager();
	GameFromFileManager(GameFromFileManager & other);
	GameFromFileManager & operator=(GameFromFileManager & other);
	~GameFromFileManager();

	//-- getters
	int getNumOfPlayers() const;
	int numOfValidShips(char user) const;

	//--queries
	bool isGameOn(bool*) const;
	bool isValidBoard() const;
	bool selfHit(BattleshipPlayerFromFile&, std::pair<int, int>);

	Ship * getShipAtCrd(int row, int col);

	//-- manager logic
	bool askPlayersToLoadAttacks();
	bool initialize(int argc, char *argv[]);
	void mainLoop();
	void findShips(char user);
	void revealSurroundings(int row, int col, char ship_type, Board &brd, vector<pair<int, int>> &coords);
	void setPlayersBoards();

	//-- utils
	void printShipsCoordinates(char user);
	static bool allSunk(vector<Ship> *ships);

private:
	int							currPlayerInx;
	int							numOfPlayers;
	BattleshipPlayerFromFile	*players[2];
	int							scores[2];
	Board						*brd;
	vector<Ship> *a_ships;
	vector<Ship> *b_ships;
};
