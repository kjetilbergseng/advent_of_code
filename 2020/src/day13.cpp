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
	size_t pos;
	size_t val;
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

size_t solve_day_13_part2(const std::vector<Vec2>& busses) {
	size_t n = 1, dn = 1, i_curr = 0;
	while (true) {
		const size_t x = busses[0].val * n;
		for (size_t i = 1; i < busses.size();++i) {
			if ((x + busses[i].pos- busses[0].pos) % busses[i].val != 0) {
				break;
			}
			else {
				if (i == busses.size() - 1) {
					return x - busses[0].pos;
				}
				else if (i_curr < i) {
					i_curr++;
					dn = std::lcm(dn, busses[i_curr].val);
				}
			}
		}
		n += dn;
	};
	throw std::runtime_error("failed to find solution to day 13 part 2");
}


std::vector<Vec2> get_input_vector(std::vector<std::string> vec) {
	std::vector<Vec2> busses;
	size_t i = 0;
	std::transform(vec.cbegin(), vec.cend(), std::back_inserter(busses),
		[&i](const std::string& str) { ++i;  return (str == "x") ? Vec2{ i-1,0ull } : Vec2{ i-1, std::stoull(str) }; });
	std::erase_if(busses, [](const Vec2& bus) {return bus.val == 0; });
	std::sort(busses.begin(), busses.end(), std::greater<Vec2>());

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

