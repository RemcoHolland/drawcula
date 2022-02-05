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
using StringUtils::trim;

class Uci {
public:
	Uci();
	void loop();
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
	void position(string);
	void go(string);
	void perft(bool, string);
	void stop();
	bool startsWith(string, string);
	bool contains(string, string);
	void changeColor();
	void resetBoard(const FenInfo&);
};