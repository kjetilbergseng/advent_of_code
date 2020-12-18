#include "day16.h"
#include <numeric>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include "utilities.h"
#include <unordered_map >
#include <bitset>
#include <cmath>
#pragma warning(push, 0) 
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <doctest/doctest.h>
#include <Eigen/Dense>
#pragma warning(pop)
struct Range{
	int from;
	int to;
};

uint64_t solve_day16_part1(const std::vector< std::vector<Range>>& validity_ranges, const std::vector< std::vector<int>>& tickets) {
	int error_rate = 0;
	for (const auto& t : tickets) {
		for (const auto& val : t) {
			bool invalid=true;
			for (const auto& crit : validity_ranges) {
				for (const auto& range : crit) {
					invalid &= (val < range.from || val > range.to);
				}
			}
			error_rate += val * invalid;
		}
	}
	return error_rate;
}

std::vector<std::vector<int>> get_valid(const std::vector< std::vector<Range>>& validity_ranges, const std::vector< std::vector<int>>& tickets) {
	std::vector<std::vector<int>> valid_tickets;
	for (const auto& t : tickets) {
		bool valid = true;
		for (const auto& val : t) {
			bool invalid = true;
			for (const auto& crit : validity_ranges) {
				for (const auto& range : crit) {
					invalid &= (val < range.from || val > range.to);
				}
			}
			valid &= (!invalid);
		}
		if (valid) {
			valid_tickets.push_back(t);
		}
	}
	return valid_tickets;
}

Eigen::MatrixXi create_vaildity_matrix(const std::vector<std::vector<Range>>& validity_ranges, const std::vector<std::vector<int>>& tickets)
{
	Eigen::MatrixXi mat(validity_ranges.size(), validity_ranges.size());
	mat.setOnes();
	const auto valid_tickets = get_valid(validity_ranges, tickets);
	for (int i = 0; i < validity_ranges.size(); ++i) {
		for (int j = 0; j < validity_ranges.size(); ++j) {
			for (const auto& ticket : valid_tickets) {
				bool valid = false;
				for (const auto& range : validity_ranges[i]) {
					if (ticket[j] >= range.from && ticket[j] <= range.to) {
						valid = true;
					}
				}
				if (!valid) {
					mat(i, j) = 0;
				}
			}
		}
	}
	return mat;
}

std::unordered_map<int64_t, int64_t> map_field_to_rule(Eigen::MatrixXi& mat)
{
	std::unordered_map<int64_t, int64_t> map;
	while (mat.sum() > 0) {
		for (int i = 0; i < mat.rows(); ++i) {
			if (mat.row(i).sum() == 1) {
				for (int j = 0; j < mat.cols(); ++j) {
					if (mat(i, j) == 1) {
						map[i] = j;
						mat.col(j).setZero();
						break;
					}
				}
			}
		}
	}
	return map;
}

int64_t solve_day16_part2(const std::vector< std::vector<Range>>& validity_ranges, const std::vector< std::vector<int>>& tickets, const std::vector<int>& my_ticket) {
	
	auto mat=create_vaildity_matrix(validity_ranges, tickets);
	auto mapped_fields=map_field_to_rule(mat);

	int64_t res = 1;
	for (int i = 0; i < 6; ++i) {
		res *= my_ticket[mapped_fields.at(i)];
	}
	return res;
}


std::vector<Range> read_range(const std::string& str)
{
	const auto inp = split_string(str,std::regex(R"(\b(?=or\b)\w+)"));
	std::vector<Range> ranges;
	for (const auto& i : inp) {
		auto rng = split_string(i, std::regex("-"));
		ranges.emplace_back(
			std::stoi(strip_non_numeric(rng[0])),
			std::stoi(strip_non_numeric(rng[1]))
		);
	}
	return ranges;
}


std::vector<int> read_ticket(const std::string& str)
{
	const auto rng = split_string(str, std::regex(","));
	std::vector<int> vec;
	for (const auto& i : rng) {
		vec.emplace_back(std::stoi(i));
	}
	return vec;
}

std::tuple<std::vector< std::vector<Range>>, std::vector< std::vector<int>>, std::vector<int>> parse_input(const std::vector<std::string>& strings) {
	std::vector< std::vector<Range>> validity_ranges;
	std::vector< std::vector<int>> tickets;
	std::vector<int> my_ticket;
	bool reading_tickets = false;
	bool reading_ranges = true;
	bool reading_my_ticket = false;
	for (const auto& str : strings) {
		reading_ranges &= (str != "");
		if (reading_ranges) {
			validity_ranges.emplace_back(read_range(str));
		}
		else if (reading_my_ticket) {
			my_ticket=read_ticket(str);
			reading_my_ticket = false;
		}
		else if (reading_tickets) {
			tickets.emplace_back(read_ticket(str));
		}
		reading_tickets |= (str == "nearby tickets:");
		reading_my_ticket |= (str == "your ticket:");
	}
	return std::make_tuple(validity_ranges,tickets, my_ticket);
}

TEST_CASE("test day16 part1") {
	const std::vector<std::string> input = {
		"class: 1-3 or 5-7",
		"row: 6-11 or 33-44",
		"seat: 13-40 or 45-50",
		"",
		"your ticket:",
		"7,1,14",
		"",
		"nearby tickets:",
		"7,3,47",
		"40,4,50",
		"55,2,20",
		"38,6,12"
	};
	const auto [validity_ranges, tickets, my_ticket]=parse_input(input);
	const auto awnser = solve_day16_part1(validity_ranges, tickets);
	CHECK(awnser == 71);
}

TEST_CASE("solve day16") {
	const auto input = read_file_to_vector_of_strings("day16_input.txt");
	const auto [validity_ranges, tickets, my_ticket] = parse_input(input);
	SUBCASE("part1") {				
		const auto awnser = solve_day16_part1(validity_ranges, tickets);
		fmt::print(fg(fmt::color::pale_golden_rod), "16-1 awnser: {}\n", awnser);
		CHECK(awnser == 25788);
	}
	SUBCASE("part2") {
		const auto awnser=solve_day16_part2(validity_ranges, tickets, my_ticket);
		fmt::print(fg(fmt::color::pale_golden_rod), "16-2 awnser: {}\n", awnser);
		CHECK(awnser == 3902565915559);
	}
}