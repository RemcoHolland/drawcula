#pragma once
#include "stringutils.h"
#include "perft.h"
#include "movelist.h"
#include "fenreader.h"
#include "search.h"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <future>
#include <limits>

using std::string;
using StringUtils::split;

class Uci {
public:
	Uci();	
	void loop();
	void position(const std::vector<std::string>&);
	void go(const std::vector<std::string>&);
	~Uci();

private:
	int color = WHITE;
	int half_moves = 0;
	int full_moves = 1;
	const string startpos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	Board board = Board(FenReader::read(startpos));
	void uci();
	void ucinewgame();
	void isready();
	void perft(bool, const std::vector<std::string>&);
	void stop();
	void changeColor();
	void resetBoard(const Position&);
	int stringToMove(int, const string&);
};