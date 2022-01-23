#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

using std::string;

namespace StringUtils
{
	void trimRight(string&);
	void trimLeft(string&);
	void trim(string&);

	//what does this template do?
	template <typename Out>
	void split(const std::string&, char, Out);
	std::vector<std::string> split(const std::string&, char);
}

