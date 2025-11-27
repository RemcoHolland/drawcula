#pragma once
#include "board.h"

class Perft {
public:
	Perft(bool, int);
	U64 calculate(int, Board);
	~Perft();

private:
	bool divide = false;
	U64 nodes = 0;
	U64 root_nodes = 0;
	int search_depth = 0;
	void negaMax(int, Board&, int);
	void printRootNodes(int) const;
};
