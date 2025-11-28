#include <chrono>
#include <thread>
#include <future>
#include <limits>
#include <iostream>
#include "uci.h"
#include "perft.h"
#include "search.h"
#include "movegen.h"
#include "stringutils.h"
#include "version.h"

std::atomic<bool> g_stop;

using StringUtils::split;

Uci::Uci() = default;

void Uci::loop() {
	string input;
	do {
		std::getline(std::cin, input);
		std::vector<std::string> commands = split(input, ' ');

		if (input == "uci") {
			uci();
		} else if (input == "ucinewgame") {
			ucinewgame();
		} else if (input == "isready") {
			isready();
		} else if (commands[0] == "position") {
			position(commands);
		} else if (commands[0] == "go") {
			go(commands);
		} else if (commands[0] == "perft") {
			perft(false, commands);
		} else if (commands[0] == "divide") {
			perft(true, commands);
		} else if (input == "stop") {
			stop();
		}
	} while (input != "quit");
}

void Uci::uci() {
	std::cout << "id name drawcula " << VERSION << std::endl;
	std::cout << "id author " << AUTHOR << std::endl;
	std::cout << "uciok" << std::endl;
}

void Uci::ucinewgame() {
	resetBoard(FenReader::read(startpos));
}

void Uci::isready() {
	std::cout << "readyok" << std::endl;
}

void Uci::position(const std::vector<std::string>& commands) {
	string fenStr;
	U64 i = 1;
	if (commands[i] == "startpos") {
		fenStr = startpos;
		i++;
	} else if (commands[i] == "fen") {
		fenStr = commands[i + 1] + " " + commands[i + 2] + " " + commands[i + 3] + " " + commands[i + 4] + " " + commands[i + 5] + " " + commands[i + 6];
		i += 7;
	}
	resetBoard(FenReader::read(fenStr));

	if (commands.size() > i && commands[i] == "moves") {
		for (i++; i < commands.size(); i++) {
			// TODO: remove color here
			auto movelist = Movegen();
			movelist.generateMoves(color, board);
			const int move = movelist.getLegalMove(stringToMove(commands[i]));
			board.makeMove(color, move);
			changeColor();
		}
	}
}

void Uci::go(const std::vector<std::string>& commands) {
	g_stop = false;
	Params params;

	for (std::vector<string>::size_type i = 1; i != commands.size(); i++) {
		if (commands[i] == "movetime") {
			params.movetime = std::chrono::milliseconds(std::stoi(commands[++i]));
		} else if (commands[i] == "wtime") {
			params.wtime = std::chrono::milliseconds(std::stoi(commands[++i]));
		} else if (commands[i] == "btime") {
			params.btime = std::chrono::milliseconds(std::stoi(commands[++i]));
		} else if (commands[i] == "winc") {
			params.winc = std::chrono::milliseconds(std::stoi(commands[++i]));
		} else if (commands[i] == "binc") {
			params.binc = std::chrono::milliseconds(std::stoi(commands[++i]));
		} else if (commands[i] == "movestogo") {
			// TODO: implement this
			++i;
		} else if (commands[i] == "depth") {
			params.max_depth = std::stoi(commands[++i]);
			params.movetime = std::chrono::milliseconds(std::numeric_limits<long long>::max());
		} else if (commands[i] == "infinite") {
			params.movetime = std::chrono::milliseconds(std::numeric_limits<long long>::max());
		}
	}
	auto search = Search();
	std::thread t1(&Search::start, search, color, board, params);
	t1.detach();
}

void Uci::perft(const bool divide, const std::vector<std::string>& commands) {
	const int depth = std::stoi(commands[1]);
	auto perft = Perft(divide, depth);
	std::thread t1(&Perft::calculate, perft, color, board);
	t1.detach();
}

void Uci::stop() {
	g_stop = true;
}

void Uci::changeColor() {
	color ^= 1;
}

void Uci::resetBoard(const Fen& fen) {
	// reset board and set new position
	board.~Board();
	new (&board) Board(fen);

	color = fen.color;
	half_moves = fen.half_moves; // not used at the moment
	full_moves = fen.full_moves; // not used at the moment
}

int Uci::stringToMove(const string& moveStr) {
	constexpr int piece = NO_PIECE;
	const int from = StringUtils::getSquare(moveStr[0], moveStr[1]);
	const int to = StringUtils::getSquare(moveStr[2], moveStr[3]);
	const int promotion = moveStr.length() == 5 ? Piece::getPromotion(moveStr[4]) : NO_PROMOTION;

	return (from << 6) + (to << 12) + (piece << 18) + (promotion << 27);
}

Uci::~Uci() = default;
