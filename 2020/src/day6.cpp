#include "day6.h"
#include <fmt/core.h>
#include <tuple>
#include "utilities.h"
#include <doctest/doctest.h>
#include<numeric>
#include <assert.h> 

size_t solve_day6_part_1(const std::vector<std::string>& inputs) {
	size_t awnser=0;
	for (auto input : inputs) {
		std::sort(input.begin(), input.end());
		awnser += std::distance(input.begin(), std::unique(input.begin(), input.end()));
	}
	return awnser;
}

int count_all_yes(std::string str, const size_t group_size)
{
	std::sort(str.begin(), str.end());
	int awnser = 0, num_awnser = 0;
	for (size_t i = 0; i < str.size(); ++i) {
		++num_awnser;
		awnser += (num_awnser == group_size);
		num_awnser *= (i + 1ui64 < str.size()) && (str[i] == str[i + 1ui64]);
	}
	return awnser;
}

int solve_day6_part_2(std::vector< std::vector<std::string>> groups) {
	int awnser = 0;
	for (const auto & group : groups) {
		std::string str = std::accumulate(group.cbegin(), group.cend(), std::string{ "" });
		awnser+= count_all_yes(str, group.size());
	}
	return awnser;
}

TEST_CASE("solve day6 part 1") {
	const auto input = read_strings_split_on_empty_line("day6_input.txt");
	const auto awnser = solve_day6_part_1(input);
	fmt::print(fg(fmt::color::pale_golden_rod), "6-1 awnser: {}\n", awnser);
	CHECK(awnser == 6947);
}

TEST_CASE("solve day6 part 2") {
	const auto input = read_groups_of_vectors_split_on_empty_line("day6_input.txt");
	const auto awnser = solve_day6_part_2(input);
	fmt::print(fg(fmt::color::pale_golden_rod), "6-2 awnser: {}\n", awnser);
	CHECK(awnser == 3398);
}


TEST_CASE("solve day6 part 1") {
	const std::vector<std::string> input =
	{ "abc","abc","abac","aaaa","b" };

	const auto awnser = solve_day6_part_1(input);

	CHECK(awnser == 11);
}

TEST_CASE("solve day6 part 1") {
	const std::vector< std::vector<std::string>> input =
	{ {"abc"},{"a","b","c"},{"ab","ac"},{"a","a","a","a"},{"b"} };

	const auto awnser = solve_day6_part_2(input);

	CHECK(awnser == 6);
}