#include "fenreader.h"

FenReader::FenReader() {
}

Position FenReader::read(string fen) {
	// split FEN
	std::vector<string> splitted_fen = split(fen, ' ');
	if (splitted_fen.size() != 6) {
		throw std::invalid_argument("fen is invalid");
	}

	// position
	U64 piece_list[COLORS][PIECES] = { { 0 } };
	string positionStr = splitted_fen.at(0);
	fillPieceList(positionStr, piece_list);

	// color
	int color = getColor(splitted_fen.at(1)[0]);

	// castling
	int castling_rights = 0;
	string castlingStr = splitted_fen.at(2);
	if (castlingStr != "-") {
		for (int i = 0; i < castlingStr.length(); i++) {
			castling_rights |= getCastlingRight(castlingStr[i]);
		}
	}

	// en passant
	U64 enpassant_square = 0;
	string enpassantStr = splitted_fen.at(3);
	if (enpassantStr != "-") {
		enpassant_square = getEnpassantSquare(enpassantStr[0], enpassantStr[1]);
	}

	//half moves
	int half_moves = std::stoi(splitted_fen.at(4));

	// full moves
	int full_moves = std::stoi(splitted_fen.at(5));

	return Position(piece_list, color, castling_rights, enpassant_square, half_moves, full_moves);
}

FenReader::~FenReader() {
}

void FenReader::fillPieceList(string position, U64(&piece_list)[COLORS][PIECES]) {
	int rank = 7;
	int file = 0;

	for (int i = 0; i < position.length(); ++i) {

		if (position[i] == '/') {
			rank--;
			file = 0;
		} else if (position[i] >= '1' && position[i] <= '8') {
			file += getNumber((position[i])); // convert to int
		} else {
			int color = getPieceColor(position[i]);
			int piece = getPiece(position[i]);
			piece_list[color][piece] += (U64)1 << (rank * RANKS + file);
			file++;
		}
	}
}

int FenReader::getColor(char color) {
	return color == 'w' ? WHITE : BLACK;
}

int FenReader::getPieceColor(char piece) {
	if (std::isupper(piece)) {
		return WHITE;
	}
	return BLACK;
}

int FenReader::getPiece(char piece_char) {
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

int FenReader::getCastlingRight(char castling_char) {
	switch (castling_char) {
	case 'K': return WHITE_KING_SIDE;
	case 'Q': return WHITE_QUEEN_SIDE;
	case 'k': return BLACK_KING_SIDE;
	case 'q': return BLACK_QUEEN_SIDE;
	default: throw std::invalid_argument("castling char is invalid");
	}
}

U64 FenReader::getEnpassantSquare(char file, char rank) {
	return (U64)1 << StringUtils::getSquare(file, rank);
}

int FenReader::getNumber(char number) {
	return number - 48;
}
