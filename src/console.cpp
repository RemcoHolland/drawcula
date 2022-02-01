#include "console.h"

const string line = "-------------------------------------------------";
const string split = "|     |     |     |     |     |     |     |     |";
const int upper_case = 90;

Console::Console() {
}

Console::~Console() {
}

void Console::printBoard(int color, Board board, bool illegalMove) {
	system("cls");

	string turn = color == WHITE ? "WHITE'S TURN" : "BLACKS TURN";

	std::cout << turn << std::endl << std::endl;
	
	for (int i = 0; i < Board::RANKS; i++) {
		std::cout << line;
		if (i == 4 && illegalMove == true)
			std::cout << "    **  ILLEGAL MOVE  **";
		std::cout << std::endl << split << std::endl;

		for (int j = (Board::SQUARES - Board::FILES - i * Board::RANKS); j < (Board::SQUARES - i * Board::RANKS); j++) {

			char piece = getPieceChar(Utils::getPower(j), board);			
		}
		std::cout << "|" << std::endl << split << std::endl;
	}
	std::cout << line << std::endl;
}

void Console::printNodes(uint64_t nodes, double searchTime) {
	if (nodes != 0) {
		std::cout << nodes << "\a nodes    " << searchTime << " sec      "
			<< (uint64_t)(nodes / (searchTime)) << " nodes/sec" << std::endl << std::endl;
	}
}

std::string Console::getUserInput(int color, Board board) {

	string input;
	do {
		std::cout << "Enter next move (for example e2e4), 'perft x', 'divide x' or 'e' for engine move" << std::endl;
		std::getline(std::cin, input, '\n');

		std::regex perft("(perft)\\s[0-9]");
		std::regex divide("(divide)\\s[0-9]");

		if (std::regex_match(input, perft)) {
			calculatePerft(false, color, board, input);
			continue;
		} else if (std::regex_match(input, divide)) {
			calculatePerft(true, color, board, input);
			continue;
		}
	} while (!isValidInput(input));

	return input;
}

char Console::getPieceChar(uint64_t square, Board board) {
	char piece = ' ';
	if ((board.getPiece(WHITE_PAWN) & square) == square)
		piece = 'P';
	else if ((board.getPiece(WHITE_KNIGHT) & square) == square)
		piece = 'N';
	else if ((board.getPiece(WHITE_BISHOP) & square) == square)
		piece = 'B';
	else if ((board.getPiece(WHITE_ROOK) & square) == square)
		piece = 'R';
	else if ((board.getPiece(WHITE_QUEEN) & square) == square)
		piece = 'Q';
	else if ((board.getPiece(WHITE_KING) & square) == square)
		piece = 'K';
	else if ((board.getPiece(BLACK_PAWN) & square) == square)
		piece = 'p';
	else if ((board.getPiece(BLACK_KNIGHT) & square) == square)
		piece = 'n';
	else if ((board.getPiece(BLACK_BISHOP) & square) == square)
		piece = 'b';
	else if ((board.getPiece(BLACK_ROOK) & square) == square)
		piece = 'r';
	else if ((board.getPiece(BLACK_QUEEN) & square) == square)
		piece = 'q';
	else if ((board.getPiece(BLACK_KING) & square) == square)
		piece = 'k';
	return piece;
}

bool Console::isValidInput(string input) {
	bool valid = false;
	std::regex move("[a-h][1-8][a-h][1-8]");	
	if (std::regex_match(input, move) || input == "e") {
		valid = true;
	}
	return valid;
}

void Console::calculatePerft(bool divide,  int color, Board board, string input) {
	int depth = input.back() - 48;
	Perft perft = Perft(divide, depth);
	uint64_t nodes = perft.calculate(color, board);
	std::cout << nodes << std::endl;
}

