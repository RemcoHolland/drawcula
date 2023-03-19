#include "magicmoves.h"
#include "uci.h"
#include "zobrist.h"

int main() {
	initmagicmoves();
	zobrist::init();
	Uci uci = Uci();
	uci.loop();
}