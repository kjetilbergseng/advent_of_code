#include "day10.h"
#include <fmt/core.h>
#include <numeric>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include "utilities.h"
#include <doctest/doctest.h>
#include <assert.h>
#include <fmt/ranges.h>
#include <functional>

bool operator==(std::vector<std::vector<char>> lhs, std::vector<std::vector<char>> rhs) {
	if (lhs.size() != rhs.size()) { return false; }
	for (int i = 0; i < rhs.size(); ++i) {
		if (lhs[i].size() != rhs[i].size()) { return false; }
		for (int j = 0; j < rhs[i].size(); ++j) {		
			if (rhs[i][j] != lhs[i][j]) {
				return false;
			}
		}
	}
	return true;
}

bool is_in_range(int row, int col, const std::vector<std::vector<char>>& input) {
	return (
		(row > -1) && (col > -1) &&
		(row < input.size()) &&
		(col < input[0].size())
		);
}

int occupied_adjacent(int row, int col, const std::vector<std::vector<char>>& input) {
	char sum = 0;
	for (int i = -1; i <= 1; ++i) {
		for (int j = -1; j <= 1; ++j) {
			if ((i != 0 || j != 0) && is_in_range(i + row, j + col, input)) {
				sum += (input[row + i][col + j] == '#');
			}
		}
	}
	return sum;
}

int occupied_seen(int row, int col, const std::vector<std::vector<char>>& input) {
	char sum = 0;
	for (int i = -1; i <= 1; ++i) {
		for (int j = -1; j <= 1; ++j) {
			int n = 1;
			while ((i != 0 || j != 0) && is_in_range(n*i + row, n*j+col, input)) {
				if (input[row + n * i][col + n * j] == '.') {
					++n;
				}
				else {
					sum += (input[row + n * i][col + n * j] == '#');
					break;
				}
			}
		}
	}
	return sum;
}

template<typename T>
std::vector<std::vector<char>> update_seats(const std::vector<std::vector<char>>& input, T count_seat_function, int seat_tolerance) {
	std::vector<std::vector<char>>output = input;
	for (int i = 0; i < input.size(); ++i) {
		for (int j = 0; j < input[i].size(); ++j) {
			int n=(input[i][j] == '.')?0: count_seat_function(i, j, input);
			if (input[i][j] == '#' && n > seat_tolerance) {
				output[i][j] = 'L';
			}
			else if (input[i][j] == 'L' && n == 0) {
				output[i][j] = '#';
			}	
		}
	}
	return output;
}

template<typename T>
long long solve(std::vector<std::vector<char>> input, T count_seat_function, int seat_tolerance) {
	while (true) {
		const auto output = update_seats(input, count_seat_function, seat_tolerance);
		if (input == output) {
			long long count = 0;
			for (const auto& i : output) {
				count+=std::count(i.cbegin(), i.cend(), '#');
			}
			return count;
		}
		input = output;
	}
}

TEST_CASE("test day11 part 1") {
	long long awnser = solve({
			{'L','.','L','L','.','L','L','.','L','L'},
			{'L','L','L','L','L','L','L','.','L','L'},
			{'L','.','L','.','L','.','.','L','.','.'},
			{'L','L','L','L','.','L','L','.','L','L'},
			{'L','.','L','L','.','L','L','.','L','L'},
			{'L','.','L','L','L','L','L','.','L','L'},
			{'.','.','L','.','L','.','.','.','.','.'},
			{'L','L','L','L','L','L','L','L','L','L'},
			{'L','.','L','L','L','L','L','L','.','L'},
			{'L','.','L','L','L','L','L','.','L','L'}
		}, 
		occupied_adjacent, 
		3
	);
	CHECK(awnser == 37);
}

TEST_CASE("test day11 part 2") {
	long long awnser = solve({
			{'L','.','L','L','.','L','L','.','L','L'},
			{'L','L','L','L','L','L','L','.','L','L'},
			{'L','.','L','.','L','.','.','L','.','.'},
			{'L','L','L','L','.','L','L','.','L','L'},
			{'L','.','L','L','.','L','L','.','L','L'},
			{'L','.','L','L','L','L','L','.','L','L'},
			{'.','.','L','.','L','.','.','.','.','.'},
			{'L','L','L','L','L','L','L','L','L','L'},
			{'L','.','L','L','L','L','L','L','.','L'},
			{'L','.','L','L','L','L','L','.','L','L'}
		},
		occupied_seen,
		4
	);
	CHECK(awnser == 26);
}

TEST_CASE("solve day11") {
	const auto input = read_2D_array_of_chars("day11_input.txt");
	SUBCASE("part 1") {
		const auto awnser = solve(input, occupied_adjacent, 3);
		fmt::print(fg(fmt::color::pale_golden_rod), "11-1 awnser: {}\n", awnser);
		CHECK(awnser == 2289);
	}
	SUBCASE("part 2") {
		const auto awnser = solve(input, occupied_seen, 4);
		fmt::print(fg(fmt::color::pale_golden_rod), "11-2 awnser: {}\n", awnser);
		CHECK(awnser == 2059);
	}
}
