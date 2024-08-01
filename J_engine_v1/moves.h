#pragma once

#include "libraries.h"
#include "position.h"
#include "error_handling.h"

class GeneratePositions {
public:
	GeneratePositions(Position* position);
	
private:
	uint64_t UpdateWhiteOccupied();
	uint64_t UpdateBlackOccupied();
	void UpdateChildBitboards(Position* child);

	uint64_t PawnAttacks(uint64_t curr_pawns) const;
	uint64_t KnightAttacks(int curr_knight) const;
	uint64_t BishopAttacks(int curr_bishop) const;
	uint64_t RookAttacks(int curr_rook) const;
	uint64_t QueenAttacks(int curr_queen) const;
	uint64_t KingAttacks() const;
	uint64_t AttackSquares() const;

	void PawnGen();
	void DiagCapture();


	//the tree of Position objects will be dynamically allocated
	Position* const position_;

	const bool white_to_move_;

	//refers to the occupied squares of the parent: "position"
	const uint64_t white_occupied_;
	const uint64_t black_occupied_;

	/*refers to the bitboards of the CHILD.These values
	* are different for each new position.
	*/ 
	uint64_t* white_bitboards_[6];
	uint64_t* black_bitboards_[6];

	//masks
	const uint64_t A_FILE_ = 0x101010101010101ULL;
	const uint64_t H_FILE_ = 0x8080808080808080ULL;

	const uint64_t KNIGHT_SPAN_ = 43234889994ULL;
};