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
	void GameFromFileManager::mainLoopEndOfGamePrint() const;
	void mainLoop();
	void GameFromFileManager::findShips(char user, vector<Ship>& ships); 
	void revealSurroundings(int row, int col, char ship_type, Board &brd, vector<pair<int, int>> &coords);
	void setPlayersBoards();

	//-- utils
	bool GameFromFileManager::initialize_board(string file_board);
	bool GameFromFileManager::initialize_players(string file_a, string file_b);
	bool GameFromFileManager::validate_ships_shape(int player_id) const;
	bool GameFromFileManager::validate_no_adjacent_ships() const;
	void GameFromFileManager::notify_players(int currPlayerInx, pair<int, int> attack, const Ship *shipPtr, bool is_hit = true) const;
	void GameFromFileManager::make_hit(int currPlayerInx, pair<int, int> attack, bool is_self_hit = false);
	void printShipsCoordinates(char user);
	static bool allSunk(const vector<Ship>& ships);
	void GameFromFileManager::update_board_print(pair<int, int> attack);
	pair<bool, string> GameFromFileManager::parse_command_line_arguments(int argc, char *argv[]);
	static string GameFromFileManager::find_attack_path(const string& path_expr_to_find, int player_id);
	static const int number_of_players = 2;
	static const int PLAYER_A = 0;
	static const int PLAYER_B = 1;

private:
	int				currPlayerInx;
	int				numOfPlayers;
	PlayerData		players[number_of_players];
	Board			brd;


};
