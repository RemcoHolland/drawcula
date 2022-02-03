#pragma once
#include <cstdint>
#include <string>
#include "stringutils.h"
#include "flag.h"
#include "utils.h"
#include "piece.h"

using std::string;
using StringUtils::getFile;
using StringUtils::getRank;
using StringUtils::getSquare;

class Move {
public:
	Move(string, int);
	Move(uint64_t, uint64_t);
	Move(int, uint64_t, uint64_t, Flag);
	Move(int, uint64_t, uint64_t, Flag, int);
	int piece;
	uint64_t from;
	uint64_t to;
	Flag flag;
	int promotion;
	bool operator==(const Move& other);
	string toString();
	~Move();

private:

};

