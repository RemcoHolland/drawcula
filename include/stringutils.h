#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

using std::string;

namespace StringUtils {
	int getSquare(char, char);
	char getRank(int);
	char getFile(int);

	//what does this template do?
	template <typename Out>
	void split(const std::string&, char, Out);
	std::vector<std::string> split(const std::string&, char);
}

