#include "position/epd.h"
#include <iterator>

Epd::Epd(U64(&piece_list)[COLORS][PIECES], int color, int castling_rights, U64 enpassant_square, std::map<string, string> operation)
	: Position(piece_list, color, castling_rights, enpassant_square) {

	std::copy(operation.begin(), operation.end(), std::inserter(Epd::operation, Epd::operation.end()));
}

Epd::~Epd() {
}