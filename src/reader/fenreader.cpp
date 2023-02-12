#include <sstream>
#include "reader/fenreader.h"
#include "reader/readutils.h"
#include "stringutils.h"

FenReader::FenReader() {
}

Fen FenReader::read(const string& fen) {
	int pos = StringUtils::nthOccurrence(fen, " ", 4);
	string fen_part1 = fen.substr(0, pos);
	string fen_part2 = fen.substr(pos + 1, fen.length());
	// split FEN
	std::vector<string> splitted_fen_part1 = StringUtils::split(fen_part1, ' ');
	if (splitted_fen_part1.size() != 4) {
		throw std::invalid_argument("fen is invalid");
	}
	Position position = ReadUtils::read(splitted_fen_part1);

	std::vector<string> splitted_fen_part2 = StringUtils::split(fen_part2, ' ');
	if (splitted_fen_part2.size() != 2) {
		throw std::invalid_argument("fen is invalid");
	}
	int half_moves = std::stoi(splitted_fen_part2.at(0));
	int full_moves = std::stoi(splitted_fen_part2.at(1));

	return Fen(position.piece_list, position.color, position.castling_rights, position.enpassant_square, half_moves, full_moves);
}

FenReader::~FenReader() {
}
