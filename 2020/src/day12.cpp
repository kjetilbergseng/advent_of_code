#include "day12.h"
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <doctest/doctest.h>
#include <numeric>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include "utilities.h"
#include <assert.h>
#include <cmath>

constexpr double pi() { return 3.14159265358979323846; }

template <typename T>
concept Ship =
requires(T & a, int b)
{
	{a.move_forward(b)};
	{a.rotate_left(b)};
	{a.rotate_right(b)};
	{a.move_east(b)};
	{a.move_north(b)};
	{a.move_west(b)};
	{a.move_south(b)};
};

struct Waypoint {
	double x;
	double y;
};

struct Ship1 {
	int x=0;
	int y=0;
	int rot=0;
	int manhattan_distance() {
		return abs(x) + abs(y);
	}
	void move_forward(int value) {
		x += static_cast<int>(std::cos(rot * pi() / 180)) * value;
		y += static_cast<int>(std::sin(rot * pi() / 180)) * value;
	}
	void rotate_left(int value) {
		rot += value;
	}
	void rotate_right(int value) {
		rot -= value;
	}
	void move_east(int value) {
		x += value;
	}
	void move_north(int value) {
		y += value;
	}
	void move_west(int value) {
		x -= value;
	}
	void move_south(int value) {
		y -= value;
	}
};

struct Ship2 {
	double x=0;
	double y=0;
	Waypoint wp{ 10,1 };
	double manhattan_distance() {
		return abs(x) + abs(y);
	}
	void move_forward(int value) {
		x += wp.x * value;
		y += wp.y * value;
	}

	void rotate_left(int value) {
		double angle = value * pi() / 180.0;
		double tmp_x = wp.x * std::cos(angle) - wp.y * sin(angle);
		wp.y = wp.x * std::sin(angle) + wp.y * cos(angle);
		wp.x = tmp_x;
	}
	void rotate_right(int value) {
		rotate_left(360 - value % 360);
	}
	void move_east(int value) {
		wp.x += value;
	}
	void move_north(int value) {
		wp.y += value;
	}
	void move_west(int value) {
		wp.x -= value;
	}
	void move_south(int value) {
		wp.y -= value;
	}
};

struct Instruction {
	char action;
	int value;
};

template<Ship S>
void handle_instruction(S& ship, const Instruction& ins) {
	if (ins.action == 'F') {
		ship.move_forward(ins.value);
	}
	else if (ins.action == 'R') {
		ship.rotate_right(ins.value);
	}
	else if (ins.action == 'L') {
		ship.rotate_left(ins.value);
	}
	else if (ins.action == 'E') {
		ship.move_east(ins.value);
	}
	else if (ins.action == 'N') {
		ship.move_north(ins.value);
	}
	else if (ins.action == 'W') {
		ship.move_west(ins.value);
	}
	else if (ins.action == 'S') {
		ship.move_south(ins.value);
	}
}

int solve_day12_part1(std::vector<std::string> input) {
	Ship1 pos;
	for (const auto& i : input) {
		handle_instruction(pos, Instruction{ i[0], std::stoi(i.substr(1, i.size() - 1)) });
	}
	return pos.manhattan_distance();
}

double solve_day12_part2(std::vector<std::string> input) {
	Ship2 pos;
	for (const auto& i : input) {
		handle_instruction(pos, Instruction{ i[0], std::stoi(i.substr(1, i.size() - 1)) });
	}
	return pos.manhattan_distance();
}

TEST_CASE("test day12 part 1") {
	auto awnser = solve_day12_part1({ "F10","N3","F7","R90","F11" });
	CHECK(awnser == 25);
}

TEST_CASE("test day12 part 2") {
	auto awnser = solve_day12_part2({ "F10","N3","F7","R90","F11" });
	CHECK(awnser == doctest::Approx(286.0));

	awnser = solve_day12_part2({ "F10","R180","F10","L180", "F10" });
	CHECK(awnser == doctest::Approx(110.0));
}

TEST_CASE("solve day12") {
	auto input = read_file_to_vector_of_strings("day12_input.txt");
	SUBCASE("part1") {
		const auto awnser = solve_day12_part1(input);
		fmt::print(fg(fmt::color::pale_golden_rod), "12-1 awnser: {}\n", awnser);
		CHECK(awnser == 441);
	}
	SUBCASE("part2") {
		const auto awnser = solve_day12_part2(input);
		fmt::print(fg(fmt::color::pale_golden_rod), "12-2 awnser: {}\n", awnser);
		CHECK(awnser == doctest::Approx(40014.0));
	}
}

