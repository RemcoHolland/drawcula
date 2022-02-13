#pragma once

#include "board.h"
#include "movegen.h"
#include "utils.h"
#include "stringutils.h"

#include <iostream>
#include <chrono>

using StringUtils::getFile;
using StringUtils::getRank;

class Perft {
public:
	Perft(bool, int);
	uint64_t calculate(int, Board);
	~Perft();

private:
	bool divide = false;
	uint64_t nodes = 0;
	uint64_t root_nodes = 0;
	int search_depth = 0;
	void negaMax(int, Board, int);
	void printRootNodes(int);
};
