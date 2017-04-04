#pragma once
#include <string>

using namespace std;

class Board
{
public:

	Board(string, int, int);
	Board(const Board&);
	Board& operator=(const Board&);
	char operator()(int, int) const;

	~Board();
private:
	int rows_;
	int columns_;
	char *data_;
};