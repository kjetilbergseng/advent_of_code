#include "day15.h"
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <doctest/doctest.h>
#include <numeric>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include "utilities.h"
#include <unordered_map >
#include <bitset>
#include <cmath>

uint64_t solve_day15(std::vector<uint64_t> input, uint64_t repeat_times) {
	std::unordered_map<uint64_t, uint64_t> map;
	for (uint64_t i = 1; i < input.size();++i) {
		map[input[i-1]] =i;
	}

	uint64_t last = input[input.size()-1];
	for (auto i = input.size(); i < repeat_times; ++i) {
		if (map.count(last) > 0) {
			uint64_t next= i - map[last];
			map[last] = i;
			last = next;
		}
		else {
			map[last] = i;
			last = 0;
		}	
	}
	return last;
}

TEST_CASE("solve day15") {
	const std::vector<uint64_t> input = { 8,11,0,19,1,2 };
	SUBCASE("part1") {		
		const auto awnser = solve_day15(input, 2020);
		fmt::print(fg(fmt::color::pale_golden_rod), "15-1 awnser: {}\n", awnser);
		CHECK(awnser == 447);
	}
	SUBCASE("part2") {
		const auto awnser = solve_day15(input, 30000000);
		fmt::print(fg(fmt::color::pale_golden_rod), "15-2 awnser: {}\n", awnser);
		CHECK(awnser == 11721679);
	}
}

TEST_CASE("test day15 part1") {
	const std::vector<uint64_t> input = {0,3,6};
	const auto awnser = solve_day15(input, 2020);
	CHECK(awnser == 436);
}


