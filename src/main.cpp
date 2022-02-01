#include "main.h"

int main() {
	initmagicmoves();
	Uci uci = Uci();
	uci.loop();
}