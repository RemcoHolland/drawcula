#pragma once
#include <string>
#include "piece.h"
#include "color.h"
#include "castling.h"
#include "position.h"
#include "flag.h"

using std::string;
typedef uint64_t U64;

constexpr int RANKS = 8;
constexpr int FILES = 8;
constexpr int SQUARES = RANKS * FILES;
constexpr U64 RANK_1 = 255;
constexpr U64 RANK_2 = 65280;
constexpr U64 RANK_4 = 4278190080;
constexpr U64 RANK_5 = 1095216660480;
constexpr U64 RANK_7 = 71776119061217280;
constexpr U64 RANK_8 = 18374686479671623680;
constexpr U64 PROMOTION_RANK = RANK_1 | RANK_8;
constexpr U64 FILE_A = 72340172838076673;
constexpr U64 FILE_H = 9259542123273814144;

constexpr U64 A1 = 1;
constexpr U64 B1 = 2;
constexpr U64 C1 = 4;
constexpr U64 D1 = 8;
constexpr U64 E1 = 16;
constexpr U64 F1 = 32;
constexpr U64 G1 = 64;
constexpr U64 H1 = 128;
constexpr U64 A8 = 72057594037927936;
constexpr U64 H8 = 9223372036854775808;

constexpr U64 KING_SIDE_SQUARES = F1 | G1;
constexpr U64 QUEEN_SIDE_SQUARES = B1 | C1 | D1;

constexpr U64 PAWN_ATTACKS[COLORS][SQUARES] = { {
	512,				1280,				2560,				5120,			     10240,			      20480,			   40960,			     16384,
	131072,			    327680,			    655360,			    1310720,			 2621440,		      5242880,			   10485760,			 4194304,
	33554432,           83886080,           167772160,          335544320,           671088640,           1342177280,          2684354560,           1073741824,
	8589934592,         21474836480,        42949672960,        85899345920,         171798691840,        343597383680,        687194767360,         274877906944,
	2199023255552,      5497558138880,      10995116277760,     21990232555520,      43980465111040,      87960930222080,      175921860444160,      70368744177664,
	562949953421312,    1407374883553280,   2814749767106560,   5629499534213120,    11258999068426240,   22517998136852480,   45035996273704960,    18014398509481984,
	0,                  0,                  0,                  0,                   0,                   0,                   0,                    0,
	0,                  0,                  0,                  0,                   0,                   0,                   0,                    0
},
{
	0,                  0,                  0,                  0,                   0,                   0,                   0,                    0,
	0,                  0,                  0,                  0,                   0,                   0,                   0,                    0,
	512,				1280,				2560,				5120,			     10240,			      20480,			   40960,			     16384,
	131072,			    327680,			    655360,			    1310720,			 2621440,		      5242880,			   10485760,			 4194304,
	33554432,           83886080,           167772160,          335544320,           671088640,           1342177280,          2684354560,           1073741824,
	8589934592,         21474836480,        42949672960,        85899345920,         171798691840,        343597383680,        687194767360,         274877906944,
	2199023255552,      5497558138880,      10995116277760,     21990232555520,      43980465111040,      87960930222080,      175921860444160,      70368744177664,
	562949953421312,    1407374883553280,   2814749767106560,   5629499534213120,    11258999068426240,   22517998136852480,   45035996273704960,    18014398509481984
} };

constexpr U64 KNIGHT_MOVES[SQUARES] = {
	132096,				329728,				659712,				 1319424,			  2638848,			   5277696,				10489856,			  4202496,
	33816580,			84410376,			168886289,			 337772578,			  675545156,		   1351090312,			2685403152,			  1075839008,
	8657044482,		    21609056261,		43234889994,		 86469779988,		  172939559976,		   345879119952,		687463207072,		  275414786112,
	2216203387392,	    5531918402816,		11068131838464,		 22136263676928,	  44272527353856,	   88545054707712,		175990581010432,	  70506185244672,
	567348067172352,	1416171111120896,	2833441750646784,	 5666883501293568,	  11333767002587136,   22667534005174272,   45053588738670592,	  18049583422636032,
	145241105196122112,	362539804446949376, 725361088165576704,	 1450722176331153408, 2901444352662306816, 5802888705324613632, 11533718717099671552, 4620693356194824192,
	288234782788157440, 576469569871282176, 1224997833292120064, 2449995666584240128, 4899991333168480256, 9799982666336960512, 1152939783987658752,  2305878468463689728,
	1128098930098176,   2257297371824128,	4796069720358912,	 9592139440717824,	  19184278881435648,   38368557762871296,	4679521487814656,	  9077567998918656
};

constexpr U64 KING_MOVES[SQUARES] = {
	770,				1797,				3594,				 7188,				  14376,			   28752,				57504,				  49216,
	197123,				460039,				920078,				 1840156,			  3680312,			   7360624,				14721248,			  12599488,
	50463488,			117769984,			235539968,			 471079936,			  942159872,		   1884319744,			3768639488,			  3225468928,
	12918652928,		30149115904,		60298231808,		 120596463616,		  241192927232,		   482385854464,		964771708928,		  825720045568,
	3307175149568,		7718173671424,		15436347342848,		 30872694685696,	  61745389371392,	   123490778742784,		246981557485568,	  211384331665408,
	846636838289408,	1975852459884544,	3951704919769088,	 7903409839538176,	  15806819679076352,   31613639358152704,	63227278716305408,	  54114388906344448,
	216739030602088448, 505818229730443264, 1011636459460886528, 2023272918921773056, 4046545837843546112, 8093091675687092224, 16186183351374184448, 13853283560024178688,
	144959613005987840, 362258295026614272, 724516590053228544,	 1449033180106457088, 2898066360212914176, 5796132720425828352, 11592265440851656704, 4665729213955833856
};

// move masks
constexpr int SORT_KEY_MASK = 0b111111;
constexpr int FROM_MASK = 0b111111 << 6;
constexpr int TO_MASK = 0b111111 << 12;
constexpr int PIECE_MASK = 0b111 << 18;
constexpr int FLAG_MASK = 0b111 << 21;
constexpr int CAPTURED_PIECE_MASK = 0b111 << 24;
constexpr int PROMOTION_MASK = 0b111 << 27;

// unmake move masks
constexpr int CAPTURE_MASK = 0b111111;   // USE SHORT OR SOMETHING INSTEAD OF INT???
constexpr int ENPASSANT_MASK = 0b111111 << 6;
constexpr int CASTLING_MASK = 0b111111 << 12;

class Board {

public:
	Board(const Position&);

	U64 piece_list[COLORS][PIECES];
	U64 occupiedBB = 0;
	U64 colorBB[2] = { 0 };
	int enpassant_square = 0;
	int castling_rights = 0;

	void setPosition(const Position&);
	const int makeMove(int, int);
	void unmakeMove(int, int, int);

	~Board();

private:
	void init(const Position&);
	void setCastlingRights(int, int, int, U64, U64);
};
