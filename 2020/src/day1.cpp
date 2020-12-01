#include "day1.h"
#include <fmt/core.h>
#include <doctest/doctest.h>
#include <vector>
#include <stdexcept>
#include "utilities.h"

int solve_day1_part1(std::vector<int> vec) {
	for (size_t i=0; i<vec.size()-1;++i){
		for (size_t j = i+1; j < vec.size(); ++j) {
			if (vec[i] + vec[j] == 2020) {return vec[i] * vec[j];}
		}
	}
	throw std::runtime_error("failed to find solution for day1 part 1");
}

int solve_day1_part2(std::vector<int> vec) {
	for (size_t i = 0; i < vec.size() - 2; ++i) {
		for (size_t j = i + 1; j < vec.size()-1; ++j) {
			for (size_t k = j + 1; k < vec.size(); ++k) {
				if (vec[i] + vec[j] + vec[k] == 2020) {	return vec[i] * vec[j]* vec[k];}
			}
		}
	}
	throw std::runtime_error("failed to find solution for day1 part 2");
}

TEST_CASE("test day1 part 1") {
	int awnser = solve_day1_part1(
		{1721,979,366,299,675,1456 });
	CHECK(awnser == 514579);
}

TEST_CASE("test day1 part 2") {
	int awnser = solve_day1_part2(
		{ 1721,979,366,299,675,1456 });
	CHECK(awnser == 241861950);
}

TEST_CASE("solve day1") {
	{
		int awnser = solve_day1_part1(
			read_file_to_vector("input.txt"));
		fmt::print(fg(fmt::color::pale_golden_rod), "1-1 awnser: {}\n", awnser);
	}
	{
		int awnser = solve_day1_part2(
			read_file_to_vector("input.txt"));
		fmt::print(fg(fmt::color::pale_golden_rod), "1-2 awnser: {}\n", awnser);
	}
}