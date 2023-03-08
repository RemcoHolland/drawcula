#pragma once
#include <string>
#include <chrono>
#include <limits>
#include <vector>
#include <atomic>
#include "board.h"

extern std::atomic<bool> g_stop;
using std::string;
typedef uint64_t U64;

struct Params {
	int max_depth = std::numeric_limits<int>::max();
	std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
	std::chrono::milliseconds movetime{ 0 };
	std::chrono::milliseconds wtime{ 0 };
	std::chrono::milliseconds btime{ 0 };
	std::chrono::milliseconds winc{ 0 };
	std::chrono::milliseconds binc{ 0 };
};

constexpr int MAX_INT = std::numeric_limits<int>::max();
constexpr int MATE{ 100000 };
constexpr int STALE_MATE{ 0 };
constexpr int EXPECTED_NR_MOVES = 40;

class Search {
public:
	Search();
	U64 getNodes();
	int start(int, Board, Params);
	~Search();

private:	
	Params params;
	U64 nodes{ 0 };

	int alphaBeta(int, int, int, int, Board&, std::vector<int>&);
	bool timeToMove(int);
	void updatePV(std::vector<int>&, const std::vector<int>&, int);
	string printPV(const std::vector<int>&);
};

