#pragma once
#include "IBattleshipGameAlgo.h"
#include <queue>
#include "BoardClass.h"
#include "BattleshipPlayerFromFile.h"
#include "Utils.h"

typedef struct PlayerData
{
	IBattleshipGameAlgo *algo;
	char id;
	int score;
	vector<Ship> ships;

} PlayerData;
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
	bool GameFromFileManager::selfHit(PlayerData& player, std::pair<int, int> attack);

	Ship * getShipAtCrd(int row, int col);

	//-- manager logic
	bool initialize(int argc, char *argv[]);
	void mainLoop();
	void GameFromFileManager::findShips(char user, vector<Ship>& ships); 
	void revealSurroundings(int row, int col, char ship_type, Board &brd, vector<pair<int, int>> &coords);
	void setPlayersBoards();

	//-- utils
	void printShipsCoordinates(char user);
	static bool allSunk(const vector<Ship>& ships);
	static const int number_of_players = 2;
	static const int PLAYER_A = 0;
	static const int PLAYER_B = 1;

private:
	int				currPlayerInx;
	int				numOfPlayers;
	PlayerData		players[number_of_players];
	Board			brd;


};
