#include "stdafx.h"
#include "BoardClass.h"
#include <iostream>
#include <string>

using namespace std;


//non-default ctor
Board::Board(string data, int rows, int columns) : rows_(rows), columns_(columns), data_(new char[rows * columns])
{
	/*
	Will fill missing slots with spaces, and will ignore
	excessive slots
	*/
	cout << "non-default Board ctor activated" << endl;
	memset(data_, ' ', rows*columns);

	int len = data.length(); //doesn't count '\0'
	auto copyAmount = (len <= rows*columns) ? len : rows*columns;
	memcpy(data_, data.c_str(), copyAmount);
}

//dtor
Board::~Board()
{
	cout << "Board dtor activated" << endl;
	delete[] data_;
}

//copy ctor
Board::Board(const Board &brd) : rows_(brd.rows_), columns_(brd.columns_), data_(new char[brd.rows_ * brd.columns_])
{
	cout << "copy Board ctor activated" << endl;
	memcpy(data_, brd.data_, brd.rows_ * brd.columns_);
}

Board& Board::operator=(const Board& other)
{
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


char Board::operator()(int row, int column) const
{
	if (-1 < row && row < rows_ && -1 < column && column < columns_)
	{
		return data_[columns_*row + column];
	}
	throw std::out_of_range("Trying to access invalid index in the board.");
}

