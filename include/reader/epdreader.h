#pragma once
#include <string>
#include "position/epd.h"

using std::string;

class EpdReader {
public:
	EpdReader();
	static Epd read(const string&);
	~EpdReader();
};