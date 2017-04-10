#pragma once
#include <utility>
#include <vector>


using namespace std;
class Ship
{
public:
	enum class ship_type : char { A_BOAT = 'B', A_SATIL = 'P' , A_SUBMARINE = 'M', A_DESTROYER = 'D',
		B_BOAT = 'b', B_SATIL = 'p', B_SUBMARINE = 'm', B_DESTROYER = 'd'};
	static const int BOAT_SIZE = 1;
	static const int SATIL_SIZE = 2;
	static const int SUBMARINE_SIZE = 3;
	static const int DESTROYER_SIZE = 4;
	//Ship(ship_type type, pair<int, int> start, pair<int, int> end);
	static bool isValidShip(vector<pair<pair<int, int>, bool>>* coordinates, ship_type type);
	static int getSizeOfShipType(Ship::ship_type type);
	vector<pair<pair<int, int>, bool>> * Ship::getCoordinates();
	static bool isAdjacentCoordinates(pair<int, int> a, pair<int, int> b);
	bool isAdjacentShips(Ship other_ship);
	Ship(ship_type type, vector<pair<int, int>> *coordinates_only);
	Ship();
	static int getRow(vector<pair<pair<int, int>, bool>>::const_reference pair);
	static int getCol(vector<pair<pair<int, int>, bool>>::const_reference pair);
	Ship::ship_type getType();
	bool getValid();
	~Ship();
private:
	ship_type type;
	int size;
	//pair<int, int> start;
	//pair<int, int> end;
	vector<pair<pair<int, int>,bool>> *coordinates; // bool is "isHit" (false if not hit)
	bool valid; //true is ship is of valid shape
};

