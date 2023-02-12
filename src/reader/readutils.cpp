#include <sstream>
#include "reader/readutils.h"
#include "stringutils.h"
#include "castling.h"

Position ReadUtils::read(std::vector<string> splitted_fen) {	
	// position
	U64 piece_list[COLORS][PIECES] = { { 0 } };
	string positionStr = splitted_fen.at(0);
	ReadUtils::fillPieceList(positionStr, piece_list);

	// color
	int color = ReadUtils::getColor(splitted_fen.at(1)[0]);

	// castling
	int castling_rights = 0;
	string castlingStr = splitted_fen.at(2);
	if (castlingStr != "-") {
		for (int i = 0; i < castlingStr.length(); i++) {
			castling_rights |= ReadUtils::getCastlingRight(castlingStr[i]);
		}
	}

	// en passant
	U64 enpassant_square = 0;
	string enpassantStr = splitted_fen.at(3);
	if (enpassantStr != "-") {
		enpassant_square = ReadUtils::getEnpassantSquare(enpassantStr[0], enpassantStr[1]);
	}

	return Position(piece_list, color, castling_rights, enpassant_square);
}

void ReadUtils::fillPieceList(string position, U64(&piece_list)[COLORS][PIECES]) {
	int rank = 7;
	int file = 0;

	for (int i = 0; i < position.length(); ++i) {

		if (position[i] == '/') {
			rank--;
			file = 0;
		} else if (position[i] >= '1' && position[i] <= '8') {
			file += ReadUtils::getNumber((position[i])); // convert to int
		} else {
			int color = ReadUtils::getPieceColor(position[i]);
			int piece = ReadUtils::getPiece(position[i]);
			piece_list[color][piece] += (U64)1 << (rank * RANKS + file);
			file++;
		}
	}
}

int ReadUtils::getColor(char color) {
	return color == 'w' ? WHITE : BLACK;
}

int ReadUtils::getPieceColor(char piece) {
	if (std::isupper(piece)) {
		return WHITE;
	}
	return BLACK;
}

int ReadUtils::getPiece(char piece_char) {
	switch (piece_char) {
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

int ReadUtils::getCastlingRight(char castling_char) {
	switch (castling_char) {
	case 'K': return WHITE_KING_SIDE;
	case 'Q': return WHITE_QUEEN_SIDE;
	case 'k': return BLACK_KING_SIDE;
	case 'q': return BLACK_QUEEN_SIDE;
	default: throw std::invalid_argument("castling char is invalid");
	}
}

U64 ReadUtils::getEnpassantSquare(char file, char rank) {
	return (U64)1 << StringUtils::getSquare(file, rank);
}

int ReadUtils::getNumber(char number) {
	return number - 48;
}