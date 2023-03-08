#pragma once
#include <map>
#include <string>
#include "position/position.h"
#include "piece.h"
#include "color.h"

using std::string;

class Epd : public Position {
public:
	Epd(U64(&)[COLORS][PIECES], int, int, U64, std::map<string, string>);
	std::map<string, string> operation;
	~Epd();
};