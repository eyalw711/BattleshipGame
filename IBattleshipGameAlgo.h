#pragma once

#include <utility> // for std::pair

enum class AttackResult {
	Miss, Hit, Sink
};

class IBattleshipGameAlgo {
public:
	virtual ~IBattleshipGameAlgo() = default;
	virtual void setBoard(const char** board, int numRows, int numCols) = 0; // called once to notify player on his board
	virtual std::pair<int, int> attack() = 0; // ask player for his move
	virtual void notifyOnAttackResult(int player, int row, int col, AttackResult result) = 0; // notify on last move result
};