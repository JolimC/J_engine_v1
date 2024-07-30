#pragma once

#include "libraries.h"

struct Error {
	static void error(std::string s) {
		std::cerr << s;
		throw std::runtime_error(s);
	}
};