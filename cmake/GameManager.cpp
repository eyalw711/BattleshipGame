#include "GameManager.h"
#include "BattleshipPlayerFromFile.h"
#include "Ship.h"
#include <iso646.h>
#include <direct.h>
#include "BattleshipNaivePlayer.h"
#include "Battleship_MAP_player.h"
using namespace std;



GameManager::GameManager(GameManager& other)
{
	DEBUG("Oh no! copy ctor of GameManager was called!");
}

GameManager& GameManager::operator=(GameManager& other)
{
	DEBUG("Oh no, operator= of GameManager was used!");
	return other;
}


bool GameManager::initialize_board(string file_board)
{
	bool set_boards_sucess = true;
	//clear terminal
	system("cls");
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
	this->brd.findShips(PLAYER_A, players[PLAYER_A].ships);
	this->brd.findShips(PLAYER_B, players[PLAYER_B].ships);


	// check board validity
	if (!this->isValidBoard())
		return false;
	DEBUG("*** board is valid ***");

	// printing coordinates of ships:
	//this->printShipsCoordinates('A');
	//this->printShipsCoordinates('B');
	return true;
}


void GameManager::free_board(const char** board) const
{
	for (int i = 0; i < brd.getNumOfRows(); i++)
		delete[] board[i];
	delete[] board;
}

bool GameManager::find_dll(string dir_path, int player_id, string& dll)
{
	string path = dir_path;
	if (dir_path.compare(".") == 0)
		path = "Working Directory";
	string s_dll = "\\*.dll"; // only .dll endings
	bool file_found = true;
	dll = Utils::find_file(dir_path + s_dll, player_id, true, file_found);
	if (!file_found)
	{
		cout << "Missing an algorithm (dll) file looking in path: " << path << endl;
		return false;
	}
	return true;
}

bool GameManager::initialize_player(string dir_path, int player_id)
{

	string dll;

	/* find DLLs*/
	bool retVal = find_dll(dir_path, player_id, dll);
	if (!retVal)
		return false;

	// Load dynamic library
	char full_path[_MAX_PATH];
	string full_dll_path = dir_path + "\\" + dll;
	_fullpath(full_path, full_dll_path.c_str(), _MAX_PATH);
	HINSTANCE hDll = LoadLibraryA(full_path); // Notice: Unicode compatible version of LoadLibrary
	if (!hDll)
	{
		std::cout << "Cannot load dll: " << full_path <<  std::endl;
		return false;
	}

	/* create A instance*/
	// Get function pointer
	// define function of the type we expect
	typedef IBattleshipGameAlgo *(*GetAlgoFunc)();
	GetAlgoFunc getFunc;
	getFunc = (GetAlgoFunc)GetProcAddress(hDll, "GetAlgorithm");
	if (!getFunc)
	{
		std::cout << "Cannot load dll: " << full_path << std::endl;
		return false;
	}
	players[player_id].algo = getFunc();
	const char** board = getBoardOfPlayer(player_id);
	players[player_id].algo->setBoard(player_id, board, brd.getNumOfRows(), brd.getNumOfCols());
	retVal &= players[player_id].algo->init(dir_path);
	if (!retVal)
	{
		std::cout << "Algorithm initialization failed for dll: " << full_path << std::endl;
		return false;
	}

	// cleanup
	free_board(board);

	return retVal;
}

bool GameManager::initialize_players(string dir_path)
{
	return initialize_player(dir_path, PLAYER_A) and initialize_player(dir_path, PLAYER_B);
}


pair<bool, string> GameManager::parse_command_line_arguments(int argc, char *argv[], bool& is_working_dir)
{
	string dir_path = ".";
	is_working_dir = (argc == 1);
	/* parsing command line arguments */
	for (int i = 1; i < argc; i++)
	{
		string argument = argv[i];
		if (argument == "-quiet")
		{
			Utils::set_quiet(true);
			if (i == 1)
				is_working_dir = true;
		}
		if (argument == "-delay")
		{
			Utils::set_delay(atoi(argv[i + 1]));
			if (i == 1)
				is_working_dir = true;
		}
	}
	if (!is_working_dir)
	{
		if (!Utils::is_valid_dir_path(argv[1]))
		{
			cout << "Wrong path: " << argv[1] << endl; //ReqPrnt
			return make_pair(false, dir_path);
		}
		dir_path = string(argv[1]);
	}
	return make_pair(true, dir_path);
}
bool GameManager::initialize(int argc, char *argv[])
{
	bool find_board = false, find_a = false, find_b = false, is_working_dir = false, find_path = true;
	string dir_path = ".", file_board, file_a, file_b, find_file_ret_val;
	pair<bool, string> parser_ret_val = parse_command_line_arguments(argc, argv, is_working_dir);
	char cCurrentPath[FILENAME_MAX];
	if (!_getcwd(cCurrentPath, sizeof(cCurrentPath)))
	{
		cout << "Error: can't extract current directory path" << endl;
		return false;

	}
	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
	dir_path = parser_ret_val.second;
	dir_path = is_working_dir ? string(cCurrentPath) : string(argv[1]);
	if (parser_ret_val.first == false)
		return false;
	/* find files */
	find_file_ret_val = Utils::find_path(dir_path + "\\*.sboard", find_path);
	if (find_path)
	{
		file_board = dir_path + "\\" + find_file_ret_val;
		find_board = true;
	}

	if (!find_board)
	{
		cout << "Missing board file (*.sboard) looking in path: " << dir_path << endl; //ReqPrnt
		string dummy_str;
		/* even if dll is missing for both players an error message will be printed only once, intentionally*/
		find_dll(dir_path, PLAYER_A, dummy_str);
	}
	else
	{
		if (initialize_board(file_board) == false)
			return false;
		return initialize_players(dir_path);
	}
	return false;
}


GameManager::GameManager() : currPlayerInx(0),
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

	player1.color = Utils::MAGNETA_COLOR;
	player2.color = Utils::GREEN_COLOR;

	players[PLAYER_A] = player1;
	players[PLAYER_B] = player2;	
}

GameManager::~GameManager()
{
	for(PlayerData player: players)
	{
		delete player.algo;
	}
}

//user should be 'A' or 'B'
int GameManager::numOfValidShips(char user) const
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


void GameManager::setPlayersBoards()
{
	/* As a manager of stupid players, I know in advance they don't care about the
	 * board so I don't make special effort in giving it to them...
	 */
	
}

bool GameManager::validate_ships_shape(int player_id) const
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

bool GameManager::validate_no_adjacent_ships() const
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
bool GameManager::isValidBoard() const
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


bool GameManager::isGameOn(bool *outOfAttacks) const
{
	if (outOfAttacks[0] && outOfAttacks[1])
		return false;

	if (allSunk(players[PLAYER_A].ships) || allSunk(players[PLAYER_B].ships))
		return false;

	return true;
}

bool GameManager::allSunk(const vector<Ship>& ships)
{
	for (vector<Ship>::const_iterator it = ships.begin(); it != ships.end(); ++it)
	{
		if (!it->isSunk())
			return false;
	}
	return true;
}

void GameManager::update_board_print(int player_color, pair<int, int> attack, int hit_color)
{
	Board& board = brd;
	COORD hit_coord;
	hit_coord.Y = attack.first;
	hit_coord.X = attack.second;
	if (board(attack.first, attack.second) == Board::SEA)
		Utils::updateBoardPrint(player_color, hit_coord, Board::SEA, hit_color);
	else
		Utils::updateBoardPrint(player_color, hit_coord, Utils::HIT_SIGN, hit_color);
}

void GameManager::notify_players(int currPlayerInx, pair<int, int> attack, const Ship *shipPtr, bool is_hit) const
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

void GameManager::mainLoopEndOfGamePrint() const
{
	if (allSunk(players[PLAYER_A].ships))
		cout << "Player B won" << endl; //Requirement print
	if (allSunk((players[PLAYER_B].ships)))
		cout << "Player A won" << endl; //Requirement print
	cout << "Points:" << endl; 	//Requirement print
	cout << "Player A: " << players[PLAYER_A].score << endl; //Requirement print
	cout << "Player B: " << players[PLAYER_B].score << endl; //Requirement print
}
void GameManager::make_hit(int currPlayerInx, pair<int, int> attack, bool is_self_hit)
{
	Ship *shipPtr = getShipAtCrd(attack.first, attack.second); //player hits itself
	shipPtr->hitAt(attack.first, attack.second);
	update_board_print(players[currPlayerInx % 2].color, attack, is_self_hit ? players[currPlayerInx % 2].color: players[(currPlayerInx + 1) % 2].color); //update board print				
	if (shipPtr->isSunk()) //if ship sinks grant points to enemy
	{
		if (is_self_hit)
			players[(currPlayerInx + 1) % 2].score += shipPtr->getScoreForSinking();
		else
			players[currPlayerInx].score += shipPtr->getScoreForSinking();
	}
	notify_players(currPlayerInx, attack, shipPtr);	// notify players			
}

void GameManager::mainLoop()
{
	Utils::ShowConsoleCursor(false);
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
				int hit_color = board(attack.first, attack.second) == Board::SEA || getShipAtCrd(attack.first, attack.second)->isAShip() ? players[PLAYER_A].color : players[PLAYER_B].color;
				update_board_print(players[(currPlayerInx) % 2].color, attack, hit_color); //update board print			
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
	Utils::ShowConsoleCursor(true);
	mainLoopEndOfGamePrint();
}

int GameManager::getNumOfPlayers() const
{
	return numOfPlayers;
}

bool GameManager::selfHit(PlayerData& player, std::pair<int, int> attack)
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

Ship *GameManager::getShipAtCrd(int row, int col)
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

/*void GameManager::revealSurroundings(int row, int col, char ship_char, Board &board, vector<pair<int, int>> &coords)
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
void GameManager::findShips(char user, vector<Ship>& ships)
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

}*/


void GameManager::printShipsCoordinates(char user)
{
	/*vector<Ship> *ships = user == 'A' ? &players[PLAYER_A].ships : &players[PLAYER_B].ships;
	for (unsigned int i = 0; i < ships->size(); i++)
	{
		vector<pair<pair<int, int>, bool>> coord = (*ships)[i].getCoordinates();
		DEBUG("printing coordinated of ship number" << i + 1 << " of " << user);
		for (unsigned int j = 0; j < coord.size(); j++)
			DEBUG((((*coord)[j]).first).first << ", " << (((*coord)[j]).first).second);
	}*/
}

// this function works only for 10X10 boards
// the board that is alocated here should be freed
const char** GameManager::getBoardOfPlayer(int player_id) const
{
	const int rows = brd.getNumOfRows();
	const int cols = brd.getNumOfCols();
	const char **board = new const char*[rows];
	for (int row = 0; row < rows; row++)
	{
		char *row_chars = new char[rows];
		for (int col = 0; col < cols; col++)
		{
			if ((player_id == PLAYER_A and Board::isBShip(brd(row + 1, col + 1))) or
				(player_id == PLAYER_B and Board::isAShip(brd(row + 1, col + 1))))
				row_chars[col] = Board::SEA;
			else
				row_chars[col] = brd(row + 1, col + 1);
		}
		board[row] = row_chars;		
	}
	return board;


}

