#include "position/epd.h"

Epd::Epd(U64(&piece_list)[COLORS][PIECES], int color, int castling_rights, U64 enpassant_square, const std::map<string, string>& operation)
	: Position(piece_list, color, castling_rights, enpassant_square), operation(operation) {}

Epd::~Epd() = default;