#pragma once

#include "piece.h"
#include "color.h"

class ConsoleUtils {
public:
	ConsoleUtils();
	static char getPromotion(int);
	static int getPromotion(char, int);
	static int getSquare(char, char);
	static char getRank(int);
	static char getFile(int);
	~ConsoleUtils();
};

