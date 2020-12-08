#include "day7.h"
#include <fmt/core.h>
#include <tuple>
#include "utilities.h"
#include <doctest/doctest.h>
#include<numeric>
#include <assert.h> 
#include "day8.h"
struct Vec2 {
	int pos;
	int acc;
};
Vec2& operator+= (Vec2& lhs, const Vec2& rhs) {
	lhs.pos += rhs.pos;
	lhs.acc += rhs.acc;
	return lhs;
}

Vec2 handle_instruction(const std::string& instruction, const int value)
{
	if (instruction == "jmp") { return { value,0 }; }
	else if (instruction == "acc") { return { 1,value }; }
	else { return { 1,0 }; }
}

bool out_of_range(const int pos, const size_t size) {
	return (pos > size+1 || pos < 0);
}
bool infinite_loop(const std::vector<int>& executed, const int pos) {
	return (std::find(executed.cbegin(), executed.cend(), pos) != executed.cend());
}

std::tuple<bool, int> run_program(const std::vector<std::tuple<std::string, int>>& input){
	Vec2 v {0, 0};
	std::vector<int> executed;
	while (true) {
		if (v.pos == input.size()) {
			return { true, v.acc };
		}
		if (out_of_range(v.pos, input.size()) || infinite_loop(executed, v.pos)){
			return { false, v.acc };
		}
		const auto [instruction, value] = input[v.pos];
		executed.push_back(v.pos);
		v += handle_instruction(instruction,value);
	}
}

int solve_day8_part_2(std::vector<std::tuple<std::string, int>>& input) {
	while (true) {
		for (auto& [instruction, value] : input) {
			if (instruction == "jmp") {
				instruction = "nop";
				auto [success, acc]=run_program(input);
				if (success) { return acc; };
				instruction = "jmp";
			}
			if (instruction == "nop") {
				instruction = "jmp";
				auto [success, acc] = run_program(input);
				if (success) { return acc; };
				instruction = "nop";
			}
		}
	}
}

std::vector<std::tuple<std::string, int>> read_input(const char* filename) {
	std::vector<std::tuple<std::string, int>> v;
	FileManager fm(filename);
	for (std::string line; std::getline(fm.get(), line); ) {
		std::erase(line, '\r');
		auto u = split_string(line, std::regex{ " " });
		v.emplace_back( u[0], std::stoi(u[1]));
	}
	return v;
}

TEST_CASE("solve day8 part 1") {
	const auto input = read_input("day8_input.txt");
	const auto [success, acc] = run_program(input);
	fmt::print(fg(fmt::color::pale_golden_rod), "8-1 awnser: {}\n", acc);
	CHECK(acc == 1766);
}

TEST_CASE("solve day8 part 2") {
	auto input = read_input("day8_input.txt");
	const auto awnser = solve_day8_part_2(input);
	fmt::print(fg(fmt::color::pale_golden_rod), "8-2 awnser: {}\n", awnser);
	CHECK(awnser == 1639);
}

TEST_CASE("test day8") {
	std::vector<std::tuple<std::string, int>> input = {
		{"nop", 0},
		{"acc", 1},
		{"jmp", 4},
		{"acc", 3 },
		{"jmp", -3},
		{"acc", -99},
		{"acc", 1},
		{"jmp", -4},
		{"acc", 6}
	};
	SUBCASE("part 1") {
		const auto [success, acc] = run_program(input);
		CHECK(acc == 5);
	}
	SUBCASE("part 2") {
		const auto acc = solve_day8_part_2(input);
		CHECK(acc == 8);
	}
}
