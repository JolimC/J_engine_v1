#pragma once

#include "libraries.h"
#include "position.h"
#include "error_handling.h"

enum class Direction {
	Diag, A_diag, File, Rank
};

enum class MyColor {
	White, Black
};

class GeneratePositions {
public:
	GeneratePositions(Position* position);


	/*
	TO DEALLOCATE:
		
	*/
	~GeneratePositions();
	
	//code for debugging purposes:
	friend class Testing;

//private:
	uint64_t UpdateWhiteOccupied();
	uint64_t UpdateBlackOccupied();
	uint64_t* UpdateParentBitboards(MyColor color);
	void UpdateChildBitboards(Position* child);
	uint64_t ComputeMyOccupied(MyColor color) const;


	uint64_t PawnAttacks(uint64_t curr_pawns, uint64_t my_occupied) const;
	uint64_t KnightAttacks(int knight_idx, uint64_t my_occupied) const;
	uint64_t BishopAttacks(int bishop_idx, uint64_t my_occupied) const;
	uint64_t RookAttacks(int rook_idx, uint64_t my_occupied) const;
	uint64_t QueenAttacks(int queen_idx, uint64_t my_occupied) const;
	uint64_t KingAttacks(MyColor color) const;
	uint64_t AttackSquares(MyColor color) const;
	

	uint64_t GeneratePieceAttacks(
		std::function<uint64_t(int, uint64_t)> PieceAttacks,
		uint64_t my_occupied, uint64_t piece_bitboard) const;

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
	const uint64_t white_occupied_;
	const uint64_t black_occupied_;
	const uint64_t occupied_;

	//refers to the PARENT bitboards
	const uint64_t* const parent_white_bb_;
	const uint64_t* const parent_black_bb_;
	const std::map<MyColor, const uint64_t* const> my_bitboards_ = {
		{MyColor::White, parent_white_bb_},
		{MyColor::Black, parent_black_bb_}
	};

	/*refers to the bitboards of the CHILD.These values
	* are different for each new position.
	* 
	* Note these are arrays of pointers to bitboards
	*/ 
	uint64_t* white_bitboards_[6];
	uint64_t* black_bitboards_[6];

	//masks
	const uint64_t A_FILE_ = 0x101010101010101ULL;
	const uint64_t H_FILE_ = 0x8080808080808080ULL;
	const uint64_t AB_FILES_ = 217020518514230019ULL;
	const uint64_t GH_FILES_ = 13889313184910721216ULL;

	//goes from rank 1 to 8
	const uint64_t RANKS_[8] = {0xFFULL, 0xFF00ULL, 0xFF0000ULL, 0xFF000000ULL, 
		0xFF00000000ULL, 0xFF0000000000ULL, 0xFF000000000000ULL, 0xFF00000000000000ULL};
	
	//goes from File A to File H
	const uint64_t FILES_[8] = {0x101010101010101ULL, 0x202020202020202ULL, 0x404040404040404ULL,
		0x808080808080808ULL, 0x1010101010101010ULL, 0x2020202020202020ULL, 0x4040404040404040ULL,
		0x8080808080808080ULL};

	/*
	Diagonal on the chessboard: starts southwest and ends northeast. It's opposite on the bitboard.
	Antidiagonal on the chessboard: starts northwest and ends southeast. It's opposite on the bitboard.

	Speaking from the bitboard perspective, the 0 idx for diagonals
	refers to the diagonal at the bottom left corner.
	The 0 idx for anti-diagonals refers to the diagonal at the bottom right corner.
	*/
	const uint64_t DIAGONALS_[15] = {0x80ULL, 0x8040ULL, 0x804020ULL, 0x80402010ULL, 0x8040201008ULL, 
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