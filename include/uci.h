#pragma once
#include <string>
#include <vector>
#include "board.h"
#include "reader/fenreader.h"

using std::string;

class Uci {
public:
	Uci();	
	void loop();
	void position(const std::vector<string>&);
	void go(const std::vector<string>&);
	static void stop();
	~Uci();

private:
	int color = WHITE;
	int half_moves = 0;
	int full_moves = 1;
	const string startpos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	Board board = Board(FenReader::read(startpos));
	static void uci();
	void ucinewgame();
	static void isready();
	void perft(bool, const std::vector<std::string>&);	
	void changeColor();
	void resetBoard(const Fen&);
	static int stringToMove(const string&);
};