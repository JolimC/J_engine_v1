#include "libraries.h"
#include "error_handling.h"
#include "position.h"
#include "moves.h"
#include "testing.h"

int main() {
	Position* p = new Position("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R w q - 100 20");
	p->DisplayPosition();
	std::cout << std::endl;

	GeneratePositions gp(p);

	/*uint64_t as1 = gp.AttackSquares(MyColor::White);
	uint64_t p1 = gp.PawnAttacks((gp.parent_white_bb_)[0], gp.white_occupied_);*/

	Testing t1;

	//correct FEN
	//Position p("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");
}