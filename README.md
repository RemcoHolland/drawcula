🧛‍♂️ Drawcula

“I vant… to drawww your games!”

Drawcula is a quirky, solid, and slightly spooky chess engine that lurks deep in your CPU, waiting for the next opponent to drain of Elo. Built for reliability, resilience, and a sometimes-uncanny ability to drag positions toward draws (but only when strategically appropriate, of course).

It also roams Lichess at night. Beware.

🧛 What is Drawcula?

Drawcula is an open-source chess engine designed for strong, stubborn play.
It evaluates positions, searches variations, and occasionally turns into a bat.
(That last one is metaphorical. Probably.)

Whether you're testing openings, playing casual matches, or want a sparring partner that doesn't throw games away, Drawcula has your back.

🦇 Features

🧠 Sharp evaluation of material, mobility, king safety, and more.

⚡ Multithreaded search for speedy bite-sized brilliance.

♟️ UCI compliant, so it works in your favorite GUI.

📜 Supports FEN and PGN setups.

🎚️ Adjustable style — from peaceful monk to bloodthirsty tactician.

🌙 Lichess integration — fight it online after midnight (local time optional).

🛠️ Installation
Prerequisites

A C++ compiler (GCC, Clang, MSVC — no silver stakes required)

Any required libraries listed in the repo

A chess GUI (Arena, Cute Chess, whatever you summon)

Build from source
git clone https://github.com/RemcoHolland/drawcula.git
cd drawcula
mkdir build && cd build
cmake ..
make


The resulting executable is your new undead chess companion.

🏃 Usage

Launch via UCI:

./drawcula --uci


Then talk to it the standard UCI way:

uci
isready
position startpos moves e2e4 e7e5
go depth 10


🎃 Or start from a scary FEN:

position fen r1bqkb1r/pppp1ppp/2n2n2/4N3/4P3/2N5/PPPP1PPP/R1BQKB1R b KQkq - 0 1

go movetime 5000

🧰 Engine Options

Tune Drawcula’s personality:

Threads — how many minions it can command [TODO]

HashSize — memory for its ancient tomes [TODO]

Aggressiveness — from “let’s peacefully coexist” to “I CRAVE TACTICS” [TODO]

Ponder — think during your time [TODO]

UCI_Elo — dial its power level like a vampire who hasn’t fed yet [TODO]

🌑 Drawcula on Lichess

You can challenge Drawcula online — but don’t be surprised if it stares into your soul first.

Typical Lichess behavior:

Accepts most time controls

Plays steady, human-friendly chess

Enjoys long walks by the FEN string

May or may not appear in engine tournaments depending on moon phase

Search for the engine’s Lichess account [Drawcula_BOT](https://lichess.org/@/Drawcula_BOT) and send a challenge!

🧛‍♀️ Contributing

Want to help Drawcula grow stronger? Contributions are welcome!

Fork the repo

Create a branch

Add features, bug fixes, or garlic-resistant improvements

Submit a PR

Celebrate with a glass of… uh… grape juice 🍷

📜 License

MIT License — permissive, friendly, and definitely not cursed.

🕯️ Acknowledgements

Thanks to everyone who tests, contributes, or battles Drawcula on moonlit nights.
Special thanks to other open-source engines for inspiration, and to Lichess for being an excellent haunt.

If you enjoy Drawcula, leave a ★ on GitHub — it keeps the engine alive.
Well… undead. 🧛‍♂️