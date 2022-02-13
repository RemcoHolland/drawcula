#include "moveinfo.h"

MoveInfo::MoveInfo(int captured_piece, int enpassant_square, int castling_rights) {
	MoveInfo::captured_piece = captured_piece;
	MoveInfo::enpassant_square = enpassant_square;
	MoveInfo::castling_rights = castling_rights;
}

MoveInfo::~MoveInfo() {
}
