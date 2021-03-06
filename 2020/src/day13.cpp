#include "day13.h"
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <doctest/doctest.h>
#include <numeric>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include "utilities.h"
#include <assert.h>
#include <limits>
#include <cmath>

struct Vec2 {
	uint64_t pos;
	uint64_t val;
};
bool operator< (const Vec2& lhs, const Vec2& rhs) { return lhs.val < rhs.val; }
bool operator> (const Vec2& lhs, const Vec2& rhs) { return rhs < lhs; }
bool operator<=(const Vec2& lhs, const Vec2& rhs) { return !(lhs > rhs); }
bool operator>=(const Vec2& lhs, const Vec2& rhs) { return !(lhs < rhs); }

int solve_day_13_part1(int target, std::vector<int> busses) {
	int min = std::numeric_limits<int>::max();
	int id=0;
	for (int i : busses) {
		int t = (i - target % i);
		if (t < min) {
			min = t;
			id = i;
		}
	}
	return min*id;
}

uint64_t solve_day_13_part2(const std::vector<Vec2>& busses) {
	uint64_t n = 1, dn = 1, i = 0, x = 0;
	while (i< busses.size()-1) {
		x = busses[0].val * n;
		if ((x + busses[i + 1].pos- busses[0].pos) % busses[i + 1].val == 0) {
			++i;
			dn = std::lcm(dn, busses[i].val);
		}
		n += dn;
	};
	return x - busses[0].pos;
}
 
std::vector<Vec2> get_input_vector(std::vector<std::string> vec) {
	std::vector<Vec2> busses;
	std::transform(vec.cbegin(), vec.cend(), std::back_inserter(busses),
		[i = 0ull ](const std::string& str) mutable { 
		++i;  
		return (str == "x") ? Vec2{ i-1,0ull } : Vec2{ i-1, std::stoull(str) }; 
	});
	std::erase_if(busses, [](const Vec2& bus) {return bus.val == 0; });

	return busses;
}

TEST_CASE("test day13 part 1") {
	auto awnser = solve_day_13_part1(939, { 7,13,59,31,19 });
	CHECK(awnser == 295);
}

TEST_CASE("test day13 part 2") {
	SUBCASE("test1") {
		std::vector<std::string> inp{ "67","7","59","61" };
		auto awnser = solve_day_13_part2( get_input_vector(inp));
		CHECK(awnser == 754018);
	}
	SUBCASE("test2") {
		std::vector<std::string> inp{ "7","13","x","x","59","x","31","19" };
		auto awnser = solve_day_13_part2(get_input_vector(inp));
		CHECK(awnser == 1068781);
	}
}

TEST_CASE("solve day13") {
	auto input = read_file_to_vector_of_strings("day13_input.txt");
	SUBCASE("part1") {		
		auto vec = split_string(input[1], std::regex{ "," });
		auto it = std::remove_if(vec.begin(), vec.end(), [](const std::string& str) {return str == "x"; });
		std::vector<int> busses;
		std::transform(vec.begin(), it, std::back_inserter(busses), [](const std::string& str) {return std::stoi(str); });
		const auto awnser = solve_day_13_part1(std::stoi(input[0]), busses);
		fmt::print(fg(fmt::color::pale_golden_rod), "13-1 awnser: {}\n", awnser);
		CHECK(awnser == 5946);
	}
	SUBCASE("part2") {
		auto vec = split_string(input[1], std::regex{ "," });
		const auto awnser = solve_day_13_part2(get_input_vector(vec));
		fmt::print(fg(fmt::color::pale_golden_rod), "13-2 awnser: {}\n", awnser);
		CHECK(awnser == 645338524823718);
	}
}

