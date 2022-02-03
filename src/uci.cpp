#include "uci.h"

std::atomic<bool> g_stop;

Uci::Uci() {
}

void Uci::loop() {
	string input;
	do {
		std::getline(std::cin, input, '\n');

		if (input == "uci") {
			uci();
		} else if (input == "ucinewgame") {
			ucinewgame();
		} else if (input == "isready") {
			isready();
		} else if (startsWith(input, "position")) {
			input = input.substr(8);
			trim(input);
			position(input);
		} else if (startsWith(input, "go")) {
			input = input.substr(2);
			trim(input);
			go(input);
		} else if (startsWith(input, "perft")) {
			input = input.substr(5);
			trim(input);
			perft(false, input);
		} else if (startsWith(input, "divide")) {
			input = input.substr(6);
			trim(input);
			perft(true, input);
		} else if (input == "stop") {
			stop();
		}
	} while (input != "quit");
}

void Uci::uci() {
	std::cout << "id name drawcula" << std::endl;
	std::cout << "id author R. Holland" << std::endl;
	std::cout << "uciok" << std::endl;
}

void Uci::ucinewgame() {
	;
}

void Uci::isready() {
	std::cout << "readyok" << std::endl;
}

void Uci::position(string input) {
	string fenStr;
	if (startsWith(input, "startpos")) {
		fenStr = startpos;
	} else if (startsWith(input, "fen")) {
		string fen = input.substr(3);
		fen = fen.substr(0, fen.find("moves"));
		trim(fen);
		fenStr = fen;
	}
	FenInfo position = FenInfo(FenReader::read(fenStr));
	board.setPosition(position);
	color = position.color;
	half_moves = position.half_moves; // not used at the moment
	full_moves = position.full_moves; // not used at the moment

	if (contains(input, "moves")) {
		string moves = input.substr(input.find("moves") + 5);
		trim(moves);
		std::vector<std::string> stringMoves = split(moves, ' ');
		for (string moveStr : stringMoves) {
			// TODO: remove color here
			Movelist movelist = Movelist();
			movelist.generateMoves(color, board);
			Move move = movelist.getLegalMove(Move(moveStr, color));
			board.makeMove(color, move);
		}
	}
}

void Uci::go(string input) {
	g_stop = false;
	struct Time time;
	int max_depth = std::numeric_limits<int>::max();

	std::vector<string> commands = split(input, ' ');
	for (std::vector<string>::size_type i = 0; i != commands.size(); i++) {
		if (commands[i] == "movetime") {
			time.movetime = std::chrono::milliseconds(std::stoi(commands[++i]));
		} else if (commands[i] == "wtime") {
			time.wtime = std::chrono::milliseconds(std::stoi(commands[++i]));
		} else if (commands[i] == "btime") {
			time.btime = std::chrono::milliseconds(std::stoi(commands[++i]));
		} else if (commands[i] == "winc") {
			time.winc = std::chrono::milliseconds(std::stoi(commands[++i]));
		} else if (commands[i] == "binc") {
			time.binc = std::chrono::milliseconds(std::stoi(commands[++i]));
		} else if (commands[i] == "movestogo") {
			// TODO: implement this
			++i;
		} else if (commands[i] == "depth") {
			max_depth = std::stoi(commands[++i]);
			time.movetime = std::chrono::milliseconds(std::numeric_limits<long long>::max());
		} else if (commands[i] == "infinite") {
			time.movetime = std::chrono::milliseconds(std::numeric_limits<long long>::max());
		}
	}
	Search search = Search();
	time.startTime = std::chrono::steady_clock::now();
	std::thread t1(&Search::start, search, color, max_depth, board, time);
	t1.detach();
}

void Uci::perft(bool divide, string input) {
	int depth = std::stoi(input);
	Perft perft = Perft(divide, depth);
	std::thread t1(&Perft::calculate, perft, color, board);
	t1.detach();
}

void Uci::stop() {
	g_stop = true;
}

bool Uci::startsWith(string mainStr, string toMatch) {
	return mainStr.find(toMatch) == 0;
}

bool Uci::contains(string mainStr, string toMatch) {
	return mainStr.find(toMatch) != std::string::npos;
}

void Uci::changeColor() {
	color ^= 1;
}

Uci::~Uci() {
}
