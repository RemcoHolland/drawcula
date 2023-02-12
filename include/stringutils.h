#pragma once
#include <vector>
#include <string>

using std::string;

namespace StringUtils {
	int getSquare(char, char);
	char getRank(int);
	char getFile(int);

	string moveToStringAN(int);
	string moveToStringLAN(int);

	//what does this template do?
	template <typename Out>
	void split(const std::string&, char, Out);
	std::vector<string> split(const string&, char);
	int nthOccurrence(const string&, const string&, int);
}