#include "moves.h"


GeneratePositions::GeneratePositions(Position* const position): position_(position), 
	white_occupied_(UpdateWhiteOccupied()), black_occupied_(UpdateBlackOccupied()), 
	occupied_(white_occupied_ | black_occupied_), white_to_move_(position->white_to_move_),
	parent_white_bb_(UpdateParentBitboards(MyColor::White)),
	parent_black_bb_(UpdateParentBitboards(MyColor::Black))	
{
	/*
	TODO:
	populate the children_ set of "position" with generated legal moves

	*/
}

GeneratePositions::~GeneratePositions() {}

uint64_t GeneratePositions::UpdateWhiteOccupied() {
	return position_->wp_ | position_->wn_ | position_->wb_ | position_->wr_ | position_->wq_ | position_->wk_;
}

uint64_t GeneratePositions::UpdateBlackOccupied() {
	return position_->bp_ | position_->bn_ | position_->bb_ | position_->br_ | position_->bq_ | position_->bk_;
}

uint64_t GeneratePositions::ComputeMyOccupied(MyColor color) const {
	uint64_t my_occupied = black_occupied_;
	if (color == MyColor::White) {
		my_occupied = white_occupied_;
	}
	return my_occupied;
}

uint64_t* GeneratePositions::UpdateParentBitboards(MyColor color) {
	uint64_t* my_bitboards = new uint64_t[6];
	if (color == MyColor::White) {
		my_bitboards[0] = position_->wp_;
		my_bitboards[1] = position_->wn_;
		my_bitboards[2] = position_->wb_;
		my_bitboards[3] = position_->wr_;
		my_bitboards[4] = position_->wq_;
		my_bitboards[5] = position_->wk_;
	}
	else {
		my_bitboards[0] = position_->bp_;
		my_bitboards[1] = position_->bn_;
		my_bitboards[2] = position_->bb_;
		my_bitboards[3] = position_->br_;
		my_bitboards[4] = position_->bq_;
		my_bitboards[5] = position_->bk_;
	}
	return my_bitboards;
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

// "opponent" refers to the opponent of the person that is taking the turn 
// in position_
uint64_t GeneratePositions::PawnAttacks(uint64_t curr_pawns, uint64_t my_occupied) const {
	uint64_t opponent_occupied = white_occupied_;
	if (my_occupied == white_occupied_) {
		opponent_occupied = black_occupied_;
	}

	uint64_t left_captures = curr_pawns << 7 & ~H_FILE_ & opponent_occupied;
	uint64_t right_captures = curr_pawns << 9 & ~A_FILE_ & opponent_occupied;

	return left_captures | right_captures;
}

//TODO: update Attacks functions to accept turn or my_occupied as parameter

//"my" is in reference to the person taking the turn in position_
uint64_t GeneratePositions::KnightAttacks(int knight_idx, uint64_t my_occupied) const {
	uint64_t knight_attacks = 0;
	if (knight_idx > 18) {
		knight_attacks = KNIGHT_SPAN_ << knight_idx - 18;
	}
	else {
		knight_attacks = KNIGHT_SPAN_ >> 18 - knight_idx;
	}

	if (knight_attacks % 8 > 3) {
		knight_attacks &= ~AB_FILES_;
	}
	else {
		knight_attacks &= ~GH_FILES_;
	}

	return knight_attacks & ~my_occupied;
}

uint64_t GeneratePositions::BishopAttacks(int bishop_idx, uint64_t my_occupied) const {
	int s = bishop_idx;
	uint64_t binary_s = 1ULL << bishop_idx;
	uint64_t diagonal = (((occupied_ & Mask(Direction::Diag, s)) 
		- (2 * binary_s)) ^ R(R(occupied_ & Mask(Direction::Diag, s))
		- (2 * R(binary_s)))) & Mask(Direction::Diag, s);
	uint64_t anti_diagonal = (((occupied_ & Mask(Direction::A_diag, s))
		- (2 * binary_s)) ^ R(R(occupied_ & Mask(Direction::A_diag, s))
		- (2 * R(binary_s)))) & Mask(Direction::A_diag, s);

	return (diagonal | anti_diagonal) & ~my_occupied;
}

uint64_t GeneratePositions::RookAttacks(int rook_idx, uint64_t my_occupied) const {
	int s = rook_idx;
	uint64_t binary_s = 1ULL << rook_idx;
	uint64_t rank = ((occupied_ - 2 * binary_s) ^ 
		R(R(occupied_) - 2 * R(binary_s))) & Mask(Direction::Rank, s);
	uint64_t file = (((occupied_ & Mask(Direction::File, s)) - (2 * binary_s)) 
		^ R(R(occupied_ & Mask(Direction::File, s)) - (2 * R(binary_s)))) 
		& Mask(Direction::File, s);

	return (rank | file) & ~my_occupied;
}

uint64_t GeneratePositions::QueenAttacks(int queen_idx, uint64_t my_occupied) const {
	return BishopAttacks(queen_idx, my_occupied) | RookAttacks(queen_idx, my_occupied);
}

uint64_t GeneratePositions::KingAttacks(MyColor color) const {
	uint64_t king_attacks = 0;
	unsigned long king_idx;
	if (color == MyColor::White) {
		_BitScanForward64(&king_idx, position_->wk_);
	}
	else {
		_BitScanForward64(&king_idx, position_->bk_);
	}

	if (king_idx > 9) {
		king_attacks = KING_SPAN_ << king_idx - 9;
	}
	else {
		king_attacks = KING_SPAN_ >> 9 - king_idx;
	}

	if (king_idx % 8 > 3) {
		king_attacks &= ~AB_FILES_;
	}
	else {
		king_attacks &= ~GH_FILES_;
	}

	return king_attacks & ~ComputeMyOccupied(color);
}


uint64_t GeneratePositions::AttackSquares(MyColor color) const {
	uint64_t my_occupied = ComputeMyOccupied(color);
	uint64_t all_pawn_attacks = PawnAttacks((my_bitboards_.at(color))[0], my_occupied);
	uint64_t all_knight_attacks = GeneratePieceAttacks(
		[this](int idx, uint64_t occupied) { return this->KnightAttacks(idx, occupied); },
		my_occupied, (my_bitboards_.at(color))[1]);
	uint64_t all_bishop_attacks = GeneratePieceAttacks(
		[this](int idx, uint64_t occupied) { return this->BishopAttacks(idx, occupied); },
		my_occupied, (my_bitboards_.at(color))[2]);
	uint64_t all_rook_attacks = GeneratePieceAttacks(
		[this](int idx, uint64_t occupied) { return this->RookAttacks(idx, occupied); },
		my_occupied, (my_bitboards_.at(color))[3]);
	uint64_t all_queen_attacks = GeneratePieceAttacks(
		[this](int idx, uint64_t occupied) { return this->QueenAttacks(idx, occupied); },
		my_occupied, (my_bitboards_.at(color))[4]);
	uint64_t all_king_attacks = KingAttacks(color);
	return all_pawn_attacks | all_knight_attacks | all_bishop_attacks | all_rook_attacks |
		all_queen_attacks | all_king_attacks;
}

uint64_t GeneratePositions::GeneratePieceAttacks(
	std::function<uint64_t(int, uint64_t)> PieceAttacks,
	uint64_t my_occupied, uint64_t piece_bitboard) const {

	uint64_t all_attacks = 0;
	unsigned long curr_bit_idx;
	while (piece_bitboard != 0) {
		_BitScanForward64(&curr_bit_idx, piece_bitboard);
		all_attacks |= PieceAttacks(curr_bit_idx, my_occupied);
		piece_bitboard &= ~(1ULL << curr_bit_idx);
	}
	return all_attacks;
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

uint64_t GeneratePositions::R(uint64_t original_bitboard) const {
	uint64_t result = 0;
	for (int idx = 0; idx < 64; idx++) {
		result = (result << 1) | (original_bitboard & 1);
		original_bitboard >>= 1;
	}
	return result;
}

uint64_t GeneratePositions::Mask(Direction direction, int piece_idx) const {
	switch (direction)
	{
	case Direction::Diag:
		return DIAGONALS_[(piece_idx / 8) + 7 - (piece_idx % 8)];
	case Direction::A_diag:
		return ANTI_DIAGONALS_[(piece_idx / 8) + (piece_idx % 8)];
	case Direction::File:
		return FILES_[piece_idx % 8];
	case Direction::Rank:
		return RANKS_[piece_idx / 8];
	default:
		Error::error("unspecified mask");
		break;
	}
}