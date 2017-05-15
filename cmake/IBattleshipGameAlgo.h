#pragma once

#include <utility> // for std::pair
#include <string>

enum class AttackResult {
	Miss, Hit, Sink
};

// IBattleshipGameAlgo for Ex2 - do not change this class - you should use it as is
class IBattleshipGameAlgo {
public:
	virtual ~IBattleshipGameAlgo() = default;
	virtual void setBoard(int player, const char** board, int numRows, int numCols) = 0;		// called once to notify player on his board
	virtual bool init(const std::string& path) = 0;		// called once to allow init from files if needed returns whether the init succeeded or failed
	virtual std::pair<int, int> attack() = 0;													// ask player for his move
	virtual void notifyOnAttackResult(int player, int row, int col, AttackResult result) = 0;	// notify on last move result
};

#ifdef ALGO_EXPORTS										// A flag defined in this project's Preprocessor's Definitions
#define ALGO_API extern "C" __declspec(dllexport)	// If we build - export
#else
#define ALGO_API extern "C" __declspec(dllimport)	// If someone else includes this .h - import
#endif

ALGO_API IBattleshipGameAlgo* GetAlgorithm();			// This method must be implemented in each player(algorithm) .cpp file