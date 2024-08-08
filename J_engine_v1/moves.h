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

	const uint64_t KNIGHT_SPAN_ = 43234889994ULL;
};