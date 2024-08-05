#include "testing.h"

Testing::Testing() {
	TestCases();
}


/*
Key:
c - correct
w - white
b - black
p,n,b,q,k - pawn, knight, bishop, queen, king

*/
void Testing::TestCases() {
	std::string fen1 = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	Position* p1 = new Position(fen1);
	GeneratePositions gp1(p1);
	const uint64_t C_WP_ATTACKS_1 = 0xFF0000ULL;
	assert(MirrorBB(C_WP_ATTACKS_1) == gp1.PawnAttacks(p1->wp_, gp1.white_occupied_));
	assert(MirrorBB(0xA00000ULL) == gp1.KnightAttacks(1, gp1.white_occupied_));
	assert(MirrorBB(0x50000ULL) == gp1.KnightAttacks(6, gp1.white_occupied_));
	assert(MirrorBB(0ULL) == gp1.BishopAttacks(2, gp1.white_occupied_));
	assert(MirrorBB(0ULL) == gp1.BishopAttacks(5, gp1.white_occupied_));
	assert(MirrorBB(0ULL) == gp1.RookAttacks(0, gp1.white_occupied_));
	assert(MirrorBB(0ULL) == gp1.RookAttacks(7, gp1.white_occupied_));
	assert(MirrorBB(0ULL) == gp1.QueenAttacks(3, gp1.white_occupied_));
	assert(MirrorBB(0ULL) == gp1.KingAttacks(MyColor::White));
	assert(MirrorBB(0xFF0000ULL) == gp1.AttackSquares(MyColor::White));

	std::string fen2 = "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R w q - 100 20";
	Position* p2 = new Position(fen2);
	GeneratePositions gp2(p2);
	const uint64_t C_WP_ATTACKS_2 = 0x1400FB0000ULL;
	assert(MirrorBB(C_WP_ATTACKS_2) == gp2.PawnAttacks(p2->wp_,gp2.white_occupied_));
	assert(MirrorBB(0xA00000ULL) == gp2.KnightAttacks(1, gp2.white_occupied_));
	assert(MirrorBB(0xA11000002ULL) == gp2.KnightAttacks(21, gp2.white_occupied_));
	assert(MirrorBB(0ULL) == gp2.BishopAttacks(2, gp2.white_occupied_));
	assert(MirrorBB(0x804020100800ULL) == gp2.BishopAttacks(5, gp2.white_occupied_));
	assert(MirrorBB(0ULL) == gp2.RookAttacks(0, gp2.white_occupied_));
	assert(MirrorBB(0x2ULL) == gp2.RookAttacks(7, gp2.white_occupied_));
	assert(MirrorBB(0x800ULL) == gp2.QueenAttacks(3, gp2.white_occupied_));
	assert(MirrorBB(0x800ULL) == gp2.KingAttacks(MyColor::White));
	assert(MirrorBB(0x805E31FB0802ULL) == gp2.AttackSquares(MyColor::White));
	
	std::string fen3 = "2r1r3/pp1bqppk/2p1p2p/3p1n2/3P1P2/2N1P3/PPP3PP/R1BQKB1R w KQ - 0 1";
	Position* p3 = new Position(fen3);
	GeneratePositions gp3(p3);
	const uint64_t C_WP_ATTACKS_3 = 0x2A00D70000ULL;
	assert(MirrorBB(C_WP_ATTACKS_3) == gp3.PawnAttacks(p3->wp_, gp3.white_occupied_));
	assert(MirrorBB(0x5088000840ULL) == gp3.KnightAttacks(18, gp3.white_occupied_));
	assert(MirrorBB(0x1000ULL) == gp3.BishopAttacks(2, gp3.white_occupied_));
	assert(MirrorBB(0x804020100800ULL) == gp3.BishopAttacks(5, gp3.white_occupied_));
	assert(MirrorBB(0x40ULL) == gp3.RookAttacks(0, gp3.white_occupied_));
	assert(MirrorBB(0x2ULL) == gp3.RookAttacks(7, gp3.white_occupied_));
	assert(MirrorBB(0x102141800ULL) == gp3.QueenAttacks(3, gp3.white_occupied_));
	assert(MirrorBB(0x1C00ULL) == gp3.KingAttacks(MyColor::White));
	assert(MirrorBB(0x807BAAD71C42ULL) == gp3.AttackSquares(MyColor::White));


	std::string fen4 = "rnbq1rk1/ppp1ppbp/3p1np1/8/2PP4/2N1PN2/PP3PPP/R1BQKB1R w KQ - 0 1";
	Position* p4 = new Position(fen4);
	GeneratePositions gp4(p4);
	const uint64_t C_WP_ATTACKS_4 = 0x7804C30000ULL;
	assert(MirrorBB(C_WP_ATTACKS_4) == gp4.PawnAttacks(p4->wp_, gp4.white_occupied_));

	std::string fen5 = "8/2k5/8/8/8/8/4K3/3R4 w - - 0 1";
	Position* p5 = new Position(fen5);
	GeneratePositions gp5(p5);
	const uint64_t C_WP_ATTACKS_5 = 0ULL;
	assert(MirrorBB(C_WP_ATTACKS_5) == gp5.PawnAttacks(p5->wp_, gp5.white_occupied_));
	

	


	std::cout << std::endl << "assertions passed" << std::endl;
}

uint64_t Testing::MirrorBB(uint64_t bitboard) {
	uint64_t mirrored_bb = 0;
	int mirrored_idx = 63;
	for (int k = 56; k >= 0; k -= 8) {
		for (int file = 0; file < 8; file++) {
			if ((bitboard & (1ULL << k + file)) != 0) {
				mirrored_bb |= 1ULL << mirrored_idx;
			}
			mirrored_idx--;
		}
	}
	return mirrored_bb;
}

void Testing::PrintBitboard(uint64_t bitboard) {
	for (int k = 63; k >= 0; k--) {
		uint64_t curr_bitboard = (1ULL << k);
		if ((curr_bitboard & bitboard) == curr_bitboard) {
			std::cout << 1;
		}
		else {
			std::cout << 0;
		}
		if (k % 8 == 0) {
			std::cout << "\n";
		}
	}
}
