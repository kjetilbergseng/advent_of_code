#include "day10.h"
#include <fmt/core.h>
#include <numeric>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include "utilities.h"
#include <doctest/doctest.h>
#include <assert.h> 
constexpr long long fact(long long n) {
	return (n > 1) ? fact(n - 1) * n : 1;
}
constexpr long long nCr(long long n, long long r) {
	return fact(n) / (fact(r) * fact(n - r));
}
long long solve_day10_part1(std::vector<int> input) {
	std::sort(input.begin(), input.end());
	std::adjacent_difference(input.begin(), input.end(), input.begin());
	return  std::count(input.begin(), input.end(), 1) 
		* (std::count(input.begin(), input.end(), 3) + 1);
}

long long solve_day10_part2(std::vector<long long> input) {
	std::sort(input.begin(), input.end());
	std::adjacent_difference(input.begin(), input.end(), input.begin());
	std::vector<long long> v;
	long long c = 0;
	for (auto i : input) {
		c += (i == 1);
		if (c > 1 && i != 1) { v.push_back(c - 1); }
		c *= (i == 1);
	}
	if (c > 1) { v.push_back(c - 1); }
	//Note: The lambda function in used in std::transform is not correct if there are more than four numbers in a row with a difference of 1 
	assert(*std::max_element(v.begin(), v.end()) < 4);
	std::transform(v.begin(), v.end(), v.begin(), [](long long n) {return nCr(n, 2) + nCr(n, 1) + 1; });
	return std::accumulate(v.begin(), v.end(), 1ll, std::multiplies<long long>());
}
TEST_CASE("test day10 part 1") {
	long long awnser = solve_day10_part1(
		{ 16,10,15,5,1,11,7,19,6,12,4 });
	CHECK(awnser == 7*5);
}

TEST_CASE("test day10 part 1") {
	long long awnser = solve_day10_part1(
		{ 28,33,18,42,31,14,46,20,48,47,24,23,49,45,19,38,39,11,1,32,25,35,8,17,7,9,4,2,34,10,3});
	CHECK(awnser == 220);
}

TEST_CASE("test day10 part 2") {
	long long awnser = solve_day10_part2(
		{ 16,10,15,5,1,11,7,19,6,12,4 });
	CHECK(awnser == 8);
}

TEST_CASE("test day10 part 2") {
	long long awnser = solve_day10_part2(
		{ 28,33,18,42,31,14,46,20,48,47,24,23,49,45,19,38,39,11,1,32,25,35,8,17,7,9,4,2,34,10,3 });
	CHECK(awnser == 19208);
}

TEST_CASE("solve day10") {
	long long awnser = solve_day10_part1(read_file_to_vector_of_ints("day10_input.txt"));
	fmt::print(fg(fmt::color::pale_golden_rod), "10-1 awnser: {}\n", awnser);
	CHECK(awnser == 1755);
}

TEST_CASE("solve day10 part 2") {
	long long awnser = solve_day10_part2(read_file_to_vector_of_int64("day10_input.txt"));
	fmt::print(fg(fmt::color::pale_golden_rod), "10-2 awnser: {}\n", awnser);
	CHECK(awnser == 4049565169664);
}

