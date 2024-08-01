#include "moves.h"


GeneratePositions::GeneratePositions(Position* const position): position_(position), white_occupied_(UpdateWhiteOccupied()), 
black_occupied_(UpdateBlackOccupied()), white_to_move_(position->white_to_move_) {
	/*
	To do:
	populate the children_ set of "position" with generated legal moves

	*/
}

uint64_t GeneratePositions::UpdateWhiteOccupied() {
	return position_->wp_ | position_->wn_ | position_->wb_ | position_->wr_ | position_->wq_ | position_->wk_;
}

uint64_t GeneratePositions::UpdateBlackOccupied() {
	return position_->bp_ | position_->bn_ | position_->bb_ | position_->br_ | position_->bq_ | position_->bk_;
}

void GeneratePositions::UpdateChildBitboards(Position* child) {
	white_bitboards_[0] = &(child->wp_);
	white_bitboards_[1] = &(child->wn_);
	white_bitboards_[2] = &(child->wb_);
	white_bitboards_[3] = &(child->wr_);
	white_bitboards_[4] = &(child->wq_);
	white_bitboards_[5] = &(child->wk_);

	black_bitboards_[0] = &(child->bp_);
	black_bitboards_[1] = &(child->bn_);
	black_bitboards_[2] = &(child->bb_);
	black_bitboards_[3] = &(child->br_);
	black_bitboards_[4] = &(child->bq_);
	black_bitboards_[5] = &(child->bk_);
}

uint64_t GeneratePositions::PawnAttacks(uint64_t curr_pawns) const {
	uint64_t left_captures = curr_pawns << 7 & ~H_FILE_ & black_occupied_;
	uint64_t right_captures = curr_pawns << 9 & ~A_FILE_ & black_occupied_;

	return left_captures | right_captures;
}

uint64_t GeneratePositions::KnightAttacks(int curr_knight) const {
	
}


uint64_t GeneratePositions::AttackSquares() const {
	return 0;
}

//TODO
void GeneratePositions::PawnGen() {
	
}

//TODO
void GeneratePositions::DiagCapture() {
	if (position_->white_to_move_) {
		uint64_t left_captures = (position_->wp_) << 7 & ~H_FILE_ & black_occupied_;
		while (left_captures != 0) {
			Position* new_position = new Position(*position_);
			UpdateChildBitboards(new_position);

			unsigned long curr_bit_idx;
			_BitScanForward64(&curr_bit_idx, left_captures); //finds the least significant non-zero bit
			uint64_t single_pawn_bitboard = 1ULL << curr_bit_idx;
			//BEFORE CODING BELOW, I must check for pins
			new_position->wp_ |= single_pawn_bitboard;		
			uint64_t original_pawn_position = 1ULL << (curr_bit_idx - 7);
			//perform bitwise operation on all black bitboards?
			//if i cannot find a way, simply subtrract original_pawn_position from black bitboards a piece to be captured


		}

	}
}

