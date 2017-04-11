#include "stdafx.h"
#include "GameFromFileManager.h"
#include "BattleshipPlayerFromFile.h"

GameFromFileManager::GameFromFileManager() : currPlayer(0) , numOfPlayers(2),
											scores{0,0}, brd(nullptr),
											a_ships(new vector<Ship>), b_ships(new vector<Ship>)
{
	//find both attack files + board
	char *attackFilesPaths[2];
		//finding...

	//load board + check validity of board

	//create all players
	for (int i = 0; i < numOfPlayers; i++)
	{
		players[i] = new BattleshipPlayerFromFile('A' + i, attackFilesPaths[i]);
	}
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
			if (isupper(board(i,j)))
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
		auto currPlayerScorePair = playersAndScores.front();
		playersAndScores.pop();
		auto currPlayer = currPlayerScorePair.first;

		do
		{
			auto attack = currPlayer.attack();
			if (board(attack.first, attack.second) == Board::SEA)
			{
				//nothing happens and the turn passes

				//notify players

				break;
			}
			else if (selfHit(currPlayer, attack))
			{
				//player hits itself
				board.setSlot(attack.first, attack.second, Board::SEA);
				//if ship sinks grant points to enemy

				// notify players

				//pass turn = break
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

		//end of turn - player goes to end of queue 
		playersAndScores.push(currPlayerScorePair);
	}
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

//user should be 'A' or 'B'
void GameFromFileManager::findShips(char user)
{
	/*Board copiedBoard(*brd);
	vector<Ship> *ships = new vector<Ship>;
	vector<pair<int, int>> coords;
	for (int i = 1; i < copiedBoard.getNumOfRows(); i++)
	{
		for (int j = 1; j < copiedBoard.getNumOfCols(); j++)
		{
			if (Board::isUserShip(user, copiedBoard(i, j)))
			{
				coords
			}
		}
	}*/
}


