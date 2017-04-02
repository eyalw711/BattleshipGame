// BattleshipGame.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "IBattleshipGameAlgo.h"
#include <iostream>
#include <string>
#include <vector>


using namespace std;

const int BOARDLEN = 10;

class Board
{
public:
	//default ctor
	Board() : width_(BOARDLEN), data_(vector<char>(BOARDLEN*BOARDLEN, ' '))
	{}

	//non-default ctor
	Board(string data) : width_(BOARDLEN)
	{
		if (data.length() != BOARDLEN*BOARDLEN)
		{
			throw std::invalid_argument("Expected exactly 100 chars");
		}
		else
		{
			data_ = vector<char>(data.begin(), data.end());
		}
	}

	//copy ctor
	Board(const Board &brd) : width_(BOARDLEN)
	{
		data_ = brd.data_;
		cout << "copy constructor activated" << endl;
	}

	Board& operator=(const Board& other)
	{
		if (this != &other)
		{
			delete &data_;
			data_ = other.data_;
		}
		//self assignment
		return *this;
	}


	char operator()(int row, int column) const 
	{
		return data_[width_*row + column];
	}

private:
	int width_;
	std::vector<char> data_;
};


int main()
{
	Board b1 = Board();
	string str1 = string("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
	cout << str1.length() << endl;

		
	try
	{
		Board b2 = Board(str1);
		cout << b2(0, 0) << endl;
	}
	catch (exception e)
	{
		cout << e.what() << endl;
	}


	/*vector<int> v1(1, 10);
	vector<int> v2(3, 0);

	cout << "v1" << endl;
	for (int i = 0; i < v1.size(); ++i)
	{
		cout << v1[i] << endl;
	}

	cout << "v2" << endl;
	for (int i = 0; i < v2.size(); ++i)
	{
		cout << v2[i] << endl;
	}

	v2 = v1;
	v1.push_back(1);

	cout << "v1" << endl;
	for (int i = 0; i < v1.size(); ++i)
	{
		cout << v1[i] << endl;
	}

	cout << "v2" << endl;
	for (int i = 0; i < v2.size(); ++i)
	{
		cout << v2[i] << endl;
	}*/



	return 0;
}

