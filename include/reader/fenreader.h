#pragma once
#include <string>
#include "position/fen.h"

using std::string;

class FenReader {
public:
	FenReader();
	static Fen read(const string&);
	~FenReader();
};