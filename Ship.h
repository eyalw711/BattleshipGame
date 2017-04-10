#pragma once
#include <utility>
#include <vector>


using namespace std;
class Ship
{
public:
	enum class ship_type { A_BOAT = 'B', A_SATIL = 'P' , A_SUBMARINE = 'M', A_DESTROYER = 'D',
		B_BOATB = 'b', B_SATIL = 'p', B_SUBMARINE = 'm', B_DESTROYER = 'd'};
	static const int BOAT_SIZE = 1;
	static const int SATIL_SIZE = 2;
	static const int SUBMARINE_SIZE = 3;
	static const int DESTROYER_SIZE = 4;
	Ship(ship_type type, pair<int, int> start, pair<int, int> end);
	Ship();
	~Ship();
private:
	ship_type type;
	int size;
	pair<int, int> start;
	pair<int, int> end;
	vector<pair<pair<int, int>,bool>> coordinates; // bool is "isHit"
	//pair<int, int> coordinates[4] = {{0,0}};
	bool hit[4] = {false};
};

