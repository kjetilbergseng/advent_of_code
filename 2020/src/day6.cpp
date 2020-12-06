#include "day6.h"
#include <fmt/core.h>
#include <tuple>
#include "utilities.h"
#include <doctest/doctest.h>
#include<numeric>
#include <assert.h> 

int number_of_uniques(std::string str)
{
	std::sort(str.begin(), str.end());
	int awnser = (str.size() > 0);
	for (size_t i = 1; i < str.size(); ++i) {
		awnser += (str[i] != str[i - 1ui64]);
	}
	return awnser;
}

int solve_day6_part_1(const std::vector<std::string>& inputs) {
	int awnser=0;
	for (std::string str; const auto & input : inputs) {
		if (input != "") {
			str += input;
		}
		if (input == "" || input==*(inputs.cend()-1)) {
			awnser+=number_of_uniques(str);
			str = "";
		}
	}
	return awnser;
}

int find_number_of_whole_group_awnsers(std::string str, const int group_size)
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


int solve_day6_part_2(std::vector<std::string> inputs) {
	int awnser = 0;
	inputs.push_back("");
	int group_size=0;
	for (std::string str; const auto & input : inputs) {
		if (input == "") {
			awnser+=find_number_of_whole_group_awnsers(str, group_size);
			str = "";
			group_size = 0;
		}
		else {
			++group_size;
			str += input;
		}
	}
	return awnser;
}

TEST_CASE("solve day6 part 1") {
	const auto input = read_file_to_vector_of_strings("day6_input.txt");
	const auto awnser = solve_day6_part_1(input);
	fmt::print(fg(fmt::color::pale_golden_rod), "6-1 awnser: {}\n", awnser);
	CHECK(awnser == 6947);
}

TEST_CASE("solve day6 part 2") {
	const auto input = read_file_to_vector_of_strings("day6_input.txt");
	const auto awnser = solve_day6_part_2(input);
	fmt::print(fg(fmt::color::pale_golden_rod), "6-2 awnser: {}\n", awnser);
	CHECK(awnser == 3398);
}


TEST_CASE("solve day6 part 1") {
	const std::vector<std::string> input =
	{ "abc","","a","b","c","","ab","ac","","a","a","a","a","","b" };

	const auto awnser = solve_day6_part_1(input);

	CHECK(awnser == 11);
}

TEST_CASE("solve day6 part 1") {
	const std::vector<std::string> input =
	{ "abc","","a","b","c","","ab","ac","","a","a","a","a","","b" };

	const auto awnser = solve_day6_part_2(input);

	CHECK(awnser == 6);
}