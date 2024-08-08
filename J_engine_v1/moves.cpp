#include "moves.h"


GeneratePositions::GeneratePositions(Position* const position): position_(position), 
	white_to_move_(position->white_to_move_)
{
	InitializePositionInfo(PositionType::Parent);
	InitializePositionInfo(PositionType::Child);
	/*
	To do:
	populate the children_ set of "position" with generated legal moves

	*/
}

GeneratePositions::~GeneratePositions() {
}


/*
When INITIALIZING the values of the either parent_info_
or child_info_, bitboards from position_ will be used.
For chiild_info_, the bitboard values will be modified later.
*/
void GeneratePositions::InitializePositionInfo(PositionType type) const {
	PositionInfo* white_info;
	PositionInfo* black_info;

	if (type == PositionType::Parent) {
		white_info = parent_white_info_;
		black_info = parent_black_info_;
	}
	else {
		white_info = child_white_info_;
		black_info = child_black_info_;
	}

	(white_info->my_bitboards_)[0] = position_->wp_;
	(white_info->my_bitboards_)[1] = position_->wn_;
	(white_info->my_bitboards_)[2] = position_->wb_;
	(white_info->my_bitboards_)[3] = position_->wr_;
	(white_info->my_bitboards_)[4] = position_->wq_;
	(white_info->my_bitboards_)[5] = position_->wk_;

	(black_info->my_bitboards_)[0] = position_->bp_;
	(black_info->my_bitboards_)[1] = position_->bn_;
	(black_info->my_bitboards_)[2] = position_->bb_;
	(black_info->my_bitboards_)[3] = position_->br_;
	(black_info->my_bitboards_)[4] = position_->bq_;
	(black_info->my_bitboards_)[5] = position_->bk_;

	for (int idx = 0; idx < 6; idx++) {
		white_info->my_occupied_ |= (white_info->my_bitboards_)[idx];
	}

	for (int idx = 0; idx < 6; idx++) {
		black_info->my_occupied_ |= (black_info->my_bitboards_)[idx];
	}

	white_info->all_occupied_ = white_info->my_occupied_ | black_info->my_occupied_;
	black_info->all_occupied_ = white_info->all_occupied_;
}

void GeneratePositions::ResetChildPositionInfo() const {
	*child_white_info_ = *parent_white_info_;
	*child_black_info_ = *parent_black_info_;
}

// "opponent" refers to the opponent of the person that is taking the turn 
// in position_
/*
IMPORTANT NOTE : purposely left out "& opponent_occupied" so that PawnAttacks can be used to find 
positions in check. When adding pawn moves to children, I MUST "& opponent_occupied"
*/
uint64_t GeneratePositions::PawnAttacks(uint64_t curr_pawns, PositionInfo* pos_info) const {
	/*uint64_t opponent_occupied = white_occupied_;
	if (my_occupied == white_occupied_) {
		opponent_occupied = black_occupied_;
	}*/
	// delete above code after coding the addition of pawn moves to children

	uint64_t left_captures = curr_pawns << 7 & ~H_FILE_;
	uint64_t right_captures = curr_pawns << 9 & ~A_FILE_;

	return (left_captures | right_captures) & ~(pos_info->my_occupied_);
}


//"my" is in reference to the person taking the turn in position_
uint64_t GeneratePositions::KnightAttacks(int knight_idx, PositionInfo* pos_info) const {
	uint64_t knight_attacks = 0;
	if (knight_idx > 18) {
		knight_attacks = KNIGHT_SPAN_ << knight_idx - 18;
	}
	else {
		knight_attacks = KNIGHT_SPAN_ >> 18 - knight_idx;
	}

	if (knight_idx % 8 > 3) {
		knight_attacks &= ~AB_FILES_;
	}
	else {
		knight_attacks &= ~GH_FILES_;
	}

	return knight_attacks & ~(pos_info->my_occupied_);
}

uint64_t GeneratePositions::BishopAttacks(int bishop_idx, PositionInfo* pos_info) const {
	int s = bishop_idx;
	uint64_t occupied = pos_info->all_occupied_;
	uint64_t binary_s = 1ULL << bishop_idx;
	uint64_t diagonal = (((occupied & Mask(Direction::Diag, s))
		- (2 * binary_s)) ^ R(R(occupied & Mask(Direction::Diag, s))
		- (2 * R(binary_s)))) & Mask(Direction::Diag, s);
	uint64_t anti_diagonal = (((occupied & Mask(Direction::A_diag, s))
		- (2 * binary_s)) ^ R(R(occupied & Mask(Direction::A_diag, s))
		- (2 * R(binary_s)))) & Mask(Direction::A_diag, s);

	return (diagonal | anti_diagonal) & ~(pos_info->my_occupied_);
}

uint64_t GeneratePositions::RookAttacks(int rook_idx, PositionInfo* pos_info) const {
	int s = rook_idx;
	uint64_t occupied = pos_info->all_occupied_;
	uint64_t binary_s = 1ULL << rook_idx;
	uint64_t rank = ((occupied - 2 * binary_s) ^
		R(R(occupied) - 2 * R(binary_s))) & Mask(Direction::Rank, s);
	uint64_t file = (((occupied & Mask(Direction::File, s)) - (2 * binary_s))
		^ R(R(occupied & Mask(Direction::File, s)) - (2 * R(binary_s))))
		& Mask(Direction::File, s);

	return (rank | file) & ~(pos_info->my_occupied_);
}

uint64_t GeneratePositions::QueenAttacks(int queen_idx, PositionInfo* pos_info) const {
	return BishopAttacks(queen_idx, pos_info) | RookAttacks(queen_idx, pos_info);
}

uint64_t GeneratePositions::KingAttacks(PositionInfo* pos_info) const {
	uint64_t king_attacks = 0;
	unsigned long king_idx;
	_BitScanForward64(&king_idx, (pos_info->my_bitboards_)[5]);

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

	return king_attacks & ~(pos_info->my_occupied_);
}


uint64_t GeneratePositions::AttackSquares(PositionInfo* pos_info) const {
	uint64_t my_occupied = pos_info->my_occupied_;
	uint64_t all_pawn_attacks = PawnAttacks((pos_info->my_bitboards_)[0], pos_info);
	uint64_t all_knight_attacks = GeneratePieceAttacks(
		[this](int idx, PositionInfo* pos_info) { return this->KnightAttacks(idx, pos_info); },
		pos_info, (pos_info->my_bitboards_)[1]);
	uint64_t all_bishop_attacks = GeneratePieceAttacks(
		[this](int idx, PositionInfo* pos_info) { return this->BishopAttacks(idx, pos_info); },
		pos_info, (pos_info->my_bitboards_)[2]);
	uint64_t all_rook_attacks = GeneratePieceAttacks(
		[this](int idx, PositionInfo* pos_info) { return this->RookAttacks(idx, pos_info); },
		pos_info, (pos_info->my_bitboards_)[3]);
	uint64_t all_queen_attacks = GeneratePieceAttacks(
		[this](int idx, PositionInfo* pos_info) { return this->QueenAttacks(idx, pos_info); },
		pos_info, (pos_info->my_bitboards_)[4]);
	uint64_t all_king_attacks = KingAttacks(pos_info);
	return all_pawn_attacks | all_knight_attacks | all_bishop_attacks | all_rook_attacks |
		all_queen_attacks | all_king_attacks;
}

uint64_t GeneratePositions::GeneratePieceAttacks(
	std::function<uint64_t(int, PositionInfo* pos_info)> PieceAttacks,
	PositionInfo* pos_info, uint64_t piece_bitboard) const {

	uint64_t all_attacks = 0;
	unsigned long curr_bit_idx;
	while (piece_bitboard != 0) {
		_BitScanForward64(&curr_bit_idx, piece_bitboard);
		all_attacks |= PieceAttacks(curr_bit_idx, pos_info);
		piece_bitboard &= ~(1ULL << curr_bit_idx);
	}
	return all_attacks;
}


//TODO
void GeneratePositions::PawnGen() {
	
}

//TODO
void GeneratePositions::DiagCapture() {
	/*
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


	}*/
}

	}
}

