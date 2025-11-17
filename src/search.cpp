#include <iostream>
#include <vector>
#include "color.h"
#include "movegen.h"
#include "search.h"
#include "square.h"
#include "stringutils.h"
#include "evaluation.h"

Search::Search() {
	g_stop = false;  // reset global stop variable
}

U64 Search::getNodes() {
	return nodes;
}

int Search::start(int color, Board board, const Params params) {
	std::srand((unsigned)std::time(NULL));  // initialize random seed
	int depth = 1;
	this->params = params;
	std::vector<int> PV;

	do {
		std::vector<int> currentPV;
		nodes = 0;
		std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
		int score = alphaBeta(color, -MAX_INT, MAX_INT, depth, board, currentPV);
		if (!g_stop) {
			string scoreStr;
			if (std::abs(score) >= MATE) {
				int multiplier = (score > 0) - (score < 0);
				int64_t nr_moves = multiplier * ((currentPV.size() + 1) / 2); // print negative values for losing mates
				scoreStr = "mate " + std::to_string(nr_moves);
			} else {
				scoreStr = "cp " + std::to_string(score);
			}
			PV = currentPV;
			long long searchtime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
			searchtime = std::max(1LL, searchtime); // searchtime must be > 0 milliseconds
			long long nps = nodes / searchtime * 1000;
			std::cout << "info depth " << depth << " score " << scoreStr << " time " << searchtime << " nodes " << nodes << " nps " << nps << " pv " << printPV(currentPV) << std::endl;
		}
	} while (!g_stop && depth++ != params.max_depth);
	const int bestmove = PV.front();
	std::cout << "bestmove " << StringUtils::moveToStringAN(bestmove) << std::endl;
	return bestmove;
}

// Fail soft AlphaBeta search
int Search::alphaBeta(int color, int alpha, int beta, int depth, Board& board, std::vector<int>& PV) {
	if (g_stop) {
		return 0;
	}
	if (nodes % 20000 == 0 && timeToMove(color)) {
		g_stop = true;
		return 0;
	}
	if (depth == 0) {
		int random = (std::rand() % 21) - 10; // add some randomness (-10..10) to the evaluation
		return  (-color | 1) * (board.getScore() + random); // -color | 1 changes to 1 or -1 when color is either 0 or 1
	}
	int bestscore = -MAX_INT;
	Movegen movegen = Movegen();
	movegen.generateMoves(color, board);
	movegen.sortMoves();
	for (int move : movegen.moves) {
		nodes++;
		int unmake_info = board.makeMove(color, move);

		if (!square::isAttacked(color, board, board.piece_list[color][KING])) {
			std::vector<int> childPV;
			int score = -alphaBeta(color ^ 1, -beta, -alpha, depth - 1, board, childPV);
			if (score >= beta) {
				board.unmakeMove(color, move, unmake_info);
				return score;
			}
			if (score > bestscore) {
				bestscore = score;
				if (score > alpha) {
					alpha = score;
					updatePV(PV, childPV, move); // TODO: check is this the right place?
				}
			}
		}
		board.unmakeMove(color, move, unmake_info);
	}
	// no legal moves found, must be mate or stalemate
	if (bestscore == -MAX_INT) {
		if (square::isAttacked(color, board, board.piece_list[color][KING])) {
			bestscore = -MATE - depth; // prefer mate at larger depths as the losing side
		} else {
			bestscore = STALE_MATE;
		}
	}
	return bestscore;
}

bool Search::timeToMove(int color) {
	if (g_stop) {
		return true;
	}
	std::chrono::milliseconds searchtime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - params.startTime);
	std::chrono::milliseconds movetime = params.movetime > std::chrono::milliseconds(0) ? params.movetime : color == WHITE ? (params.wtime + (params.winc * EXPECTED_NR_MOVES)) / EXPECTED_NR_MOVES : (params.btime + (params.binc * EXPECTED_NR_MOVES)) / EXPECTED_NR_MOVES;

	return searchtime > movetime;
}

void Search::updatePV(std::vector<int>& PV, const std::vector<int>& childPV, int move) {
	PV.clear();
	PV.push_back(move);
	std::copy(childPV.begin(), childPV.end(), back_inserter(PV));
}

string Search::printPV(const std::vector<int>& PV) {
	string pvStr;
	for (int move : PV) {
		pvStr += StringUtils::moveToStringAN(move);
		pvStr += " ";
	}
	return (pvStr.substr(0, pvStr.size() - 1));
}

Search::~Search() {
}
