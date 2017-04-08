#pragma once
#include <string>
#include <vector>
#include "Ship.h"

using namespace std;

class Board
{
public:
	static const char SEA			= ' ';
	static const char A_BOAT		= 'B';
	static const char A_SATIL		= 'P';
	static const char A_SUBMARINE	= 'M';
	static const char A_DESTROYER	= 'D';
	static const char B_BOAT		= 'b';
	static const char B_SATIL		= 'p';
	static const char B_SUBMARINE	= 'm';
	static const char B_DESTROYER	= 'd';

	Board(int rows, int columns);
	Board(string, int, int);
	void SetBoardFromFile(const char* path);
	Board(const Board&);
	Board& operator=(const Board&);
	char operator()(int, int) const;
	static bool isLegalBoardElement(char c);
	static bool isUserShip(char user, char marineObject);
	static bool isAShip(char marineObject);
	static bool isBShip(char marineObject);

	void setSlot(int row, int col, char marineObject);

	int getNumOfRows() const;
	int getNumOfCols() const;
	void findShips(char user);
	friend std::ostream& operator<<(std::ostream &strm, const Board &brd);
	~Board();
private:
	int rows_;
	int columns_;
	char *data_;
	Ship a_ships[5];
	Ship b_ships[5];
};