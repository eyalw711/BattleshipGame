#include "stdafx.h"
#include "Ship.h"


Ship::Ship()
{
	
}
Ship::Ship(ship_type type, pair<int, int> start, pair<int, int> end) : type(type), start(start), end(end)
{
	// find size of ship
	switch (type)
	{
	case (ship_type::A_BOAT):
	case (ship_type::B_BOATB):
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
			coordinates[index] = { i, j };
			index++;
		}
	}
}

Ship::~Ship()
{
}
