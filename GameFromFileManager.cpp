#include "stdafx.h"
#include "GameFromFileManager.h"
#include "BattleshipPlayerFromFile.h"
#include "Ship.h"
#include <iostream>
#include <fstream>
#include <sstream>
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

	/* reading file names in the path or current directory to file_names.txt */
	if (argc >= 2)
	{
		if (!is_valid_dir_path(argv[1]))
		{
			cout << "Wrong path: " << argv[1] << endl; //ReqPrnt
			return false;
		}

		string const command = string("dir ") + argv[1] + string(" /b /a-d > file_names.txt 2>&1");
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
			brd->SetBoardFromFile((file_board).c_str());
		}
		else
		{
			brd->SetBoardFromFile((string(argv[1]) + "\\" + file_board).c_str());
			file_a = string(argv[1]) + "\\" + file_a;
			file_b = string(argv[1]) + "\\" + file_b;
		}
		DEBUG(*brd);
		
		// find ships
		this->findShips('A');
		this->findShips('B');


		// check board validity
		if (!this->isValidBoard())
			return false;

		// printing coordinates of ships:
		//this->printShipsCoordinates('A');
		//this->printShipsCoordinates('B');

		players[0] = new BattleshipPlayerFromFile('A', file_a);
		players[1] = new BattleshipPlayerFromFile('B', file_b);
		DEBUG("*** board is valid ***");
		return true;
	}
	return false;
}

GameFromFileManager::GameFromFileManager() : currPlayerInx(0),
	numOfPlayers(2), players{nullptr, nullptr},
	scores{0,0}, brd(new Board(10,10)), a_ships(new vector<Ship>), b_ships(new vector<Ship>)
{
}

GameFromFileManager::~GameFromFileManager()
{
	delete brd;
	delete players[0];
	delete players[1];
	delete a_ships;
	delete b_ships;
}

//user should be 'A' or 'B'
int GameFromFileManager::numOfValidShips(char user) const
{
	int cnt = 0;
	vector<Ship> *ships;
	if (user == 'A')
		ships = a_ships;
	else
		ships = b_ships;

	for (vector<Ship>::iterator it = ships->begin(); it != ships->end(); ++it)
		if (it->getValid())
			cnt++;
	return cnt;
}

bool GameFromFileManager::askPlayersToLoadAttacks()
{
	bool success = players[0]->loadAttacksFromFile();
	success = success && players[1]->loadAttacksFromFile();
	return success;
}


void GameFromFileManager::setPlayersBoards()
{
	/* As a manager of stupid players, I know in advance they don't care about the
	 * board so I don't make special effort in giving it to them...
	 */
	players[0]->setBoard(nullptr, brd->getNumOfRows(), brd->getNumOfCols());
	players[1]->setBoard(nullptr, brd->getNumOfRows(), brd->getNumOfCols());
}

bool GameFromFileManager::isValidBoard() const
{
	Ship::ship_type ship_types_A[4] = { Ship::ship_type::A_BOAT, Ship::ship_type::A_SATIL, Ship::ship_type::A_SUBMARINE, Ship::ship_type::A_DESTROYER };
	Ship::ship_type ship_types_B[4] = { Ship::ship_type::B_BOAT, Ship::ship_type::B_SATIL, Ship::ship_type::B_SUBMARINE, Ship::ship_type::B_DESTROYER };
	bool is_valid = true;
	// validate shape of A ships
	for(int i = 0 ; i < 4 ; i++)
	{
		for (vector<Ship>::iterator it = a_ships->begin(); it != a_ships->end(); ++it)
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
		for (vector<Ship>::iterator it = b_ships->begin(); it != b_ships->end(); ++it)
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
	all_ships.insert(all_ships.end(), a_ships->begin(), a_ships->end());
	all_ships.insert(all_ships.end(), b_ships->begin(), b_ships->end());
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

	if (allSunk(a_ships) || allSunk(b_ships))
		return false;

	return true;
}

bool GameFromFileManager::allSunk(vector<Ship> *ships)
{
	for (vector<Ship>::iterator it = ships->begin(); it != ships->end(); ++it)
	{
		if (!it->isSunk())
			return false;
	}
	return true;
}

void GameFromFileManager::mainLoop()
{
	Board& board = (*brd);

	bool OutOfAttacks[2] = { false, false };

	while (isGameOn(OutOfAttacks))
	{
		BattleshipPlayerFromFile& currPlayer = *(players[currPlayerInx]);
		do
		{
			auto attack = currPlayer.attack();
			DEBUG("player " << currPlayerInx << " attack is "  << attack.first << ", " << attack.second);
			DEBUG("Points:");
			DEBUG("Player A: " << scores[0]);
			DEBUG("Player B: " << scores[1]);

			if (attack.first == -1 && attack.second == -1)
			{
				OutOfAttacks[currPlayerInx] = true;
				//no more attacks for this player
				currPlayerInx = (currPlayerInx + 1) % 2;
				break;
			}
			if (board(attack.first, attack.second) == Board::SEA || getShipAtCrd(attack.first, attack.second)->isSunk())
			{
				//notify players
				players[0]->notifyOnAttackResult(currPlayerInx, attack.first,
					attack.second, AttackResult::Miss);
				players[1]->notifyOnAttackResult(currPlayerInx, attack.first,
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

				//if ship sinks grant points to enemy
				if (shipPtr->isSunk())
					scores[(currPlayerInx + 1) % 2] += shipPtr->getScoreForSinking();

				// notify players
				players[0]->notifyOnAttackResult(currPlayerInx, attack.first,
					attack.second, shipPtr->isSunk() ? AttackResult::Sink : AttackResult::Hit);
				players[1]->notifyOnAttackResult(currPlayerInx, attack.first,
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

				//if ship sinks grant points to player
				if (shipPtr->isSunk())
					scores[currPlayerInx] += shipPtr->getScoreForSinking();

				//notify players
				players[0]->notifyOnAttackResult(currPlayerInx, attack.first,
					attack.second, shipPtr->isSunk() ? AttackResult::Sink : AttackResult::Hit);
				players[1]->notifyOnAttackResult(currPlayerInx, attack.first,
					attack.second, shipPtr->isSunk() ? AttackResult::Sink : AttackResult::Hit);


				if (!isGameOn(OutOfAttacks)) //which means this hit caused victory
					break;

				//else; next turn = no break
			}
		} while (true);
	}

	if (allSunk(a_ships))
		cout << "Player B won" << endl; //Requirement print
	if (allSunk((b_ships)))
		cout << "Player A won" << endl; //Requirement print

	//Requirement prints
	cout << "Points:" << endl;
	cout << "Player A: " << scores[0] << endl;
	cout << "Player B: " << scores[1] << endl;
}

int GameFromFileManager::getNumOfPlayers() const
{
	return numOfPlayers;
}

bool GameFromFileManager::selfHit(BattleshipPlayerFromFile& player, std::pair<int, int> attack)
{
	Board& board = (*brd);
	Ship *shipPtr = this->getShipAtCrd(attack.first, attack.second);
	if (shipPtr != nullptr)
	{
		if (player.getId() == 'A')
			return (shipPtr->isAShip() and !shipPtr->isSunk());
		else
			return (shipPtr->isBShip() and !shipPtr->isSunk());
	}

	return false;
}

Ship *GameFromFileManager::getShipAtCrd(int row, int col)
{
	Board& board = (*brd);
	vector<Ship> *ships;
	if ((board.isInBoard(row, col)) && (board(row, col) != Board::SEA))
	{
		if (isupper(board(row,col)))
			ships = a_ships;
		else
			ships = b_ships;
		
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
void GameFromFileManager::findShips(char user)
{
	Board copiedBoard(*brd);
	vector<Ship> *ships = new vector<Ship>;
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
				ships->push_back(Ship(ship_type, &coords));
			}
		}
	}
	if (user == 'A')
		a_ships = ships;
	else
		b_ships = ships;
}


void GameFromFileManager::printShipsCoordinates(char user)
{
	vector<Ship> *ships = user == 'A' ? a_ships : b_ships;
	for (unsigned int i = 0; i < ships->size(); i++)
	{
		vector<pair<pair<int, int>, bool>> *coord = (*ships)[i].getCoordinates();
		DEBUG("printing coordinated of ship number" << i + 1 << " of " << user);
		for (unsigned int j = 0; j < coord->size(); j++)
			DEBUG((((*coord)[j]).first).first << ", " << (((*coord)[j]).first).second);
	}
}


