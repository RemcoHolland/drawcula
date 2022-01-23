#pragma once

#include <string>
#include <list>
#include <algorithm>
#include "board.h"
#include "console.h"
#include "movelist.h"
#include "search.h"
#include "color.h"
#include "fenreader.h"
#include "feninfo.h"

class Game {

public:
	int color = WHITE;
	Board board = Board(startpos);

	Game();
	void setPosition(FenInfo fen);
	void doMove(Move move);
	void play();
	~Game();

private:
	const FenInfo startpos = FenReader::read("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

	uint64_t nodes = 0;
	double searchTime = 0;
	clock_t startTime = 0;
	int half_moves = 0;
	int full_moves = 0;

	void changeColor();
	Move getEngineMove(int, Board);
	bool isLegalMove(Movelist, Move);
	Move getLegalMove(Movelist, Move);
	bool endOfGame();
	void startClock();
	void stopClock();
};

