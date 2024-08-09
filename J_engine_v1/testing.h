#pragma once

#include "moves.h"

class Testing {
public:
	Testing();

	//stores many FEN strings and creates the corresponding GP obj
	void TestCases();


	//utility
	uint64_t MirrorBB(uint64_t bitboard);
	void PrintBitboard(uint64_t bitboard);
private:
	std::string fen_;
};