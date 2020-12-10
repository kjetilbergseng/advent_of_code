#include "day10.h"
#include <fmt/core.h>
#include <numeric>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include "utilities.h"
#include <doctest/doctest.h>
#include <assert.h> 

long long solve_day10_part1(std::vector<int> input) {
	std::sort(input.begin(), input.end());
	std::adjacent_difference(input.begin(), input.end(), input.begin());
	return  std::count(input.begin(), input.end(), 1) * (std::count(input.begin(), input.end(), 3) + 1);
}

TEST_CASE("test day10 part 1") {
	long long awnser = solve_day10_part1(
		{ 16,10,15,5,1,11,7,19,6,12,4 });
	CHECK(awnser == 7*5);
}

TEST_CASE("test day10 part 2") {
	long long awnser = solve_day10_part1(
		{ 28,33,18,42,31,14,46,20,48,47,24,23,49,45,19,38,39,11,1,32,25,35,8,17,7,9,4,2,34,10,3});
	CHECK(awnser == 220);
}

TEST_CASE("solve day9") {
	auto input = read_file_to_vector_of_ints("day10_input.txt");

	long long awnser = solve_day10_part1(input);
	fmt::print(fg(fmt::color::pale_golden_rod), "10-1 awnser: {}\n", awnser);
	//CHECK(awnser == 776203571);
}

