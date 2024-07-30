#pragma once

#include "libraries.h"
#include "position.h"
#include "error_handling.h"

class GeneratePositions {
public:
	GeneratePositions(Position* position);
	
private:
	void PawnGen();
	void DiagCapture();


	//the tree of Position objects will be dynamically allocated
	Position* const position_;

	const uint64_t H_FILE = 0x8080808080808080ULL;
};