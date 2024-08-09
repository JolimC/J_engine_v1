#pragma once

#include "libraries.h"
#include "position.h"
#include "error_handling.h"

enum class Direction {
	Diag, A_diag, File, Rank
};

enum class PositionType {
	Parent, Child
};


struct PositionInfo {
	uint64_t my_bitboards_[6];
	uint64_t my_occupied_ = 0;
	uint64_t all_occupied_ = 0;
};

class GeneratePositions {
public:
	GeneratePositions(Position* position);


	/*
	TO DEALLOCATE:
	PositionInfo objects created for parent_info_ and child_info_
	*/
	~GeneratePositions();

	//code for debugging purposes:
	friend class Testing;

	//private:

		//called to initialize the values of the parent_info_ and child_info_ maps
	void InitializePositionInfo(PositionType type) const;
	void ResetChildPositionInfo() const;
	void UpdateChildOccupied() const;		//TODO

	uint64_t PawnAttacks(uint64_t curr_pawns, PositionInfo* pos_info) const;
	uint64_t KnightAttacks(int knight_idx, PositionInfo* pos_info) const;
	uint64_t BishopAttacks(int bishop_idx, PositionInfo* pos_info) const;
	uint64_t RookAttacks(int rook_idx, PositionInfo* pos_info) const;
	uint64_t QueenAttacks(int queen_idx, PositionInfo* pos_info) const;
	uint64_t KingAttacks(PositionInfo* pos_info) const;
	uint64_t AttackSquares(PositionInfo* pos_info) const;

	uint64_t GeneratePieceAttacks(
		std::function<uint64_t(int, PositionInfo* pos_info)> PieceAttacks,
		PositionInfo* pos_info, uint64_t piece_bitboard) const;


	void PawnGen();
	void DiagCapture();

	// reverses bits in a uint64_t. 
	// I used a simple name to keep the formula compact
	uint64_t R(uint64_t original_bitboard) const;

	uint64_t Mask(Direction direction, int piece_idx) const;



	//the tree of Position objects will be dynamically allocated
	Position* const position_;

	const bool white_to_move_;

	//refers to the occupied squares of the parent: "position"
	/*
	TO DELETE
	after adding the struct, this is unnecessary:
	const uint64_t white_occupied_;
	const uint64_t black_occupied_;
	const uint64_t occupied_;*/

	//refers to the PARENT bitboards
	//note that the values pointed to should not be changed after initializing
	PositionInfo* const parent_white_info_ = new PositionInfo;
	PositionInfo* const parent_black_info_ = new PositionInfo;
	const std::map<bool, const PositionInfo* const> parent_info_ = {
		{true, parent_white_info_},
		{false, parent_black_info_}
	};

	/*refers to the bitboards of the CHILD.These values
	* are different for each new position.
	*/
	PositionInfo* const child_white_info_ = new PositionInfo;
	PositionInfo* const child_black_info_ = new PositionInfo;
	const std::map<bool, PositionInfo* const> child_info_ = {
		{true, child_white_info_},
		{false, child_black_info_}
	};

	//masks
	const uint64_t A_FILE_ = 0x101010101010101ULL;
	const uint64_t H_FILE_ = 0x8080808080808080ULL;
	const uint64_t AB_FILES_ = 217020518514230019ULL;
	const uint64_t GH_FILES_ = 13889313184910721216ULL;

	//goes from rank 1 to 8
	const uint64_t RANKS_[8] = { 0xFFULL, 0xFF00ULL, 0xFF0000ULL, 0xFF000000ULL,
		0xFF00000000ULL, 0xFF0000000000ULL, 0xFF000000000000ULL, 0xFF00000000000000ULL };

	//goes from File A to File H
	const uint64_t FILES_[8] = { 0x101010101010101ULL, 0x202020202020202ULL, 0x404040404040404ULL,
		0x808080808080808ULL, 0x1010101010101010ULL, 0x2020202020202020ULL, 0x4040404040404040ULL,
		0x8080808080808080ULL };

	/*
	Diagonal on the chessboard: starts southwest and ends northeast. It's opposite on the bitboard.
	Antidiagonal on the chessboard: starts northwest and ends southeast. It's opposite on the bitboard.

	Speaking from the bitboard perspective, the 0 idx for diagonals
	refers to the diagonal at the bottom left corner.
	The 0 idx for anti-diagonals refers to the diagonal at the bottom right corner.
	*/
	const uint64_t DIAGONALS_[15] = { 0x80ULL, 0x8040ULL, 0x804020ULL, 0x80402010ULL, 0x8040201008ULL,
		0x804020100804ULL, 0x80402010080402ULL, 0x8040201008040201ULL, 0x4020100804020100ULL,
		0x2010080402010000ULL, 0x1008040201000000ULL, 0x804020100000000ULL,
		0x402010000000000ULL, 0x201000000000000ULL, 0x100000000000000ULL
	};
	const uint64_t ANTI_DIAGONALS_[15] = { 0x1ULL, 0x102ULL, 0x10204ULL, 0x1020408ULL,
		0x102040810ULL, 0x10204081020ULL, 0x1020408102040ULL, 0x102040810204080ULL,
		0x204081020408000ULL, 0x408102040800000ULL, 0x810204080000000ULL, 0x1020408000000000ULL,
		0x2040800000000000ULL, 0x4080000000000000ULL, 0x8000000000000000ULL
	};

	const uint64_t KNIGHT_SPAN_ = 43234889994ULL;
	const uint64_t KING_SPAN_ = 460039ULL;
};