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
	Move(const string&, int);
	Move(int, int);
	Move(int, int, int, Flag);
	Move(int, int, int, Flag, int);
	int piece;
	int from;
	int to;
	Flag flag;
	int promotion;
	bool operator==(const Move& other);
	string toString();
	~Move();

private:

};

