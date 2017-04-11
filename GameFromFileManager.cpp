#include "stdafx.h"
#include "GameFromFileManager.h"
#include "BattleshipPlayerFromFile.h"
#include "Ship.h"
#include <iostream>

using namespace std;

GameFromFileManager::GameFromFileManager(Board *brd, string file_a, string file_b) : currPlayerInx(0),
	numOfPlayers(2), players{new BattleshipPlayerFromFile('A', file_a), new BattleshipPlayerFromFile('B', file_b)},
	scores{0,0}, brd(brd), a_ships(new vector<Ship>), b_ships(new vector<Ship>)
{			
	findShips('A');
	findShips('B');
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
		cout << "Too many ships for player A" << endl;
		is_valid = false;
	}
	if (num_of_a_ships < 5)
	{
		cout << "Too few ships for player A" << endl;
		is_valid = false;
	}
	
	// validate number of B ships
	int num_of_b_ships = numOfValidShips('B');
	if (num_of_b_ships > 5)
	{
		cout << "Too many ships for player B" << endl;
		is_valid = false;
	}
	if (num_of_b_ships < 5)
	{
		cout << "Too few ships for player B" << endl;
		is_valid = false;
	}

	// validate no adjacent ships
	vector<Ship> all_ships;
	all_ships.insert(all_ships.end(), a_ships->begin(), a_ships->end());
	all_ships.insert(all_ships.end(), b_ships->begin(), b_ships->end());
	for (unsigned int i = 0; i < all_ships.size()-1 ; i++)
		for (unsigned int j = i+1; j < all_ships.size(); j++)
		{
			if(all_ships[i].isAdjacentShips(all_ships[j]))
			{
				cout << "Adjacent Ships on Board" << endl;
				return false;
			}
		}
	return is_valid;
}

//invalid
bool GameFromFileManager::isGameOn() const
{
	bool foundLowerCase = false, foundUpperCase = true;
	Board& board = (*brd);
	for (int i = 0; i < board.getNumOfRows(); i++)
	{
		for (int j = 0; j < board.getNumOfCols(); j++)
		{
			if (isupper(board(i, j)))
				foundUpperCase = true;

			else if (islower(board(i, j)))
				foundLowerCase = true;
		}
	}
	if ((foundLowerCase && !foundUpperCase) || (!foundLowerCase && foundUpperCase)) //TODO: why this condition?
		return true;

	return false;
}

void GameFromFileManager::mainLoop()
{
	Board& board = (*brd);

	while (isGameOn())
	{
		BattleshipPlayerFromFile& currPlayer = *(players[currPlayerInx]);
		do
		{
			auto attack = currPlayer.attack();
			if (board(attack.first, attack.second) == Board::SEA)
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
				
				//if ship sinks grant points to enemy

				// notify players
				players[0]->notifyOnAttackResult(currPlayerInx, attack.first,
					attack.second, AttackResult::Miss);
				players[1]->notifyOnAttackResult(currPlayerInx, attack.first,
					attack.second, AttackResult::Miss);
				
				//pass turn
				currPlayerInx = (currPlayerInx + 1) % 2;
				break;
			}
			else
			{
				//player hits enemy
				board.setSlot(attack.first, attack.second, Board::SEA);

				//if ship sinks grant points to player

				//notify players

				//next turn = no break
			}
		} while (true);
	}

	//TODO: end of game -- notify winner and score
}

int GameFromFileManager::getNumOfPlayers() const
{
	return numOfPlayers;
}

bool GameFromFileManager::selfHit(BattleshipPlayerFromFile& player, std::pair<int, int> attack) const
{
	Board& board = (*brd);
	if (player.getId() == 'A' && isupper(board(attack.first, attack.second)))
	{
		return true;
	}
	if (player.getId() == 'B' && islower(board(attack.first, attack.second)))
	{
		return true;
	}
	return false;
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
				vector<pair<int, int>> *coords = new vector<pair<int, int>>;
				char ship_char = copiedBoard(i, j);
				Ship::ship_type ship_type = static_cast<Ship::ship_type>(copiedBoard(i, j));
				revealSurroundings(i, j, ship_char, copiedBoard, *coords);				
				ships->push_back(Ship(ship_type, coords));
				delete coords;
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
			cout << "printing coordinated of ship number" << i+1 << " of " << user << endl;
			for(unsigned int j = 0; j < coord->size(); j++)
				cout << (((*coord)[j]).first).first << ", " << (((*coord)[j]).first).second << endl;
		}
}


