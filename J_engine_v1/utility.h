#pragma once

#include "libraries.h"

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
	bool im_white = true;
};