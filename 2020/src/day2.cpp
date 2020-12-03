#include "day2.h"
#include <fmt/core.h>
#include <tuple>
#include "utilities.h"
#include <doctest/doctest.h>
#include<numeric>
enum class policy {
	part1,
	part2
};

std::tuple<int, int, char, std::string> parse_input(std::string input) {
	std::erase(input, ' ');
	int min = std::stoi(input.substr(0, input.find('-')));
	int max = std::stoi(input.substr(input.find('-')+1, input.find(':')- input.find('-')-2));
	char letter = input.substr(input.find(':') - 1, 1).front();
	std::string str= input.substr(input.find(':')+1, input.size() - input.find(':')-1);
	
	return { min,max,letter,str };
}

bool is_valid_password(int min, int max, char c, const std::string& password, policy pol) {
	switch (pol)
	{
	case policy::part1: {
		int n = 0;
		for (const char& pw:password) { n += (pw == c); }
		return (n >= min && n <= max);
	}
	case policy::part2:
		return ((password[min - 1] == c) ^ (password[max - 1] == c));
	default:
		throw std::runtime_error("invalid policy");
	}

}

int solve_day2(const std::vector<std::string>& input, policy pol) {
	int valid = 0;
	for (const auto &i:input){
		auto [min, max, letter, password]=parse_input(i);
		valid += is_valid_password(min, max, letter, password, pol);
	}
	return valid;
}

TEST_CASE("solve day2 part 1") {
	int awnser = solve_day2(read_file_to_vector_of_strings("day2_input.txt"), policy::part1);
	fmt::print(fg(fmt::color::pale_golden_rod), "2-1 awnser: {}\n", awnser);
}

TEST_CASE("solve day2 part 1") {
	int awnser = solve_day2(read_file_to_vector_of_strings("day2_input.txt"), policy::part2);
	fmt::print(fg(fmt::color::pale_golden_rod), "2-2 awnser: {}\n", awnser);
}

TEST_CASE("test day2 part 1") {
	int awnser = solve_day2({
		"1 - 3 a: abcde",
		"1 - 3 b : cdefg",
		"2 - 9 c : ccccccccc" }, policy::part1);
	CHECK(awnser == 2);
}

TEST_CASE("test day2 part 2") {
	int awnser = solve_day2({
		"1 - 3 a: abcde",
		"1 - 3 b : cdefg",
		"2 - 9 c : ccccccccc" }, policy::part2);
	CHECK(awnser == 1);
}

TEST_CASE("parse input") {
	{
		auto [min, max, letter, password] = parse_input("1 - 3 a: abcde");
		CHECK(min == 1);
		CHECK(max == 3);
		CHECK(letter == 'a');
		CHECK(password == "abcde");
	}
	{
		auto [min, max, letter, password] = parse_input("1-3 b: cdefg");
		CHECK(min == 1);
		CHECK(max == 3);
		CHECK(letter == 'b');
		CHECK(password == "cdefg");
	}
}

TEST_CASE("test is_valid_password part1") {
	{
		bool is_valid = is_valid_password(1, 3, 'a', "abcde", policy::part1);
		CHECK(is_valid == true);
	}
	{
		bool is_valid = is_valid_password(1, 3, 'b', "cdefg", policy::part1);
		CHECK(is_valid == false);
	}
	{
		bool is_valid = is_valid_password(2, 9, 'c', "ccccccccc", policy::part1);
		CHECK(is_valid == true);
	}
}

TEST_CASE("test is_valid_password part2") {
	{
		bool is_valid = is_valid_password(1, 3, 'a', "abcde", policy::part2);
		CHECK(is_valid == true);
	}
	{
		bool is_valid = is_valid_password(1, 3, 'b', "cdefg", policy::part2);
		CHECK(is_valid == false);
	}
	{
		bool is_valid = is_valid_password(2, 9, 'c', "ccccccccc", policy::part2);
		CHECK(is_valid == false);
	}
}

