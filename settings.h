#pragma once

namespace settings {
	extern bool universumBeenCreated;
	extern bool programShouldWork;
	extern bool programShouldEnd;
}

namespace max_variables_sizes {
	const long long LongValue = 9223372036854775807;

	const char CharIntValue[] = "2147483647";
	const char CharMinIntValue[] = "-2147483648";

	const int IntValue = 2147483647;
	const int MinIntValue = -2147483648;

	const double DoubleValue = 1.7E+308;
	const double MinDoubleValue = -1.7E+308;
}

namespace buffer_sizes {
	const int string_size = 16;
	const int n_int_size = 6;
	const long long power_int_size = 12;
	const int multipicity_int_size = 5;
	const int char_size = 2;
	const int file_size = 20;
}