#pragma once

#include "libraries.h"
#include "error_handling.h"

class Position {
public:
	Position(std::string fen);
	Position(const Position& position);

	friend class GeneratePositions;

	//public helper functions and variables for debugging purposes
	void DisplayPosition() const;
private:
	uint64_t wp_ = 0;
	uint64_t wn_ = 0;
	uint64_t wb_ = 0;
	uint64_t wr_ = 0;
	uint64_t wq_ = 0;
	uint64_t wk_ = 0;
	uint64_t bp_ = 0;
	uint64_t bn_ = 0;
	uint64_t bb_ = 0;
	uint64_t br_ = 0;
	uint64_t bq_ = 0;
	uint64_t bk_ = 0;

	bool white_to_move_ = true;

	bool wq_castle_ = false;
	bool wk_castle_ = false;
	bool bq_castle_ = false;
	bool bk_castle_ = false;

	// When checking for ep availability, check for "null"
	std::string en_passant_sq_ = "null";

	int halfmoves_ = 0;
	int fullmoves_ = 1;

	std::set<Position*> children_;
	
	void ReadFen(std::string fen);
	int SetPosition(std::string fen);
	void ReadNonSlash(int* squares_remaining, char token);
	void ReadSpace(char curr_char, std::string error_info) const;
	void IncrementCurrIdx(const std::string &fen, int* curr_idx) const;
	void ReadCastlingRights(const std::string &fen, int* curr_idx);
	void ReadEPTarget(const std::string& fen, int* curr_idx);
	void ReadHalfmove(const std::string& fen, int* curr_idx);
	void ReadFullmove(const std::string& fen, int* curr_idx);


	//private helper functions and variables for debugging purposes
	char* GeneratePositionDisplay() const;  // deletion of board_position never specified
	const std::map<int, char>bb_to_piece_map_ = 
		{{0, 'P'}, 
		{1, 'N'}, 
		{2, 'B'},
		{3, 'R'},
		{4, 'Q'}, 
		{5, 'K'}, 
		{6, 'p'}, 
		{7, 'n'}, 
		{8, 'b'},
		{9, 'r'},
		{10, 'q'},
		{11, 'k'},
		};
};