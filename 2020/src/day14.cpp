#include "day14.h"
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <doctest/doctest.h>
#include <numeric>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include "utilities.h"
#include <unordered_map >
#include <bitset>
#include <cmath>
struct Data_block{
	uint64_t positive_mask;
	uint64_t negative_mask;
	std::vector<std::pair<uint64_t, uint64_t>> data;
};

uint64_t positive_mask_form_string(std::string str) {
	return std::bitset<36>(std::regex_replace(str, std::regex{ "X|x" }, "0")).to_ullong();
}
uint64_t negative_mask_form_string(std::string str) {
	str = std::regex_replace(str, std::regex{ "1" }, "X");
	str = std::regex_replace(str, std::regex{ "0" }, "1");
	return positive_mask_form_string(str);
}

std::vector<Data_block> strings_to_data_blocks(const std::vector<std::string>& strings) {
	std::vector<Data_block> blocks;
	for (const auto& str : strings) {
		auto v=split_string(str, std::regex(" = "));
		if (v[0] == "mask") {
			blocks.emplace_back(
				positive_mask_form_string(v[1]),
				negative_mask_form_string(v[1]),
				std::vector<std::pair<uint64_t, uint64_t>>{}
			);
		}
		else {
			blocks[blocks.size() - 1].data.emplace_back(
				std::stoull(strip_non_numeric(v[0])),
				std::stoull(strip_non_numeric(v[1]))
			);
		}
	}
	return blocks;
}

std::vector<std::pair<uint64_t, uint64_t>> strings_to_data_part2(const std::vector<std::string>& strings) {
	std::vector<std::pair<uint64_t, uint64_t>> data;
	std::vector<uint64_t> masks;
	uint64_t mask=0;
	for (const auto& str : strings) {
		auto v = split_string(str, std::regex(" = "));
		
		if (v[0] == "mask") {
			masks.clear();
			auto m = std::regex_replace(v[1], std::regex{ "[0-9]" }, "0");
			m = std::regex_replace(m, std::regex{ "X|x" }, "1");
			mask = std::bitset<36>(m).to_ullong();
			uint64_t combinations = std::count(v[1].cbegin(), v[1].cend(), 'X');
			combinations = static_cast<uint64_t>(std::pow(2, combinations));
			for (uint64_t i = 0; i < combinations; ++i) {
				auto floating_bits = std::bitset<36>(i).to_string();
				auto fixed_bits = std::bitset<36>(positive_mask_form_string(v[1])).to_string();
				for (uint64_t n = 0, j = 0; auto & c : fixed_bits) {
					if (v[1][j] == 'X') {
						++n;
						c = floating_bits[floating_bits.size()-n];			
					}
					++j;
				}
				masks.emplace_back(std::bitset<36>(fixed_bits).to_ullong());
			}

		}
		else {
			std::vector<uint64_t> adresses;
			uint64_t adr = std::stoull(strip_non_numeric(v[0]));
			for (const auto m : masks) {
				uint64_t mask_adr = (adr | m) & std::bitset<36>(mask).flip().to_ullong();
				mask_adr = mask_adr|(m & mask);
				data.emplace_back(
					mask_adr,
					std::stoull(strip_non_numeric(v[1]))
				);
			}
		}
	}
	return data;
}


void add_data_to_memory(std::unordered_map<uint64_t, uint64_t>& memory, const Data_block& block) {
	for (auto& i : block.data) {
		memory[i.first] = block.positive_mask | i.second & ~block.negative_mask;
	}
}

uint64_t solve_day13_part1(const std::vector<Data_block>& blocks) {
	std::unordered_map<uint64_t, uint64_t> memory;
	for (const auto& block : blocks) {
		add_data_to_memory(memory, block);
	}
	return std::accumulate(memory.begin(), memory.end(), 0ull,
		[](auto lhs, auto rhs) {return lhs + rhs.second; });
}

uint64_t solve_day13_part2(const std::vector<std::pair<uint64_t, uint64_t>>& data) {
	std::unordered_map<uint64_t, uint64_t> memory;
	for (const auto& d : data) {
		memory[d.first] = d.second;
	}
	return std::accumulate(memory.begin(), memory.end(), 0ull,
		[](auto lhs, auto rhs) {return lhs + rhs.second; });
}

TEST_CASE("test day13 part2") {
	const std::vector<std::string> input = {
		"mask = 000000000000000000000000000000X1001X",
		"mem[42] = 100",
		"mask = 00000000000000000000000000000000X0XX",
		"mem[26] = 1"
	};
	const auto data = strings_to_data_part2(input);
	const auto awnser = solve_day13_part2(data);
	CHECK(awnser == 208);
}

TEST_CASE("solve day13") {
	const auto input = read_file_to_vector_of_strings("day14_input.txt");
	SUBCASE("part1") {
		const auto blocks = strings_to_data_blocks(input);
		const auto awnser = solve_day13_part1(blocks);
		fmt::print(fg(fmt::color::pale_golden_rod), "14-1 awnser: {}\n", awnser);
		CHECK(awnser == 9879607673316);
	}
	SUBCASE("part2") {
		const auto data = strings_to_data_part2(input);
		const auto awnser = solve_day13_part2(data);
		fmt::print(fg(fmt::color::pale_golden_rod), "14-2 awnser: {}\n", awnser);
		CHECK(awnser == 3435342392262);
	}
}

TEST_CASE("test day13 part1") {
	const std::vector<std::string> input = {
		"mask = XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X",
		"mem[8] = 11",
		"mem[7] = 101",
		"mem[8] = 0"
	};
	const auto blocks = strings_to_data_blocks(input);
	const auto awnser = solve_day13_part1(blocks);
	CHECK(awnser == 165);
}

