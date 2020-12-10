#include "day9.h"
#include <fmt/core.h>
#include <vector>
#include <stdexcept>
#include "utilities.h"
#include <doctest/doctest.h>
#include <assert.h> 

long long solve_day9_part1(const std::vector<long long>& input, const size_t preamble_length) {
	bool found = false;
	for (size_t k = 0; k < input.size() - preamble_length; ++k) {
		for (size_t i = k; i < k + (preamble_length - 1)*(!found); ++i) {
			for (size_t j = i + 1; j < (k + preamble_length)*(!found); ++j) {
				if (input[i] + input[j] == input[k + preamble_length]) { found = true; }
			}
		}
		if (!found) { return input[k + preamble_length]; }
		found = false;
	}
	throw std::runtime_error("failed to find solution for day9 part 1");
}

long long solve_day9_part2(const std::vector<long long>& input, const long long target) {
	for (size_t k = 0; k < input.size(); ++k) {
		long long sum = 0;
		for (size_t i = k; sum < target; ++i) {
			sum += input[i];
			if (sum == target) {
				auto max=*std::max_element(input.begin() + k, input.begin() + i);
				auto min=*std::min_element(input.begin() + k, input.begin() + i);
				return max+min;
			}
		}
	}
	throw std::runtime_error("failed to find solution for day9 part 2");
}

TEST_CASE("test day9 part 1") {
	long long awnser = solve_day9_part1(
		{ 35,20,15,25,47,40,62,55,65,95,102,117,150,182,127,219, 299,277,309,576 }, 5);
	CHECK(awnser == 127);
}

TEST_CASE("test day9 part 2") {
	long long awnser = solve_day9_part2(
		{ 35,20,15,25,47,40,62,55,65,95,102,117,150,182,127,219, 299,277,309,576 }, 127);
	CHECK(awnser == 62);
}

TEST_CASE("solve day9") {
	auto input = read_file_to_vector_of_int64("day9_input.txt");

	long long awnser = solve_day9_part1(input, 25);
	fmt::print(fg(fmt::color::pale_golden_rod), "9-1 awnser: {}\n", awnser);
	CHECK(awnser == 776203571);

	awnser = solve_day9_part2(input, awnser);
	fmt::print(fg(fmt::color::pale_golden_rod), "9-2 awnser: {}\n", awnser);
	CHECK(awnser == 104800569);
}

