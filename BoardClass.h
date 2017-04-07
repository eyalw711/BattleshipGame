#pragma once
#include <string>

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

	Board(string, int, int);
	Board(const Board&);
	Board& operator=(const Board&);
	char operator()(int, int) const;

	void setSlot(int row, int col, char marineObject);

	int getNumOfRows() const;
	int getNumOfCols() const;
	~Board();
private:
	int rows_;
	int columns_;
	char *data_;
};