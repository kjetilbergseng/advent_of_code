#include "day3.h"
#include <fmt/core.h>
#include <tuple>
#include "utilities.h"
#include <doctest/doctest.h>
#include<numeric>

struct slope {
	size_t x;
	size_t y;
};

size_t solve_day3_part1(std::vector<std::string> vec, slope s) {
	size_t x = 0, y = 0, trees = 0;
	while (y<vec.size()) {
		trees += (vec[y][x] == '#');
		x += ( x+s.x < vec[y].size() ) ? s.x : s.x- vec[y].size();
		y += s.y;
	}
	return trees;
}

size_t solve_day3_part2(std::vector<std::string> vec, std::vector<slope> slopes) {
	size_t awnser = 1;
	for (const auto& s : slopes) {
		awnser *= solve_day3_part1(vec, s);
	}
	return awnser;
}

TEST_CASE("solve day3 part 1") {
	auto awnser = solve_day3_part1(read_file_to_vector_of_strings("day3_input.txt"), { 3,1 });
	fmt::print(fg(fmt::color::pale_golden_rod), "3-1 awnser: {}\n", awnser);
}

TEST_CASE("solve day3 part 2") {
	auto awnser = solve_day3_part2(read_file_to_vector_of_strings("day3_input.txt"),
		{ {1,1}, {3,1}, {5,1}, {7,1}, {1,2} });
	fmt::print(fg(fmt::color::pale_golden_rod), "3-2 awnser: {}\n", awnser);
}

TEST_CASE("test day3 part 1") {
	auto awnser = solve_day3_part1({
		"..##.......",
		"#...#...#..",
		".#....#..#.",
		"..#.#...#.#",
		".#...##..#.",
		"..#.##.....",
		".#.#.#....#",
		".#........#",
		"#.##...#...",
		"#...##....#",
		".#..#...#.#" }, { 3,1 });
	CHECK(awnser == 7);
}
