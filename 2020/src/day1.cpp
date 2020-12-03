#include "day1.h"
#include <fmt/core.h>
#include <vector>
#include <stdexcept>
#include "utilities.h"
#include <doctest/doctest.h>
#include <assert.h> 

int solve_day1_part1(const std::vector<int>& input, const int target=2020) {
	int len = static_cast<int>(input.size());
	for (int i=0; i< len -1;++i){
		for (int j = i+1; j < len; ++j) {
			if (input[i] + input[j] == target) {return input[i] * input[j];}
		}
	}
	throw std::runtime_error("failed to find solution for day1 part 1");
}

int solve_day1_part2(const std::vector<int>& input, const int target=2020) {
	int len = static_cast<int>(input.size());
	for (int i = 0; i < len - 2; ++i) {
		for (int j = i + 1; j < len -1; ++j) {
			for (int k = j + 1; k < len; ++k) {
				if (input[i] + input[j] + input[k] == target) {	return input[i] * input[j]* input[k];}
			}
		}
	}
	throw std::runtime_error("failed to find solution for day1 part 2");
}

TEST_CASE("test day1 part 1") {
	{
		int awnser = solve_day1_part1(
			{ 1721,979,366,299,675,1456 });
		CHECK(awnser == 514579);
	}
	{
		int awnser = solve_day1_part1(
			{ 1721,979,1010,366,299,675,1456 });
		CHECK(awnser == 514579);
	}
}

TEST_CASE("test day1 part 2") {
	int awnser = solve_day1_part2(
		{ 1721,979,366,299,675,1456 });
	CHECK(awnser == 241861950);
}

TEST_CASE("solve day1") {
	{
		int awnser = solve_day1_part1(
			read_file_to_vector_of_ints("day1_input.txt"));
		fmt::print(fg(fmt::color::pale_golden_rod), "1-1 awnser: {}\n", awnser);
	}
	{
		int awnser = solve_day1_part2(
			read_file_to_vector_of_ints("day1_input.txt"));
		fmt::print(fg(fmt::color::pale_golden_rod), "1-2 awnser: {}\n", awnser);
	}
}