#include "search.h"

Search::Search() {
}

uint64_t Search::getNodes() {
	return nodes;
}

void Search::start(int color, int max_depth, Board board, const Time time) {
	int depth{ 1 };
	this->time = time;
	std::vector<int> currentPV;
	std::vector<int> PV;

	do {
		nodes = 0;
		std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
		int score = negaMax(color, depth, board, currentPV);
		if (g_stop == false) {
			string scoreStr;
			if (std::abs(score) >= MATE) {
				int multiplier = (score > 0) - (score < 0);
				int64_t moves = multiplier * ((currentPV.size() + 1) / 2); // print negative values for losing mates
				scoreStr = "mate " + std::to_string(moves);
			} else {
				scoreStr = "cp " + std::to_string(score);
			}
			PV = currentPV;
			long long searchtime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
			searchtime = std::max(1LL, searchtime); // searchtime must be > 0 milliseconds
			long long nps = nodes / searchtime * 1000;
			std::cout << "info depth " << depth << " score " << scoreStr << " time " << searchtime << " nodes " << nodes << " nps " << nps << " pv " << printPV(currentPV) << std::endl;
		}
	} while (g_stop == false && depth++ != max_depth);
	std::cout << "bestmove " << StringUtils::moveToString(PV.front()) << std::endl;
}

//Improvement: make a root negaMax, see chess programming network.
int Search::negaMax(int color, int depth, Board& board, std::vector<int>& PV) {
	if (g_stop) {
		return 0;
	}
	if (nodes % 10000 == 0 && timeToMove(color)) {
		g_stop = true;
		return 0;
	}
	if (depth == 0) {
		Evaluation evaluation = Evaluation();
		// -color | 1 changes to 1 or -1 when color is either 0 or 1
		return  (-color | 1) * evaluation.getScore(board);
	}
	int max = -std::numeric_limits<int>::max();
	Movegen movelist = Movegen();
	movelist.generateMoves(color, board);
	for (auto move : movelist.moves) {
		nodes++;
		MoveInfo moveInfo = board.makeMove(color, move);
		std::vector<int> childPV;

		if (!Square::isAttacked(color, board, board.piece_list[color == WHITE ? WHITE_KING : BLACK_KING])) {
			int score = -negaMax(color ^ 1, depth - 1, board, childPV);
			if (score > max) {
				max = score;
				updatePV(PV, childPV, move);
			}
		}
		board.unmakeMove(color, move, moveInfo);
	}
	// no legal moves found, must be mate or stalemate
	if (max == -std::numeric_limits<int>::max())
		if (Square::isAttacked(color, board, board.piece_list[color == WHITE ? WHITE_KING : BLACK_KING])) {
			max = -MATE - depth; // prefer mate at larger depths as the losing side
		} else {
			max = STALE_MATE;
		}
	return max;
}

bool Search::timeToMove(int color) {
	if (g_stop) {
		return true;
	}
	std::chrono::milliseconds searchtime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - time.startTime);
	std::chrono::milliseconds movetime = time.movetime > std::chrono::milliseconds(0) ? time.movetime : color == WHITE ? (time.wtime + (time.winc * EXPECTED_NR_MOVES)) / EXPECTED_NR_MOVES : (time.btime + (time.binc * EXPECTED_NR_MOVES)) / EXPECTED_NR_MOVES;

	return searchtime > movetime;
}

void Search::updatePV(std::vector<int>& PV, const  std::vector<int>& childPV, int move) {
	PV.clear();
	PV.push_back(move);
	std::copy(childPV.begin(), childPV.end(), back_inserter(PV));
}

string Search::printPV(const std::vector<int>& PV) {
	string pvStr = "";
	for (int move : PV) {
		pvStr += StringUtils::moveToString(move);
		pvStr += " ";
	}
	return (pvStr.substr(0, pvStr.size() - 1));
}

Search::~Search() {
}
