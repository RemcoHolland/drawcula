#include <sstream>
#include "reader/readutils.h"
#include "stringutils.h"
#include "castling.h"

Position ReadUtils::read(const std::vector<string>& splitted_fen) {
	// position
	U64 piece_list[COLORS][PIECES] = { {} };
	const string& positionStr = splitted_fen.at(0);
	fillPieceList(positionStr, piece_list);

	// color
	const int color = getColor(splitted_fen.at(1)[0]);

	// castling
	int castling_rights = 0;
	const string& castlingStr = splitted_fen.at(2);
	if (castlingStr != "-") {
		for (int i = 0; i < castlingStr.length(); i++) {
			castling_rights |= getCastlingRight(castlingStr[i]);
		}
	}

	// en passant
	U64 enpassant_square = 0;
	const string& enpassantStr = splitted_fen.at(3);
	if (enpassantStr != "-") {
		enpassant_square = getEnpassantSquare(enpassantStr[0], enpassantStr[1]);
	}

	return Position(piece_list, color, castling_rights, enpassant_square);
}

void ReadUtils::fillPieceList(const string& position, U64(&piece_list)[COLORS][PIECES]) {
	int rank = 7;
	int file = 0;

	for (int i = 0; i < position.length(); ++i) {

		if (position[i] == '/') {
			rank--;
			file = 0;
		} else if (position[i] >= '1' && position[i] <= '8') {
			file += getNumber(position[i]); // convert to int
		} else {
			const int color = getPieceColor(position[i]);
			const int piece = getPiece(position[i]);
			piece_list[color][piece] += static_cast<U64>(1) << (rank * RANKS + file);
			file++;
		}
	}
}

constexpr int ReadUtils::getColor(const char color) {
	return color == 'w' ? WHITE : BLACK;
}

constexpr int ReadUtils::getPieceColor(const char piece) {
	if (std::isupper(piece)) {
		return WHITE;
	}
	return BLACK;
}

constexpr int ReadUtils::getPiece(const char piece) {
	switch (piece) {
	case 'P':
	case 'p': return PAWN;
	case 'N':
	case 'n': return KNIGHT;
	case 'B':
	case 'b': return BISHOP;
	case 'R':
	case 'r': return ROOK;
	case 'Q':
	case 'q': return QUEEN;
	case 'K':
	case 'k': return KING;
	default: throw std::invalid_argument("piece is invalid");
	}
}

constexpr int ReadUtils::getCastlingRight(const char castling_char) {
	switch (castling_char) {
	case 'K': return WHITE_KING_SIDE;
	case 'Q': return WHITE_QUEEN_SIDE;
	case 'k': return BLACK_KING_SIDE;
	case 'q': return BLACK_QUEEN_SIDE;
	default: throw std::invalid_argument("castling char is invalid");
	}
}

constexpr U64 ReadUtils::getEnpassantSquare(const char file, const char rank) {
	return static_cast<U64>(1) << StringUtils::getSquare(file, rank);
}

constexpr int ReadUtils::getNumber(const char number) {
	return number - 48;
}