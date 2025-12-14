#include "evaluation.h"
#include "magicmoves.h"
#include "uci.h"
#include "zobrist.h"

int main() {
	initmagicmoves();
	zobrist::init();
	evaluation::initPieceSquareTable();
	auto uci = Uci();
	uci.loop();
}