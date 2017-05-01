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

bool GameFromFileManager::initialize(int argc, char *argv[])
{
	bool find_board = false, find_a = false, find_b = false;
	string file_board, file_a, file_b;

	Utils::set_quiet(false);
	Utils::set_delay(Utils::DEFAULT_PRINT_DELAY);

	/* reading file names in the path or current directory to file_names.txt */
	for (int i= 0; i < argc ; i++)
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
			return false;
		}

		string const command = string("dir \"") + argv[1] + string("\" /b /a-d > file_names.txt 2>&1");
		system(command.c_str()); //insert to file_names.txt
	}
	else if (argc == 1)
	{
		system("dir /b /a-d > file_names.txt 2>&1");
	}

	/* reading file_name.txt and validate file types */
	std::ifstream infile("file_names.txt");
	std::string line;
	while (std::getline(infile, line))
	{
		if (line.substr(line.find_last_of(".") + 1) == "sboard")
		{
			find_board = true;
			file_board = line;
		}
		if (line.substr(line.find_last_of(".") + 1) == "attack-a")
		{
			find_a = true;
			file_a = line;
		}
		if (line.substr(line.find_last_of(".") + 1) == "attack-b")
		{
			find_b = true;
			file_b = line;
		}
	}

	string path = (argc == 1) ? "Working Directory" : argv[1];
	if (!find_board)
		cout << "Missing board file (*.sboard) looking in path: " << path << endl; //ReqPrnt
	if (!find_a)
		cout << "Missing attack file for player A (*.attack-a) looking in path: " << path << endl; //ReqPrnt
	if (!find_b)
		cout << "Missing attack file for player B (*.attack-b) looking in path: " << path << endl; //ReqPrnt

	if (find_board && find_a && find_b)
	{
		// set board using file
		if (argc == 1)
		{
			brd.SetBoardFromFile((file_board).c_str());
		}
		else
		{
			brd.SetBoardFromFile((string(argv[1]) + "\\" + file_board).c_str());
			file_a = string(argv[1]) + "\\" + file_a;
			file_b = string(argv[1]) + "\\" + file_b;
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
		bool retVal = true;

		players[0].algo = new BattleshipPlayerFromFile(); //TODO: create dll IAlgo!
		players[0].algo->setBoard(nullptr, brd.getNumOfRows(), brd.getNumOfCols()); //TODO: create the board really!
		retVal &= players[0].algo->init(file_a);
		players[1].algo = new BattleshipPlayerFromFile();
		players[1].algo->setBoard(nullptr, brd.getNumOfRows(), brd.getNumOfCols());
		retVal &= players[1].algo->init(file_b);
		return retVal;
	}
	return false;
}

/*
 *
 */
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

bool GameFromFileManager::isValidBoard() const
{
	Ship::ship_type ship_types_A[4] = { Ship::ship_type::A_BOAT, Ship::ship_type::A_SATIL, Ship::ship_type::A_SUBMARINE, Ship::ship_type::A_DESTROYER };
	Ship::ship_type ship_types_B[4] = { Ship::ship_type::B_BOAT, Ship::ship_type::B_SATIL, Ship::ship_type::B_SUBMARINE, Ship::ship_type::B_DESTROYER };
	bool is_valid = true;
	// validate shape of A ships
	for(int i = 0 ; i < 4 ; i++)
	{
		for (vector<Ship>::const_iterator it = players[PLAYER_A].ships.begin(); it != players[PLAYER_A].ships.end(); ++it)
		{
			if (it->getType() == ship_types_A[i] && !it->getValid())
			{
				//Requirement print
				cout << "Wrong size or shape for ship " << static_cast<char> (ship_types_A[i]) << " for player A" << endl;
				is_valid = false;
				break;
			}
		}
	}

	// validate shape of B ships
	for (int i = 0; i < 4; i++)
	{
		for (vector<Ship>::const_iterator it = players[PLAYER_B].ships.begin(); it != players[PLAYER_B].ships.end(); ++it)
		{
			if (it->getType() == ship_types_B[i] && !it->getValid())
			{
				//Requirement print
				cout << "Wrong size or shape for ship " << static_cast<char> (ship_types_B[i]) << " for player B" << endl;
				is_valid = false;
				break;
			}
		}
	}

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
	vector<Ship> all_ships;
	all_ships.insert(all_ships.end(), players[PLAYER_A].ships.begin(), players[PLAYER_A].ships.end());
	all_ships.insert(all_ships.end(), players[PLAYER_B].ships.begin(), players[PLAYER_B].ships.end());
	for (int i = 0; i < static_cast<int>(all_ships.size()) - 1; i++)
	{
		for (unsigned int j = i+1; j < all_ships.size(); j++)
		{
			if(all_ships[i].isAdjacentShips(all_ships[j]))
			{
				//Requirement print
				cout << "Adjacent Ships on Board" << endl;
				return false;
			}
		}
	}	
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

			if (attack.first == -1 && attack.second == -1)
			{
				OutOfAttacks[currPlayerInx] = true;
				//no more attacks for this player
				currPlayerInx = (currPlayerInx + 1) % 2;
				break;
			}
			if (board(attack.first, attack.second) == Board::SEA || getShipAtCrd(attack.first, attack.second)->isSunk())
			{
				//update board print
				COORD hit_coord;
				hit_coord.Y = attack.first;
				hit_coord.X = attack.second;
				if (board(attack.first, attack.second) == Board::SEA)
					Utils::updateBoardPrintMiss(hit_coord, Board::SEA);
				else
					Utils::updateBoardPrintMiss(hit_coord,'X');
				//notify players
				players[PLAYER_A].algo->notifyOnAttackResult(currPlayerInx, attack.first,
					attack.second, AttackResult::Miss);
				players[PLAYER_B].algo->notifyOnAttackResult(currPlayerInx, attack.first,
					attack.second, AttackResult::Miss);
				
				//nothing happens and the turn passes
				currPlayerInx = (currPlayerInx + 1) % 2;
				break;
			}
			else if (selfHit(currPlayer, attack))
			{
				//player hits itself
				Ship *shipPtr = getShipAtCrd(attack.first, attack.second);
				shipPtr->hitAt(attack.first, attack.second);


				//update board print
				COORD hit_coord;
				hit_coord.Y = attack.first;
				hit_coord.X = attack.second;
				Utils::updateBoardPrintHit(hit_coord);


				//if ship sinks grant points to enemy
				if (shipPtr->isSunk())
					players[(currPlayerInx + 1) % 2].score += shipPtr->getScoreForSinking();

				// notify players
				players[PLAYER_A].algo->notifyOnAttackResult(currPlayerInx, attack.first,
					attack.second, shipPtr->isSunk() ? AttackResult::Sink : AttackResult::Hit);
				players[PLAYER_B].algo->notifyOnAttackResult(currPlayerInx, attack.first,
					attack.second, shipPtr->isSunk() ? AttackResult::Sink : AttackResult::Hit);
				
				//pass turn
				currPlayerInx = (currPlayerInx + 1) % 2;
				break;
			}
			else
			{
				//player hits enemy
				Ship *shipPtr = getShipAtCrd(attack.first, attack.second);
				shipPtr->hitAt(attack.first, attack.second);

				//update board print
				COORD hit_coord;
				hit_coord.Y = attack.first;
				hit_coord.X = attack.second;
				Utils::updateBoardPrintHit(hit_coord);

				//if ship sinks grant points to player
				if (shipPtr->isSunk())
					players[currPlayerInx].score += shipPtr->getScoreForSinking();

				//notify players
				players[PLAYER_A].algo->notifyOnAttackResult(currPlayerInx, attack.first,
					attack.second, shipPtr->isSunk() ? AttackResult::Sink : AttackResult::Hit);
				players[PLAYER_B].algo->notifyOnAttackResult(currPlayerInx, attack.first,
					attack.second, shipPtr->isSunk() ? AttackResult::Sink : AttackResult::Hit);


				if (!isGameOn(OutOfAttacks)) //which means this hit caused victory
					break;

				//else; next turn = no break
			}
		} while (true);
	}

	if (allSunk(players[PLAYER_A].ships))
		cout << "Player B won" << endl; //Requirement print
	if (allSunk((players[PLAYER_B].ships)))
		cout << "Player A won" << endl; //Requirement print

	//Requirement prints
	cout << "Points:" << endl;
	cout << "Player A: " << players[PLAYER_A].score << endl;
	cout << "Player B: " << players[PLAYER_B].score << endl;
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


