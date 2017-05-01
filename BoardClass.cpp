#include "stdafx.h"
#include "BoardClass.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iso646.h>


using namespace std;

std::istream& safeGetline(std::istream& is, std::string& t)
{
	t.clear();

	// The characters in the stream are read one-by-one using a std::streambuf.
	// That is faster than reading them one-by-one using the std::istream.
	// Code that uses streambuf this way must be guarded by a sentry object.
	// The sentry object performs various tasks,
	// such as thread synchronization and updating the stream state.

	std::istream::sentry se(is, true);
	std::streambuf* sb = is.rdbuf();

	for (;;) {
		int c = sb->sbumpc();
		switch (c) {
		case '\n':
			return is;
		case '\r':
			if (sb->sgetc() == '\n')
				sb->sbumpc();
			return is;
		case EOF:
			// Also handle the case when the last line has no line ending
			if (t.empty())
				is.setstate(std::ios::eofbit);
			return is;
		default:
			t += (char)c;
		}
	}
}



//non-default ctor
Board::Board(int rows, int columns) : rows_(rows), columns_(columns), data_(new char[rows * columns])
{
	DEBUG("non-default Board ctor activated - board with only spaces created");
	memset(data_, ' ', rows*columns);
}


bool Board::SetBoardFromFile(const char* path)
{
	std::ifstream infile(path);
	std::string line;
	
	int row = 0;
	if (infile.is_open())
	{
		while (safeGetline(infile, line) && row < rows_)
		{
			std::stringstream ss(line);
			int column = 0;
			while (column < static_cast<int>(line.size()) && column < columns_)
			{
				if (isLegalBoardElement(ss.peek()))
				{
					data_[columns_*row + column] = ss.peek();
					ss.ignore();
				}
				else
					ss.ignore();
				column++;
			}
			row++;
		}
		infile.close();
		return true;
	}

	DEBUG("SetBoardFromFile: Unable to open file");
	return false;

}

//dtor
Board::~Board()
{
	DEBUG("Board dtor activated");
	delete[] data_;
}

//copy ctor
Board::Board(const Board &brd) : rows_(brd.rows_), columns_(brd.columns_), data_(new char[brd.rows_ * brd.columns_])
{
	DEBUG("copy Board ctor activated");
	memcpy(data_, brd.data_, brd.rows_ * brd.columns_);
}

Board& Board::operator=(const Board& other)
{
	DEBUG("operator= of Board was called");
	if (this != &other)
	{
		if (columns_*rows_ != other.columns_*other.rows_) //bummer
		{
			delete[] data_;
			data_ = new char[other.columns_*other.rows_];
		}
		columns_ = other.columns_;
		rows_ = other.rows_;
		memcpy(data_, other.data_, rows_*columns_);
	}
	//self assignment
	return *this;
}


/*In the game, indexing starts at 1*/
char Board::operator()(int row, int column) const
{
	row -= 1;
	column -= 1;
	if (-1 < row && row < rows_ && -1 < column && column < columns_)
	{
		return data_[columns_*row + column];
	}
	throw std::out_of_range("Trying to access invalid index in the board.");
}

bool Board::isLegalBoardElement(char marineObject)
{
	if (marineObject == Board::SEA ||
		marineObject == Board::A_BOAT ||
		marineObject == Board::A_SATIL ||
		marineObject == Board::A_SUBMARINE ||
		marineObject == Board::A_DESTROYER ||
		marineObject == Board::B_BOAT ||
		marineObject == Board::B_SATIL ||
		marineObject == Board::B_SUBMARINE ||
		marineObject == Board::B_DESTROYER)
	{
		return true;
	}
	return false;
}

//user should be 'A' or 'B'
bool Board::isUserShip(char user, char marineObject)
{
	if (user == 'A')
		return isAShip(marineObject);
	else if (user == 'B')
		return isBShip(marineObject);
	else
		throw std::logic_error("Invalid User");
}
bool Board::isAShip(char marineObject)
{
	if (marineObject == Board::A_BOAT ||
		marineObject == Board::A_SATIL ||
		marineObject == Board::A_SUBMARINE ||
		marineObject == Board::A_DESTROYER)
	{
		return true;
	}
	return false;
}

bool Board::isBShip(char marineObject)
{
	if (marineObject == Board::B_BOAT ||
		marineObject == Board::B_SATIL ||
		marineObject == Board::B_SUBMARINE ||
		marineObject == Board::B_DESTROYER)
	{
		return true;
	}
	return false;
}

void Board::setSlot(int row, int col, char marineObject)
{
	if (marineObject == Board::SEA         ||
		marineObject == Board::A_BOAT	   ||
		marineObject == Board::A_SATIL	   ||
		marineObject == Board::A_SUBMARINE ||
		marineObject == Board::A_DESTROYER ||
		marineObject == Board::B_BOAT	   ||
		marineObject == Board::B_SATIL	   ||
		marineObject == Board::B_SUBMARINE ||
		marineObject == Board::B_DESTROYER)
	{
		if (isInBoard(row,col))
		{
			row -= 1;
			col -= 1;
			data_[row*columns_ + col] = marineObject;
		}
		else
		{
			throw std::out_of_range("Indices out of range of the board!");
		}
	}
	else
	{
		throw std::logic_error("Invalid Marine Object!");
	}
}

int Board::getNumOfRows() const
{
	return rows_;
}

int Board::getNumOfCols() const
{
	return columns_;
}

bool Board::isInBoard(int row, int col)
{
	if (row >=1 and row <= rows_ and col >= 1 and col <= columns_)
		return true;
	return false;
}


std::ostream& operator<<(std::ostream &strm, const Board &brd) {
	// making columns
	strm << "    ";
	for(int i = 1 ; i <= brd.columns_ ; i++)
		strm << i << " ";
	strm << endl;

	for (int i = 1; i <= brd.rows_; i++)
	{
		if (i == 10)
			strm << i << "  ";
		else
			strm << i << "   ";
		for(int j = 1; j <= brd.columns_; j++)
		{
			strm << brd(i,j) << " ";
		}
		strm << endl;
	}
	return strm;
}

