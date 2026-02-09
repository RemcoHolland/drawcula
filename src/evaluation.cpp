#include "evaluation.h"
#include "utils.h"

constexpr int SWITCH_TO_ENDGAME_MATERIAL = PAWN_VALUE * (NR_PAWNS / 2)
							   + KNIGHT_VALUE * (NR_KNIGHTS / 2)
							   + BISHOP_VALUE * (NR_BISHOPS / 2)
							   + ROOK_VALUE * (NR_ROOKS / 2)
							   + QUEEN_VALUE * (NR_QUEENS / 2);

// Total material at the start of the game is 7860
int total_material_value = PAWN_VALUE * NR_PAWNS
						 + KNIGHT_VALUE * NR_KNIGHTS
						 + BISHOP_VALUE * NR_BISHOPS
						 + ROOK_VALUE * NR_ROOKS
						 + QUEEN_VALUE * NR_QUEENS;

bool endgame = false;

void evaluation::initPieceSquareTable()
{
	for (int color = 0; color < COLORS; color++) {
		for (int piece = 0; piece < PIECES; piece++) {
			for (int square = 0; square < SQUARES; square++) {
				if (color == WHITE) {
					PIECE_SQUARE[color][piece][square] =
						piece == PAWN ? PAWN_PSQT[FLIP[square]] :
						piece == KNIGHT ? KNIGHT_PSQT[FLIP[square]] :
						piece == BISHOP ? BISHOP_PSQT[FLIP[square]] :
						piece == ROOK ? ROOK_PSQT[FLIP[square]] :
						piece == QUEEN ? QUEEN_PSQT[FLIP[square]] :
						KING_MG_PSQT[FLIP[square]];
				} else {
					PIECE_SQUARE[color][piece][square] =
						piece == PAWN ? PAWN_PSQT[square] :
						piece == KNIGHT ? KNIGHT_PSQT[square] :
						piece == BISHOP ? BISHOP_PSQT[square] :
						piece == ROOK ? ROOK_PSQT[square] :
						piece == QUEEN ? QUEEN_PSQT[square] :
						KING_MG_PSQT[square];
				}
			}
		}
	}
}

int evaluation::positionalScore(const U64(&piece_list)[COLORS][PIECES]) {
	int score = 0;
	for (int color = WHITE; color < COLORS; color++) {
		for (int piece = PAWN; piece < PIECES; piece++) {
			U64 pieces = piece_list[color][piece];
			while (pieces) {
				const int square = Utils::getLS1B(pieces);
				score += (-color | 1) * PIECE_SQUARE[color][piece][square];
				pieces &= pieces - 1; // clear LSB
			}
		}
	}
	return score;
}

void evaluation::useKingEndgamePSQT() {
   for (int color = 0; color < COLORS; color++) {
	   	  for (int square = 0; square < SQUARES; square++) {
		 if (color == WHITE) {
			PIECE_SQUARE[color][KING][square] = KING_EG_PSQT[FLIP[square]];
		 } else {
			PIECE_SQUARE[color][KING][square] = KING_EG_PSQT[square];
		 }
	  }
   }
}

void evaluation::useKingMiddlegamePSQT() {
	for (int color = 0; color < COLORS; color++) {
		for (int square = 0; square < SQUARES; square++) {
			if (color == WHITE) {
				PIECE_SQUARE[color][KING][square] = KING_MG_PSQT[FLIP[square]];
			} else {
				PIECE_SQUARE[color][KING][square] = KING_MG_PSQT[square];
			}
		}
	}
}

void evaluation::increaseMaterial(const int value) {
	total_material_value += value;
}

void evaluation::decreaseMaterial(const int value) {
	total_material_value -= value;
}

void evaluation::determineGamePhase() {
	const bool shouldBeEndgame = total_material_value <= SWITCH_TO_ENDGAME_MATERIAL;
	if (shouldBeEndgame == endgame) return;

	if (shouldBeEndgame) {
		useKingEndgamePSQT();
	} else {
		useKingMiddlegamePSQT();
	}
	endgame = shouldBeEndgame;
}
