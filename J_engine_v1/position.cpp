#include "position.h"

Position::Position(std::string fen) {
	ReadFen(fen);
	UpdateWhiteOccupied();
	UpdateBlackOccupied();
}

//note that the children_ set is not copied over
Position::Position(const Position& position) {
	wp_ = position.wp_;
	wn_ = position.wn_;
	wb_ = position.wb_;
	wr_ = position.wr_;
	wq_ = position.wq_;
	wk_ = position.wk_;
	bp_ = position.bp_;
	bn_ = position.bn_;
	bb_ = position.bb_;
	br_ = position.br_;
	bq_ = position.bq_;
	bk_ = position.bk_;

	white_occupied_ = position.white_occupied_;
	black_occupied_ = position.black_occupied_;

	white_to_move_ = position.white_to_move_;

	wq_castle_ = position.wq_castle_;
	wk_castle_ = position.wk_castle_;
	bq_castle_ = position.bq_castle_;
	bk_castle_ = position.bk_castle_;

	en_passant_sq_ = position.en_passant_sq_;

	halfmoves_ = position.halfmoves_;
	fullmoves_ = position.fullmoves_;
}

/*
Preconditions:
curr_idx is the index of a ' ' character


Notes:
IncrementCurrIdx() is used to both increment curr_idx and error check string size
curr_idx is a pointer to an int that is passed around several functions. It is incremented
in ReadFen, ReadCastlingRights, and other similar functions that deal with reading the 
FEN.

*/
void Position::ReadFen(std::string fen) {
	//SetPosition first reads in the location of the pieces, populating the bitboards
	//returns the idx that SetPosition ended on
	int* curr_idx = new int(SetPosition(fen));

	IncrementCurrIdx(fen, curr_idx);
	switch (fen.at(*curr_idx)) {
	case 'w':
		white_to_move_ = true;
		break;
	case 'b':
		white_to_move_ = false;
		break;
	default:
		Error::error("FEN: player turn unspecified");
	}

	IncrementCurrIdx(fen, curr_idx);
	ReadSpace(fen.at(*curr_idx), "FEN: invalid character following turn information");

	IncrementCurrIdx(fen, curr_idx);
	if (fen.at(*curr_idx) == '-') {
		IncrementCurrIdx(fen, curr_idx);
	}
	else {
		//this function takes care of incrementing curr_idx to the following ' ' character
		ReadCastlingRights(fen, curr_idx);
	}

	ReadSpace(fen.at(*curr_idx), "FEN: invalid character following castle information");

	IncrementCurrIdx(fen, curr_idx);
	ReadEPTarget(fen, curr_idx);

	IncrementCurrIdx(fen, curr_idx);
	ReadSpace(fen.at(*curr_idx), "FEN: invalid character following en passant information");

	IncrementCurrIdx(fen, curr_idx);
	ReadHalfmove(fen, curr_idx);
	
	ReadSpace(fen.at(*curr_idx), "FEN: invalid character following halfmove information");

	IncrementCurrIdx(fen, curr_idx);
	ReadFullmove(fen, curr_idx);

	/*
	I have not included error checking to ensure that the half and fullmove numbers are even possible
	*/

	delete curr_idx;
}

void Position::IncrementCurrIdx(const std::string &fen, int* curr_idx) const {
	if (++(*curr_idx) >= fen.size()) {
		Error::error("incomplete FEN string");
	}
}

void Position::ReadSpace(char curr_char, std::string error_info) const {
	if (curr_char != ' ') {
		Error::error(error_info);
	}
}

void Position::ReadCastlingRights(const std::string &fen, int* curr_idx) {
	std::set<char> castle_options({ 'K','Q','k','q' });
	while (fen.at(*curr_idx) != ' ') {
		switch (fen.at(*curr_idx)) {
		case 'K':
			if (castle_options.count('K')) {
				castle_options.erase('K');
				wk_castle_ = true;
			}
			else {
				Error::error("FEN: incorrect specification of castling rights - order");
			}
			break;
		case 'Q':
			if (castle_options.count('Q')) {
				castle_options.erase('K');
				castle_options.erase('Q');
				wq_castle_ = true;
			}
			else {
				Error::error("FEN: incorrect specification of castling rights - order");
			}
			break;
		case 'k':
			if (castle_options.count('k')) {
				castle_options.erase('K');
				castle_options.erase('Q');
				castle_options.erase('k');
				bk_castle_ = true;
			}
			else {
				Error::error("FEN: incorrect specification of castling rights - order");
			}
			break;
		case 'q':
			if (castle_options.count('q')) {
				castle_options.erase('K');
				castle_options.erase('Q');
				castle_options.erase('k');
				castle_options.erase('q');
				bq_castle_ = true;
			}
			else {
				Error::error("FEN: incorrect specification of castling rights - order");
			}
			break;
		default:
			Error::error("FEN: incorrect specification of castling rights");
		}
		IncrementCurrIdx(fen, curr_idx);
	}
}


void Position::ReadEPTarget(const std::string& fen, int* curr_idx) {
	if (fen.at(*curr_idx) == '-') {
		en_passant_sq_ = "null";
	}
	else {
		std::set<char> allowable_files({'a','b','c','d','e','f','g','h'});
		if (!allowable_files.count(fen.at(*curr_idx))) {
			Error::error("FEN: invalid file specified regarding target en passant square");
		}
		en_passant_sq_ = fen.at(*curr_idx);
		
		IncrementCurrIdx(fen, curr_idx);
		switch (fen.at(*curr_idx)) {
		case '3':
		case '6':
			en_passant_sq_.push_back(fen.at(*curr_idx));
			break;
		default:
			Error::error("FEN: invalid rank specified regarding target en passant square");
		}
	}
}

void Position::ReadHalfmove(const std::string& fen, int* curr_idx) {
	std::string move_count;
	do {
		if (!std::isdigit(fen.at(*curr_idx))) {
			Error::error("FEN: no halfmove count specified");
		}
		move_count.push_back(fen.at(*curr_idx));
		IncrementCurrIdx(fen, curr_idx);
	} while (fen.at(*curr_idx) != ' ');		// note that the while loop exits when curr_idx refers to a ' ' character

	halfmoves_ = std::stoi(move_count);	 
}

void Position::ReadFullmove(const std::string& fen, int* curr_idx) {
	std::string move_count;
	for (; *curr_idx < fen.size(); (*curr_idx)++) {
		if (!std::isdigit(fen.at(*curr_idx))) {
			Error::error("FEN: incorrect fullmove specification");
		}
		move_count.push_back(fen.at(*curr_idx));
	}

	fullmoves_ = std::stoi(move_count);
}


// this function only takes the first part of the FEN (the part that specifies the piece locations)
int Position::SetPosition(std::string fen) {
	int* squares_remaining = new int(63);
	int curr_idx = 0;
	bool slash_found = false;
	for (; curr_idx < fen.size(); curr_idx++) {
		char token = fen.at(curr_idx);
		if (token == ' ' && *squares_remaining == -1) {
			delete squares_remaining;
			return curr_idx;
		}
		if (*squares_remaining < 0) {
			Error::error("FEN: too many characters specified");
		}
		if (*squares_remaining != 63 && *squares_remaining % 8 == 7) {
			if (slash_found) {
				ReadNonSlash(squares_remaining, token);
				slash_found = false;
			}
			else if (token == '/') {
				slash_found = true;
			}
			else {
				Error::error("FEN: '/' missing");
			}
		} 
		else {
			ReadNonSlash(squares_remaining, token);
		}
	}
	delete squares_remaining;
	Error::error("FEN: incomplete fen");
	return 0;
}

void Position::ReadNonSlash(int* squares_remaining, char token) {
	if (std::isdigit(token)) {
		if (token == '0') {
			Error::error("FEN: invalid character 0");
		}
		*squares_remaining -= token - '0';
	}
	else {
		uint64_t piece_bitboard = 1ULL << 7 + *squares_remaining - 2 * (*squares_remaining % 8);	// derived from [7-(s%8)] + [s-(s%8)]
		switch (token) {
		case 'P':
			wp_ |= piece_bitboard;
			break;
		case 'B':
			wb_ |= piece_bitboard;
			break;
		case 'N':
			wn_ |= piece_bitboard;
			break;
		case 'R':
			wr_ |= piece_bitboard;
			break;
		case 'Q':
			wq_ |= piece_bitboard;
			break;
		case 'K':
			wk_ |= piece_bitboard;
			break;
		case 'p':
			bp_ |= piece_bitboard;
			break;
		case 'b':
			bb_ |= piece_bitboard;
			break;
		case 'n':
			bn_ |= piece_bitboard;
			break;
		case 'r':
			br_ |= piece_bitboard;
			break;
		case 'q':
			bq_ |= piece_bitboard;
			break;
		case 'k':
			bk_ |= piece_bitboard;
			break;
		default:
			Error::error("FEN: invalid character");
		}
		(*squares_remaining)--;
	}
}

void Position::UpdateWhiteOccupied() {
	white_occupied_ = wp_ | wn_ | wb_ | wr_ | wq_ | wk_;
}

void Position::UpdateBlackOccupied() {
	black_occupied_ = bp_ | bn_ | bb_ | br_ | bq_ | bk_;
}


//this returns an array containing the pieces. idx 0 is A8 and idx 63 is H1.
//this layout makes it easier to cout the board
char* Position::GeneratePositionDisplay() const {
	const int bitboards_size = 12;
	uint64_t bitboards[] = {wp_, wn_, wb_, wr_, wq_, wk_,bp_, bn_, bb_, br_, bq_, bk_};
	char* board_position = new char[64];
	char bp_idx = 0;
	for (int k = 56; k >= 0; k -= 8 ) {
		for (int file = 0; file < 8; file++) {
			bool piece_found = false;
			for (int bb_idx = 0; bb_idx < bitboards_size; bb_idx++) {
				if ((bitboards[bb_idx] & (1ULL << k + file)) != 0) {
					board_position[bp_idx] = bb_to_piece_map_.at(bb_idx);
					piece_found = true;
					break;
				}
			}
			if (!piece_found) {
				board_position[bp_idx] = '0';
			}
			bp_idx++;
		}
	}
	return board_position;
}


//cout the piece positions as described in the board_position array
void Position::DisplayPosition() const {
	char* board_position = GeneratePositionDisplay();
	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 8; c++) {
			std::cout << board_position[r * 8 + c];
		}
		std::cout << std::endl;
	}
}