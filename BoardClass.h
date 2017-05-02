#pragma once
#include <string>
#include <vector>
#include "Ship.h"

using namespace std;

std::istream& safeGetline(std::istream& is, std::string& t);

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

	static const int DEFAULT_BOARD_WIDTH = 10;

	Board() : Board(DEFAULT_BOARD_WIDTH, DEFAULT_BOARD_WIDTH) {}//empty constructor
	Board(int rows, int columns); //non-default
	Board(const Board&); //copy ctor
	Board& operator=(const Board&);
	char operator()(int, int) const;
	friend std::ostream& operator<<(std::ostream &strm, const Board &brd);
	~Board();



	bool isInBoard(int row, int col);
	static bool isLegalBoardElement(char c);
	static bool isUserShip(char user, char marineObject);
	static bool isAShip(char marineObject);
	static bool isBShip(char marineObject);

	void setSlot(int row, int col, char marineObject);
	bool SetBoardFromFile(const char* path);

	int getNumOfRows() const;
	int getNumOfCols() const;

private:
	int rows_;
	int columns_;
	char *data_;





};