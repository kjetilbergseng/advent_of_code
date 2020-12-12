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
	{a.manhattan_distance()};
};

struct Waypoint {
	double x;
	double y;
};

struct Ship1 {
	double x=0;
	double y=0;
	double rot=0;
	double manhattan_distance() {
		return abs(x) + abs(y);
	}
	void move_forward(int value) {
		x +=(std::cos(rot * pi() / 180)) * value;
		y += (std::sin(rot * pi() / 180)) * value;
	}
	void rotate_left(int value) { rot += value; }
	void rotate_right(int value) { rot -= value; }
	void move_east(int value) { x += value; }
	void move_west(int value) { x -= value; }
	void move_north(int value) { y += value; }
	void move_south(int value) { y -= value; }
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
	void rotate_right(int value) { rotate_left(360 - value % 360); }
	void move_east(int value) {	wp.x += value; }
	void move_west(int value) { wp.x -= value; }
	void move_north(int value) { wp.y += value;	}
	void move_south(int value) { wp.y -= value;	}
};

struct Instruction {
	char action;
	int value;
};

template<Ship S>
void handle_instruction(S& ship, const Instruction& ins) {
	switch (ins.action)
	{
	case 'F':
		ship.move_forward(ins.value); break;
	case 'R':
		ship.rotate_right(ins.value); break;
	case 'L':
		ship.rotate_left(ins.value); break;
	case 'E':
		ship.move_east(ins.value); break;
	case 'N':
		ship.move_north(ins.value); break;
	case 'W':
		ship.move_west(ins.value); break;
	case 'S':
		ship.move_south(ins.value); break;
	default:
		throw std::runtime_error("invalid instruction in day 12");
	}
}

template<Ship S>
double solve(S ship, std::vector<std::string> input) {
	for (const auto& i : input) {
		handle_instruction(ship, Instruction{ i[0], std::stoi(i.substr(1, i.size() - 1)) });
	}
	return ship.manhattan_distance();
}

TEST_CASE("test day12 part 1") {
	auto awnser = solve(Ship1{}, { "F10","N3","F7","R90","F11" });
	CHECK(awnser == doctest::Approx(25.0));
}

TEST_CASE("test day12 part 2") {
	auto awnser = solve(Ship2{}, { "F10","N3","F7","R90","F11" });
	CHECK(awnser == doctest::Approx(286.0));

	awnser = solve(Ship2{}, { "F10","R180","F10","L180", "F10" });
	CHECK(awnser == doctest::Approx(110.0));
}

TEST_CASE("solve day12") {
	auto input = read_file_to_vector_of_strings("day12_input.txt");
	SUBCASE("part1") {
		const auto awnser = solve(Ship1{},input);
		fmt::print(fg(fmt::color::pale_golden_rod), "12-1 awnser: {:g}\n", awnser);
		CHECK(awnser == doctest::Approx(441.0));
	}
	SUBCASE("part2") {
		const auto awnser = solve(Ship2{}, input);
		fmt::print(fg(fmt::color::pale_golden_rod), "12-2 awnser: {:g}\n", awnser);
		CHECK(awnser == doctest::Approx(40014.0));
	}
}

