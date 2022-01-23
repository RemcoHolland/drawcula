#pragma once
#include "game.h"
#include "stringutils.h"
#include "perft.h"
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
	const string startpos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	Game game = Game();
	void uci();
	void ucinewgame();
	void isready();
	void position(string);
	void go(string);
	void perft(bool, string);
	void stop();
	bool startsWith(string, string);
	bool contains(string, string);
};