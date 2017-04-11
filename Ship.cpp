#include "stdafx.h"
#include "Ship.h"
#include <iostream>


Ship::Ship()
{
	
}
Ship::Ship(const Ship & ship) : type(ship.type), size(ship.size),
	coordinates(nullptr), valid(ship.valid)
{
	coordinates = new vector<pair<pair<int, int>, bool>>;
	vector<pair<pair<int, int>, bool>>& vec = *coordinates;

	vec = *(ship.coordinates); //overloaded operator= of vector: does deep copy
}

Ship& Ship::operator=(Ship& other)
{
	DEBUG("Oh no, operator= of Ship was used!");
	return other;
}

/*Ship::Ship(ship_type type, pair<int, int> start, pair<int, int> end) : type(type), start(start), end(end),
			 coordinates(new vector<pair<pair<int, int>, bool>>())
{
	// find size of ship
	switch (type)
	{
	case (ship_type::A_BOAT):
	case (ship_type::B_BOAT):
		size = BOAT_SIZE;
		break;
	case (ship_type::A_SATIL):
	case(ship_type::B_SATIL):
		size = SATIL_SIZE;
		break;
	case (ship_type::A_SUBMARINE):
	case(ship_type::B_SUBMARINE):
		size = SUBMARINE_SIZE;
		break;
	case (ship_type::A_DESTROYER):
	case(ship_type::B_DESTROYER):
		size = DESTROYER_SIZE;
		break;
	}

	// saving coordinates of ship
	int index = 0;
	for(int i = start.first; i < end.first ; i ++)
	{
		for (int j = start.second; j < end.second; j++)
		{
			coordinates->push_back(make_pair(make_pair(i, j), false));
			index++;
		}
	}
}*/



int Ship::getScoreForSinking() const
{
	switch (type)
	{
	case (ship_type::A_BOAT):
	case (ship_type::B_BOAT):
		return BOAT_SCORE;
	case (ship_type::A_SATIL):
	case(ship_type::B_SATIL):
		return SATIL_SCORE;
	case (ship_type::A_SUBMARINE):
	case(ship_type::B_SUBMARINE):
		return SUBMARINE_SCORE;
	case (ship_type::A_DESTROYER):
	case(ship_type::B_DESTROYER):
		return DESTROYER_SCORE;
	default:
		return BOAT_SCORE;
	}
}

void Ship::hitAt(int row, int col)
{
	for (vector<pair<pair<int, int>, bool>>::iterator it = coordinates->begin(); it != coordinates->end(); ++it)
	{
		if (make_pair(row, col) == it->first)
		{
			it->second = true;
		}
	}
}

bool Ship::isSunk() const
{
	for (vector<pair<pair<int, int>, bool>>::iterator it = coordinates->begin(); it != coordinates->end(); ++it)
	{
		if (!it->second) //second is a bool which means "is Hit"
			return false;
	}
	return true;

}
bool Ship::containsCoord(int row, int col) const
{
	bool contains = false;
	for (vector<pair<pair<int, int>, bool>>::iterator it = coordinates->begin(); it != coordinates->end(); ++it)
	{
		if (it->first == make_pair(row, col))
		{
			contains = true;
			break;
		}
	}
	return contains;
}

int Ship::getRow(vector<pair<pair<int, int>, bool>>::const_reference pair)
{
	return pair.first.first;
}
int  Ship::getCol(vector<pair<pair<int, int>, bool>>::const_reference pair)
{
	return pair.first.second;
}

Ship::ship_type Ship::getType()
{
	return type;
}

bool Ship::isAShip()
{
	if (type == ship_type::A_BOAT || type == ship_type::A_SATIL || 
		type == ship_type::A_SUBMARINE || type == ship_type::A_DESTROYER)
	{
		return true;
	}
	return false;
}
bool Ship::isBShip()
{
	if (type == ship_type::B_BOAT || type == ship_type::B_SATIL ||
		type == ship_type::B_SUBMARINE || type == ship_type::B_DESTROYER)
	{
		return true;
	}
	return false;
}

int Ship::getSizeOfShipType(Ship::ship_type type)
{
	int size;
	switch (type)
	{
	case (ship_type::A_BOAT):
	case (ship_type::B_BOAT):
		size = BOAT_SIZE;
		break;
	case (ship_type::A_SATIL):
	case(ship_type::B_SATIL):
		size = SATIL_SIZE;
		break;
	case (ship_type::A_SUBMARINE):
	case(ship_type::B_SUBMARINE):
		size = SUBMARINE_SIZE;
		break;
	case (ship_type::A_DESTROYER):
	case(ship_type::B_DESTROYER):
		size = DESTROYER_SIZE;
		break;
	}
	return size;
}


bool Ship::isValidShip(vector<pair<pair<int, int>, bool>>* coordinates, ship_type type)
{
	int cur_row, cur_col;
	bool dif_row = false, dif_col = false;
	int size = coordinates->size();
	
	int row = Ship::getRow((*coordinates)[0]);
	int col = Ship::getCol((*coordinates)[0]);
	for(int i = 0; i < size; i++)
	{
		cur_row = Ship::getRow((*coordinates)[i]);
		cur_col = Ship::getCol((*coordinates)[i]);
		if (cur_row != row)
			dif_row = true;
		if (cur_col != col)
			dif_col = true;
	}
	if ((dif_row && dif_col) || size > 4 || getSizeOfShipType(type) != size)
		return false;
	return true;
}

vector<pair<pair<int, int>, bool>> * Ship::getCoordinates()
{
	return coordinates;
}

Ship::Ship(ship_type type, vector<pair<int, int>> *coordinates_only) :type(type), coordinates(nullptr)
{	
	this->size = coordinates_only->size();
	coordinates = new vector<pair<pair<int, int>, bool>>;
	for (int i = 0; i < size; i++)
	{
		int row = (*coordinates_only)[i].first;
		int col = (*coordinates_only)[i].second;
		coordinates->push_back(make_pair(make_pair(row,col), false));
	}
	this->valid = isValidShip(coordinates, type);
}

bool Ship::getValid()
{
	return valid;
}

bool Ship::isAdjacentCoordinates(pair<int, int> a, pair<int, int> b)
{
	int a_x = a.first;
	int a_y = a.second;
	int b_x = b.first;
	int b_y = b.second;

	if (a_x == b_x && a_y == b_y - 1)
		return true;
	if (a_x == b_x && a_y == b_y + 1)
		return true;
	if (a_y == b_y && a_x == b_x - 1)
		return true;
	if (a_y == b_y && a_x == b_x + 1)
		return true;	
	return false;
}

bool Ship::isAdjacentShips(Ship other_ship)
{
	for (vector<pair<pair<int, int>, bool>>::iterator it = coordinates->begin(); it != coordinates->end(); ++it)
		for (vector<pair<pair<int, int>, bool>>::iterator it2 = other_ship.coordinates->begin(); it2 != other_ship.coordinates->end(); ++it2)
			if (Ship::isAdjacentCoordinates(it->first, it2->first))
				return true;
	return false;
}



Ship::~Ship()
{
	DEBUG("dtor is called for a ship of type " << static_cast<char>(type));
	delete coordinates;
}
