#include <sstream>
#include <algorithm>
#include "reader/epdreader.h"
#include "reader/readutils.h"
#include "stringutils.h"

EpdReader::EpdReader() = default;

Epd EpdReader::read(const string& epd) {
	const int pos = StringUtils::nthOccurrence(epd, " ", 4);
	const string epd_part1 = epd.substr(0, pos);
	const string epd_part2 = epd.substr(pos + 1, epd.length());
	// split epd
	const std::vector<string> splitted_epd_part1 = StringUtils::split(epd_part1, ' ');
	if (splitted_epd_part1.size() != 4) {
		throw std::invalid_argument("epd is invalid");
	}
	Position position = ReadUtils::read(splitted_epd_part1);

	// get operation as key values and put in operation map
	const std::vector<string> splitted_epd_part2 = StringUtils::split(epd_part2, ';');
	std::map<string, string> operation;

	for (int i = 0; i < splitted_epd_part2.size(); i++) {
		std::vector<string> splits = StringUtils::split(splitted_epd_part2[i], ' ');
		operation.insert({ splits[0], splits[1] });
	}
	return Epd(position.piece_list, position.color, position.castling_rights, position.enpassant_square, operation);
}

EpdReader::~EpdReader() = default;