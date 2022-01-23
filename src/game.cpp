#include "game.h"

Game::Game() {
}

void Game::setPosition(FenInfo position) {
	(&board)->~Board();
	new (&board) Board(position);
	color = position.getColor();
	half_moves = position.getHalfMoves();
	full_moves = position.getFullMoves();
}

void Game::doMove(Move move)
{
	Movelist movelist = Movelist();
	movelist.generateMoves(color, board);
	
	//NB pseudo-legal move
	Move legalMove = getLegalMove(movelist, move);
	board.makeMove(color, legalMove);
	changeColor();
}

void Game::play() {

	//	color = position.getColor();
	//	half_moves = position.getHalfMoves();
	//	full_moves = position.getFullMoves();

	//	Board board = Board(position);
	bool illegalMove = false;

	while (!endOfGame()) {
		Console::printBoard(color, board, illegalMove);
		Console::printNodes(nodes, searchTime);

		Movelist movelist = Movelist();
		movelist.generateMoves(color, board);

		string input = Console::getUserInput(color, board);

		Move move;
		if (input == "e") {
			illegalMove = false;
			move = getEngineMove(color, board);
		}
		else {
			//move = Move(input);

			if (!isLegalMove(movelist, move)) {
				illegalMove = true;
				continue;
			}
			nodes = 0;
			move = getLegalMove(movelist, move);
		}
		MoveInfo moveInfo = board.makeMove(color, move);
		if (Square::isAttacked(color, board, board.getPiece(color == WHITE ? WHITE_KING : BLACK_KING))) {
			board.unmakeMove(color, move, moveInfo);
			illegalMove = true;
			continue;
		}
		illegalMove = false;
		changeColor();
	}
}

void Game::changeColor() {
	color ^= 1;
}

Move Game::getEngineMove(int color, Board board) {
	Search search = Search();

	startClock();
	//search.start(color, board, 1000);
	stopClock();

	nodes = search.getNodes();
	return Move();
}

bool Game::isLegalMove(Movelist movelist, Move move) {
	bool legal = false;
	if (movelist.containsMove(move)) {
		legal = true;
	}
	return legal;
}

Move Game::getLegalMove(Movelist movelist, Move user_move) {
	std::list<Move> moves = movelist.getMoves();
	for (std::list<Move>::iterator it = moves.begin(); it != moves.end(); ++it) {
		Move move = *it;
		if (move == user_move) {
			return move;
		}
	}
	return user_move;
}

bool Game::endOfGame() {
	return false;
}

void Game::startClock() {
	startTime = std::clock();
}

void Game::stopClock() {
	searchTime = (std::clock() - startTime) / (double)CLOCKS_PER_SEC;
}

Game::~Game() {
}


