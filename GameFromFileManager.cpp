#include "stdafx.h"
#include "GameFromFileManager.h"
#include "BattleshipPlayerFromFile.h"
#include "Ship.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <iso646.h>
using namespace std;



GameFromFileManager::GameFromFileManager(GameFromFileManager& other)
{
	DEBUG("Oh no! copy ctor of GameFromFileManager was called!");
}

GameFromFileManager& GameFromFileManager::operator=(GameFromFileManager& other)
{
	DEBUG("Oh no, operator= of GameFromFileManager was used!");
	return other;
}

bool is_valid_dir_path(const char *pathname)
{
	struct stat info;

	if ((stat(pathname, &info) != 0) || !(info.st_mode & S_IFDIR))
		return false;
	return true;
}

bool GameFromFileManager::initialize_board(string file_board)
{
	bool set_boards_sucess = true;
	// set board using file
	set_boards_sucess = brd.SetBoardFromFile((file_board).c_str());
	if (!set_boards_sucess)
	{
		std::cout << "Error while open board file" << std::endl;
		return false;
	}
	//DEBUG(*brd);
	if (!Utils::get_quiet())
		cout << brd << endl;

	// find ships
	this->findShips('A', players[PLAYER_A].ships);
	this->findShips('B', players[PLAYER_B].ships);


	// check board validity
	if (!this->isValidBoard())
		return false;
	DEBUG("*** board is valid ***");

	// printing coordinates of ships:
	//this->printShipsCoordinates('A');
	//this->printShipsCoordinates('B');
	return true;
}

string GameFromFileManager::find_attack_path(const string& path_expr_to_find, int player_id)
{
	WIN32_FIND_DATAA fileData;
	HANDLE hFind;
	string first_file, second_file; // lexiogrphic order
	string retVal = "", tmp_str = "";
	hFind = FindFirstFileA(path_expr_to_find.c_str(), &fileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
			retVal = FILE_NOT_FOUND;
		//else
		//	retVal = ERROR_FINDING_PATH;
	}

	else
	{
		first_file = fileData.cFileName;
		second_file = fileData.cFileName;
		do
		{
			tmp_str = fileData.cFileName;
			if (first_file.compare(tmp_str) > 0)
			{
				second_file = first_file;
				first_file = tmp_str;
			}
			else if (second_file.compare(tmp_str) > 0 || first_file.compare(second_file) == 0)
			{
				second_file = tmp_str;
			}

		} while (FindNextFileA(hFind, &fileData));
		if (player_id == PLAYER_A)
			retVal = first_file;
		else
			retVal = second_file;
		FindClose(hFind);
	}
	return retVal;
}

bool GameFromFileManager::initialize_players(string file_a, string file_b)
{
	bool retVal = true;
	players[PLAYER_A].algo = new BattleshipPlayerFromFile(); //TODO: create dll IAlgo!
	players[PLAYER_A].algo->setBoard(PLAYER_A, nullptr, brd.getNumOfRows(), brd.getNumOfCols()); //TODO: create the board really!
	retVal &= players[PLAYER_A].algo->init(file_a);
	players[PLAYER_B].algo = new BattleshipPlayerFromFile();
	players[PLAYER_B].algo->setBoard(PLAYER_B, nullptr, brd.getNumOfRows(), brd.getNumOfCols());
	retVal &= players[PLAYER_B].algo->init(file_b);

	return retVal;
}

pair<bool, string> GameFromFileManager::parse_command_line_arguments(int argc, char *argv[])
{
	string dir_path = ".";
	/* parsing command line arguments */
	for (int i = 0; i < argc; i++)
	{
		string argument = argv[i];
		if (argument == "-quiet")
			Utils::set_quiet(true);
		if (argument == "-delay")
			Utils::set_delay(atoi(argv[i + 1]));
	}
	if (argc >= 2)
	{
		if (!is_valid_dir_path(argv[1]))
		{
			cout << "Wrong path: " << argv[1] << endl; //ReqPrnt
			return make_pair(false, dir_path);
		}
		dir_path = string(argv[1]);
	}
	return make_pair(true, dir_path);
}
bool GameFromFileManager::initialize(int argc, char *argv[])
{
	bool find_board = false, find_a = false, find_b = false;
	string dir_path = ".", file_board, file_a, file_b, find_file_ret_val;
	pair<bool, string> parser_ret_val = parse_command_line_arguments(argc, argv);
	dir_path = parser_ret_val.second;
	if (parser_ret_val.first == false)
		return false;
	/* find files */
	find_file_ret_val = Utils::find_path(dir_path + "\\*.sboard");
	if (find_file_ret_val != FILE_NOT_FOUND)
	{
		file_board = dir_path + "\\" + find_file_ret_val;
		find_board = true;
	}	
	find_file_ret_val = find_attack_path(dir_path + "\\*.attack", PLAYER_A);
	if (find_file_ret_val != FILE_NOT_FOUND)
	{
		file_a = dir_path + "\\" + find_file_ret_val;
		find_a = true;
	}
	find_file_ret_val = find_attack_path(dir_path + "\\*.attack", PLAYER_B);
	if (find_file_ret_val != FILE_NOT_FOUND)
	{
		file_b = dir_path + "\\" + find_file_ret_val;
		find_b = true;
	}			
	string path = (argc == 1) ? "Working Directory" : argv[1];
	if (!find_board)
		cout << "Missing board file (*.sboard) looking in path: " << path << endl; //ReqPrnt
	if (!find_a)
		cout << "Missing attack file for player A (*.attack) looking in path: " << path << endl; //ReqPrnt
	if (!find_b)
		cout << "Missing attack file for player B (*.attack) looking in path: " << path << endl; //ReqPrnt
	if (find_board && find_a && find_b)
	{
		if (initialize_board(file_board) == false)
			return false;
		return initialize_players(file_a, file_b);
	}
	return false;
}


GameFromFileManager::GameFromFileManager() : currPlayerInx(0),
numOfPlayers(number_of_players), brd(Board())
{
	PlayerData player1;
	PlayerData player2;

	player1.score = 0;
	player2.score = 0;

	player1.id = 'A';
	player2.id = 'B';

	player1.algo = nullptr;
	player2.algo = nullptr;

	players[PLAYER_A] = player1;
	players[PLAYER_B] = player2;

	
}

GameFromFileManager::~GameFromFileManager()
{
	for(PlayerData player: players)
	{
		delete player.algo;
	}
}

//user should be 'A' or 'B'
int GameFromFileManager::numOfValidShips(char user) const
{
	int cnt = 0;
	const vector<Ship> *ships;
	if (user == 'A')
		ships = &players[PLAYER_A].ships;
	else
		ships = &players[PLAYER_B].ships;

	for (vector<Ship>::const_iterator it = ships->begin(); it != ships->end(); ++it)
		if (it->getValid())
			cnt++;
	return cnt;
}


void GameFromFileManager::setPlayersBoards()
{
	/* As a manager of stupid players, I know in advance they don't care about the
	 * board so I don't make special effort in giving it to them...
	 */
	
}

bool GameFromFileManager::validate_ships_shape(int player_id) const
{
	Ship::ship_type ship_types_A[4] = { Ship::ship_type::A_BOAT, Ship::ship_type::A_SATIL, Ship::ship_type::A_SUBMARINE, Ship::ship_type::A_DESTROYER };
	Ship::ship_type ship_types_B[4] = { Ship::ship_type::B_BOAT, Ship::ship_type::B_SATIL, Ship::ship_type::B_SUBMARINE, Ship::ship_type::B_DESTROYER };
	Ship::ship_type *ship_types;
	if (player_id == PLAYER_A)
		ship_types = ship_types_A;
	else
		ship_types = ship_types_B;
	bool is_valid = true;
	// validate shape of A ships
	for (int i = 0; i < 4; i++)
	{
		for (vector<Ship>::const_iterator it = players[player_id].ships.begin(); it != players[player_id].ships.end(); ++it)
		{
			if (it->getType() == ship_types[i] && !it->getValid())
			{
				//Requirement print
				if (player_id == PLAYER_A)
					cout << "Wrong size or shape for ship " << static_cast<char> (ship_types_A[i]) << " for player A" << endl;
				else
					cout << "Wrong size or shape for ship " << static_cast<char> (ship_types_B[i]) << " for player B" << endl;
				is_valid = false;
				break;
			}
		}
	}
	return is_valid;
}

bool GameFromFileManager::validate_no_adjacent_ships() const
{
	vector<Ship> all_ships;
	all_ships.insert(all_ships.end(), players[PLAYER_A].ships.begin(), players[PLAYER_A].ships.end());
	all_ships.insert(all_ships.end(), players[PLAYER_B].ships.begin(), players[PLAYER_B].ships.end());
	for (int i = 0; i < static_cast<int>(all_ships.size()) - 1; i++)
	{
		for (unsigned int j = i + 1; j < all_ships.size(); j++)
		{
			if (all_ships[i].isAdjacentShips(all_ships[j]))
			{
				//Requirement print
				cout << "Adjacent Ships on Board" << endl;
				return false;
			}
		}
	}
	return true;
}
bool GameFromFileManager::isValidBoard() const
{
	// validate shape of ships
	bool is_valid = validate_ships_shape(PLAYER_A) && validate_ships_shape(PLAYER_B);
	// validate number of A ships
	int num_of_a_ships = numOfValidShips('A');
	if (num_of_a_ships > 5)
	{
		//Requirement print
		cout << "Too many ships for player A" << endl;
		is_valid = false;
	}
	if (num_of_a_ships < 5)
	{
		//Requirement print
		cout << "Too few ships for player A" << endl;
		is_valid = false;
	}	
	// validate number of B ships
	int num_of_b_ships = numOfValidShips('B');
	if (num_of_b_ships > 5)
	{
		//Requirement print
		cout << "Too many ships for player B" << endl;
		is_valid = false;
	}
	if (num_of_b_ships < 5)
	{
		//Requirement print
		cout << "Too few ships for player B" << endl;
		is_valid = false;
	}

	// validate no adjacent ships
	is_valid &= validate_no_adjacent_ships();
	return is_valid;
}


bool GameFromFileManager::isGameOn(bool *outOfAttacks) const
{
	if (outOfAttacks[0] && outOfAttacks[1])
		return false;

	if (allSunk(players[PLAYER_A].ships) || allSunk(players[PLAYER_B].ships))
		return false;

	return true;
}

bool GameFromFileManager::allSunk(const vector<Ship>& ships)
{
	for (vector<Ship>::const_iterator it = ships.begin(); it != ships.end(); ++it)
	{
		if (!it->isSunk())
			return false;
	}
	return true;
}

void GameFromFileManager::update_board_print(pair<int, int> attack) 
{
	Board& board = brd;
	COORD hit_coord;
	hit_coord.Y = attack.first;
	hit_coord.X = attack.second;
	if (board(attack.first, attack.second) == Board::SEA)
		Utils::updateBoardPrint(hit_coord, Board::SEA);
	else
		Utils::updateBoardPrint(hit_coord, Utils::HIT_SIGN);
}

void GameFromFileManager::notify_players(int currPlayerInx, pair<int, int> attack, const Ship *shipPtr, bool is_hit) const
{
	if(is_hit)
	{
		players[PLAYER_A].algo->notifyOnAttackResult(currPlayerInx, attack.first,
			attack.second, shipPtr->isSunk() ? AttackResult::Sink : AttackResult::Hit);
		players[PLAYER_B].algo->notifyOnAttackResult(currPlayerInx, attack.first,
			attack.second, shipPtr->isSunk() ? AttackResult::Sink : AttackResult::Hit);
	}
	else
	{
		players[PLAYER_A].algo->notifyOnAttackResult(currPlayerInx, attack.first,
			attack.second, AttackResult::Miss);
		players[PLAYER_B].algo->notifyOnAttackResult(currPlayerInx, attack.first,
			attack.second, AttackResult::Miss);
	}
}

void GameFromFileManager::mainLoopEndOfGamePrint() const
{
	if (allSunk(players[PLAYER_A].ships))
		cout << "Player B won" << endl; //Requirement print
	if (allSunk((players[PLAYER_B].ships)))
		cout << "Player A won" << endl; //Requirement print
	cout << "Points:" << endl; 	//Requirement print
	cout << "Player A: " << players[PLAYER_A].score << endl; //Requirement print
	cout << "Player B: " << players[PLAYER_B].score << endl; //Requirement print
}
void GameFromFileManager::make_hit(int currPlayerInx, pair<int, int> attack, bool is_self_hit)
{
	Ship *shipPtr = getShipAtCrd(attack.first, attack.second); //player hits itself
	shipPtr->hitAt(attack.first, attack.second);
	update_board_print(attack); //update board print				
	if (shipPtr->isSunk()) //if ship sinks grant points to enemy
	{
		if (is_self_hit)
			players[(currPlayerInx + 1) % 2].score += shipPtr->getScoreForSinking();
		else
			players[currPlayerInx].score += shipPtr->getScoreForSinking();
	}
	notify_players(currPlayerInx, attack, shipPtr);	// notify players			
}

void GameFromFileManager::mainLoop()
{
	Board& board = brd;
	bool OutOfAttacks[2] = { false, false };
	while (isGameOn(OutOfAttacks))
	{
		PlayerData& currPlayer = players[currPlayerInx];
		do
		{
			auto attack = currPlayer.algo->attack();
			DEBUG("player " << currPlayerInx << " attack is "  << attack.first << ", " << attack.second);
			DEBUG("Points:");
			DEBUG("Player A: " << players[PLAYER_A].score);
			DEBUG("Player B: " << players[PLAYER_B].score);
			if (attack.first == -1 && attack.second == -1) //no more attacks for this player
			{
				OutOfAttacks[currPlayerInx] = true;				
				currPlayerInx = (currPlayerInx + 1) % 2;
				break;
			}
			if (board(attack.first, attack.second) == Board::SEA || getShipAtCrd(attack.first, attack.second)->isSunk())
			{				
				update_board_print(attack); //update board print				
				notify_players(currPlayerInx, attack, nullptr, false); //notify players				
				currPlayerInx = (currPlayerInx + 1) % 2; //nothing happens and the turn passes
				break;
			}
			else if (selfHit(currPlayer, attack))
			{
				make_hit(currPlayerInx, attack, true);
				currPlayerInx = (currPlayerInx + 1) % 2; //pass turn
				break;
			}
			else
			{
				make_hit(currPlayerInx, attack);
				if (!isGameOn(OutOfAttacks)) //which means this hit caused victory
					break;
			}
		} while (true);
	}
	mainLoopEndOfGamePrint();
}

int GameFromFileManager::getNumOfPlayers() const
{
	return numOfPlayers;
}

bool GameFromFileManager::selfHit(PlayerData& player, std::pair<int, int> attack)
{
	Board& board = brd;
	Ship *shipPtr = this->getShipAtCrd(attack.first, attack.second);
	if (shipPtr != nullptr)
	{
		if (player.id == 'A')
			return (shipPtr->isAShip() and !shipPtr->isSunk());
		else
			return (shipPtr->isBShip() and !shipPtr->isSunk());
	}

	return false;
}

Ship *GameFromFileManager::getShipAtCrd(int row, int col)
{
	Board& board = brd;
	vector<Ship> *ships;
	if ((board.isInBoard(row, col)) && (board(row, col) != Board::SEA))
	{
		if (isupper(board(row,col)))
			ships = &players[PLAYER_A].ships;
		else
			ships = &players[PLAYER_B].ships;
		
		for (vector<Ship>::iterator it = ships->begin(); it != ships->end(); ++it)
		{
			if (it->containsCoord(row, col))
				return &(*it);
		}
	}
	return nullptr;
}

void GameFromFileManager::revealSurroundings(int row, int col, char ship_char, Board &board, vector<pair<int, int>> &coords)
{
	if (board(row, col) == ship_char)
	{
		board.setSlot(row, col, Board::SEA);
		coords.push_back(make_pair(row, col));
		int rows[2] = { row + 1, row - 1 };
		int cols[2] = { col + 1, col - 1 };
		for (int i = 0; i < 2 ; i++)
		{
				if(board.isInBoard(row, cols[i]))
					revealSurroundings(row,cols[i], ship_char, board, coords);
				if (board.isInBoard(rows[i], col))
					revealSurroundings(rows[i], col, ship_char, board, coords);
		}
	}
}

//user should be 'A' or 'B'
void GameFromFileManager::findShips(char user, vector<Ship>& ships)
{
	Board copiedBoard(brd);
	for (int i = 1; i <= copiedBoard.getNumOfRows(); i++)
	{
		for (int j = 1; j <= copiedBoard.getNumOfCols(); j++)
		{
			if (Board::isUserShip(user, copiedBoard(i, j))) //if ship found
			{
				vector<pair<int, int>> coords = vector<pair<int, int>>();
				char ship_char = copiedBoard(i, j);
				Ship::ship_type ship_type = static_cast<Ship::ship_type>(copiedBoard(i, j));
				revealSurroundings(i, j, ship_char, copiedBoard, coords);				
				ships.push_back(Ship(ship_type, &coords));
			}
		}
	}

}


void GameFromFileManager::printShipsCoordinates(char user)
{
	vector<Ship> *ships = user == 'A' ? &players[PLAYER_A].ships : &players[PLAYER_B].ships;
	for (unsigned int i = 0; i < ships->size(); i++)
	{
		vector<pair<pair<int, int>, bool>> coord = (*ships)[i].getCoordinates();
		DEBUG("printing coordinated of ship number" << i + 1 << " of " << user);
		for (unsigned int j = 0; j < coord.size(); j++)
			DEBUG((((*coord)[j]).first).first << ", " << (((*coord)[j]).first).second);
	}
}


