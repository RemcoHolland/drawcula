#include <list>
#include <iostream>
#include <chrono>
#include <vector>
#include <cmath>
#include <limits>
#include "move.h"
#include "evaluation.h"
#include "color.h"
#include "movelist.h"
#include "square.h"
#include "moveinfo.h"
#include "atomic"

#pragma once

extern std::atomic<bool> g_stop;

struct Time {
	std::chrono::steady_clock::time_point startTime;
	std::chrono::milliseconds movetime{ 0 };
	std::chrono::milliseconds wtime{ 0 };
	std::chrono::milliseconds btime{ 0 };
	std::chrono::milliseconds winc{ 0 };
	std::chrono::milliseconds binc{ 0 };
};

class Search {
public:
	Search();
	uint64_t getNodes();
	void start(int, int, Board&, const Time&);
	~Search();

private:
	const int MATE{ 100000 };
	const int STALE_MATE{ 0 };
	const int EXPECTED_NR_MOVES = 40;
	Time time;
	uint64_t nodes{ 0 };

	int negaMax(int, int, Board&, std::vector<Move>&);
	bool timeToMove(int);
	void updatePV(std::vector<Move>&, std::vector<Move>, Move);
	string printPV(std::vector<Move>);
};

