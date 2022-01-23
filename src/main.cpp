#include "../include/main.h"

int main() {
	initmagicmoves();
	Uci uci = Uci();
	uci.loop();
	//Game game = Game();
	//game.play();
}