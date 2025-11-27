#include "magicmoves.h"
#include "uci.h"
#include "zobrist.h"

int main() {
	initmagicmoves();
	zobrist::init();
	auto uci = Uci();
	uci.loop();
}