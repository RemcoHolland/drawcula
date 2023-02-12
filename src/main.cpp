#include "magicmoves.h"
#include "uci.h"

int main() {
	initmagicmoves();
	Uci uci = Uci();
	uci.loop();
}