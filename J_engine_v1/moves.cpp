#include "moves.h"


GeneratePositions::GeneratePositions(Position* const position): position_(position) {
	/*
	To do:
	populate the children_ set of "position" with generated legal moves

	*/
}


void GeneratePositions::PawnGen() {

}

void GeneratePositions::DiagCapture() {
	if (position_->white_to_move_) {
		uint64_t left_captures = (position_->wp_) << 7 & ~H_FILE & position_->black_occupied_;
		while (left_captures != 0) {
			Position* new_position = new Position(*position_);
			unsigned long curr_bit_idx;
			_BitScanForward64(&curr_bit_idx, left_captures); //finds the least significant non-zero bit
			new_position->wp_ |= 1ULL << curr_bit_idx;
			uint64_t original_pawn_position = 1ULL << (curr_bit_idx - 7);
			//perform bitwise operation on all black bitboards?
			//if i cannot find a way, simply subtrract original_pawn_position from black bitboards a piece to be captured

		}

	}
}

