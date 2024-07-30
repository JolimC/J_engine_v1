#include "libraries.h"
#include "error_handling.h"
#include "position.h"
#include "moves.h"

int main() {
	Position* p = new Position("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R w q - 100 20");
	p->DisplayPosition();
	GeneratePositions gp(p);
	

	//correct FEN
	//Position p("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");
}