#include "day17.h"
#include <numeric>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include "utilities.h"
#include <unordered_map >
#include <bitset>
#include <cmath>
#pragma warning(push, 1) 
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <doctest/doctest.h>
#include <Eigen/Dense>
#pragma warning(pop)


Eigen::MatrixXi update_plane(const Eigen::MatrixXi& sum_layers, const Eigen::MatrixXi& previous_plane)
{
	Eigen::MatrixXi plane(previous_plane.rows(), previous_plane.cols());
	plane.setZero();
	for (int64_t m = 1; m < previous_plane.rows() - 1; ++m) {
		for (int64_t n = 1; n < previous_plane.cols() - 1; ++n) {
			int sum = sum_layers.block(m - 1, n - 1, 3, 3).sum();
			if (sum == 3) {
				plane(m, n) = 1;
			}
			else if (previous_plane(m, n) == 1 && sum == 4) {
				plane(m, n) = 1;
			}
		}
	}
	return plane;
}

int sum(std::vector<Eigen::MatrixXi>& planes)
{
	int sum = 0;
	for (size_t i = 0; i < planes.size(); ++i) {
		sum += planes[i].sum()*(1+(i>0));
	}
	return sum;
}

int solve_day17_part1(std::vector<Eigen::MatrixXi> previous, int64_t cycles) {
	std::vector<Eigen::MatrixXi> next;
	Eigen::MatrixXi zero_layer = previous[0];
	zero_layer.setZero();
	previous.push_back(zero_layer);
	previous.push_back(zero_layer);
	for (size_t i = 0; i < previous.size() -1; ++i) {	
		auto sum_layers = previous[i] + previous[i + 1] + previous[abs(static_cast<int>(i) - 1)];
		next.emplace_back(update_plane(sum_layers, previous[i]));
	}
	if (cycles == 1) {
		return sum(next);
	}
	else {
		return solve_day17_part1(next, cycles - 1);
	}
}

int sum_4d(std::vector<std::vector<Eigen::MatrixXi>>& planes)
{
	int sum = 0;

	for (size_t i = 0; i < planes.size(); ++i) {
		for (size_t j = 0; j < planes[i].size(); ++j) {
			sum += planes[i][j].sum() * (1 + (j > 0 && i > 0) + (j > 0) + (i > 0));
		}
	}
	return sum;
}

void pad_4D(std::vector<std::vector<Eigen::MatrixXi>>& input, Eigen::MatrixXi layer) {
	for (auto& i : input) {
		i.push_back(layer);
	}
	std::vector<Eigen::MatrixXi> mat;
	for (int i = 0; i < input[0].size();++i) {
		mat.push_back(layer);
	}
	input.push_back(mat);
}

int solve_day17_part2(std::vector<std::vector<Eigen::MatrixXi>> previous, int64_t cycles) {
	std::vector <std::vector<Eigen::MatrixXi>> next;
	Eigen::MatrixXi zero_layer = previous[0][0];
	zero_layer.setZero();
	pad_4D(previous, zero_layer);
	pad_4D(previous, zero_layer);

	for (size_t i = 0; i < previous.size() - 1; ++i) {
		std::vector<Eigen::MatrixXi> next_i;
		for (size_t j = 0; j < previous[0].size() - 1; ++j) {
			Eigen::MatrixXi sum_layers= zero_layer;
			for (int n = -1; n <= 1; ++n) {
				for (int m = -1; m <= 1; ++m) {
					sum_layers += previous[abs(static_cast<int>(i) + n)][abs(static_cast<int>(j) + m)];
				}
			}
			next_i.emplace_back(update_plane(sum_layers, previous[i][j]));
		}
		next.push_back(next_i);
	}

	if (cycles == 1) {
		return sum_4d(next);
	}
	else {
		return solve_day17_part2(next, cycles - 1);
	}
}

Eigen::MatrixXi add_padding(const Eigen::MatrixXi& matrix, ptrdiff_t padding) {
	Eigen::MatrixXi m(matrix.rows() + 2ull * padding, matrix.cols() + 2ull * padding);
	m.setZero();
	for (ptrdiff_t i=0; i < matrix.rows(); ++i) {
		for (ptrdiff_t j=0; j < matrix.cols(); ++j) {
			m(i + padding, j + padding) = matrix(i, j);
		}
	}
	return m;
}

TEST_CASE("test day17 part1") {
	int64_t cycles = 6;
	Eigen::MatrixXi plane(3, 3);
	plane << 0, 1, 0,
			 0, 0, 1,
			 1, 1, 1;
	plane=add_padding(plane, cycles+1);
	auto awnser = solve_day17_part1({ plane }, cycles);
	CHECK(awnser == 112);
}

TEST_CASE("test day17 part2") {
	int64_t cycles = 6;
	Eigen::MatrixXi plane(3, 3);
	plane << 0, 1, 0,
		     0, 0, 1,
		     1, 1, 1;
	plane = add_padding(plane, cycles + 1);
	auto awnser = solve_day17_part2({ {plane} }, cycles);
	CHECK(awnser == 848);
}


TEST_CASE("solve day17") {
	int64_t cycles = 6;
	Eigen::MatrixXi plane(8, 8);
	plane <<
		0, 1, 0, 1, 0, 1, 0, 0,
		0, 0, 1, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 0, 0, 1,
		1, 1, 1, 1, 1, 0, 0, 1,
		1, 1, 1, 1, 1, 0, 0, 1,
		1, 1, 1, 0, 0, 1, 0, 1,
		1, 0, 0, 1, 1, 0, 1, 1,
		1, 0, 1, 0, 1, 1, 1, 1;
	plane = add_padding(plane, cycles + 1);
	SUBCASE("part1") {					
		const auto awnser = solve_day17_part1({ plane }, cycles);
		fmt::print(fg(fmt::color::pale_golden_rod), "17-1 awnser: {}\n", awnser);
		CHECK(awnser == 375);
	}
	SUBCASE("part2") {
		const auto awnser = solve_day17_part2({ { plane } }, cycles);
		fmt::print(fg(fmt::color::pale_golden_rod), "17-2 awnser: {}\n", awnser);
		CHECK(awnser == 2192);
	}
}