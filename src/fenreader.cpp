#include "fenreader.h"

FenReader::FenReader() {
}

FenInfo FenReader::read(string fen) {
	// split FEN
	std::vector<string> splitted_fen = split(fen, ' ');
	if (splitted_fen.size() != 6) {
		throw std::invalid_argument("fen is invalid");
	}

	// position
	uint64_t piece_list[TOTAL_PIECES] = { 0 };
	string positionStr = splitted_fen.at(0);
	fillPieceList(positionStr, piece_list);

	// color
	int color = getColor(splitted_fen.at(1)[0]);

	// castling
	int castling_rights{ 0 };
	string castlingStr = splitted_fen.at(2);
	if (castlingStr != "-") {
		for (int i{ 0 }; i < castlingStr.length(); i++) {
			castling_rights |= getCastlingRight(castlingStr[i]);
		}
	}

	// en passant
	uint64_t enpassant_square{ 0 };
	string enpassantStr = splitted_fen.at(3);
	if (enpassantStr != "-") {
		enpassant_square = getEnpassantSquare(enpassantStr[0], enpassantStr[1]);
	}

	//half moves
	int half_moves = std::stoi(splitted_fen.at(4));

	// full moves
	int full_moves = std::stoi(splitted_fen.at(5));

	return FenInfo(piece_list, color, castling_rights, enpassant_square, half_moves, full_moves);
}

FenReader::~FenReader() {
}

void FenReader::fillPieceList(string position, uint64_t(&piece_list)[TOTAL_PIECES]) {
	int rank{ 7 };
	int file{ 0 };

	for (int i{ 0 }; i < position.length(); i++) {

		if (position[i] == '/') {
			rank--;
			file = 0;
		} else if (position[i] >= '1' && position[i] <= '8') {
			file += getNumber((position[i])); // convert to int
		} else {
			int piece = getPiece(position[i]);
			piece_list[piece] += Utils::getPower(rank * Board::RANKS + file);
			file++;
		}
	}
}

int FenReader::getColor(char color) {
	return color == 'w' ? WHITE : BLACK;
}

int FenReader::getPiece(char piece_char) {
	switch (piece_char) {
	case 'P': return WHITE_PAWN;
	case 'N': return WHITE_KNIGHT;
	case 'B': return WHITE_BISHOP;
	case 'R': return WHITE_ROOK;
	case 'Q': return WHITE_QUEEN;
	case 'K': return WHITE_KING;
	case 'p': return BLACK_PAWN;
	case 'n': return BLACK_KNIGHT;
	case 'b': return BLACK_BISHOP;
	case 'r': return BLACK_ROOK;
	case 'q': return BLACK_QUEEN;
	case 'k': return BLACK_KING;
	default: throw std::invalid_argument("piece is invalid");
	}
}

int FenReader::getCastlingRight(char castling_char) {
	switch (castling_char) {
	case 'K': return KING_SIDE;
	case 'Q': return QUEEN_SIDE;
	case 'k': return KING_SIDE << BLACK;
	case 'q': return QUEEN_SIDE << BLACK;
	default: throw std::invalid_argument("castling char is invalid");
	}
}

uint64_t FenReader::getEnpassantSquare(char file, char rank) {
	return Utils::getPower(StringUtils::getSquare(file, rank));
}

int FenReader::getNumber(char number) {
	return number - 48;
}
