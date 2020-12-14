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

struct Data_block{
	size_t positive_mask;
	size_t negative_mask;
	std::vector<std::pair<size_t, size_t>> data;
};
size_t positive_mask_form_string(std::string str) {
	return std::bitset<36>(std::regex_replace(str, std::regex{ "X|x" }, "0")).to_ullong();
}
size_t negative_mask_form_string(std::string str) {
	str = std::regex_replace(str, std::regex{ "1" }, "X");
	str = std::regex_replace(str, std::regex{ "0" }, "1");
	return std::bitset<36>(std::regex_replace(str, std::regex{ "X|x" }, "0")).to_ullong();
}
void add_data_to_memory(std::unordered_map<size_t, size_t> &memory, const Data_block& block) {
	for (auto &i : block.data) {
		memory[i.first] = block.positive_mask | i.second;
		memory[i.first] &= ~block.negative_mask;
	}
}

std::vector<Data_block> strings_to_data_blocks(const std::vector<std::string>& strings) {
	std::vector<Data_block> blocks;
	for (const auto& str : strings) {
		auto v=split_string(str, std::regex(" = "));
		if (v[0] == "mask") {
			blocks.emplace_back(
				positive_mask_form_string(v[1]),
				negative_mask_form_string(v[1]),
				std::vector<std::pair<size_t, size_t>>{}
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

size_t solve_day13_part1(const std::vector<Data_block>& blocks) {
	std::unordered_map<size_t, size_t> memory;
	for (const auto& block : blocks) {
		add_data_to_memory(memory, block);
	}
	return std::accumulate(memory.begin(), memory.end(), 0ull,
		[](auto lhs, auto rhs) {return lhs + rhs.second; });
}

TEST_CASE("solve day13") {
	const auto input = read_file_to_vector_of_strings("day14_input.txt");
	const auto blocks = strings_to_data_blocks(input);
	SUBCASE("part1") {		
		const auto awnser = solve_day13_part1(blocks);
		fmt::print(fg(fmt::color::pale_golden_rod), "14-1 awnser: {}\n", awnser);
		CHECK(awnser == 9879607673316);
	}
	SUBCASE("part2") {
		//fmt::print(fg(fmt::color::pale_golden_rod), "14-2 awnser: {}\n", awnser);
		//CHECK(awnser == 0);
	}
}

