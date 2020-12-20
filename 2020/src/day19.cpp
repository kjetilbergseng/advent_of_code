#include "day19.h"
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

std::string to_string(const std::vector<int>& vec) {
	std::string str;
	str.reserve(vec.size());
	for (int i : vec) {
		switch (i)
		{
		case -1:
			str += 'a'; break;
		case -2:
			str += 'b'; break;
		default:
			throw std::runtime_error("invalid case in to_string");
		}
	}
	return str;
}

std::vector<std::string> output_to_vector_of_string(const std::vector<std::vector<int>>& output) {
	std::vector<std::string> str;
	for (const auto& vec : output) {
		str.push_back(to_string(vec));
	}
	return str;
}

int count_matches(std::vector<std::string>& messages, std::vector<std::string>& output) {
	int count = 0;
	size_t out_start=0;
	std::sort(messages.begin(), messages.end());
	std::sort(output.begin(), output.end());
	for (const auto& msg : messages) {
		for (size_t i = out_start; i < output.size();++i) {
			if (output[i] == msg) {
				++count;
				out_start = i + 1;
				break;
			}
		}		
	}
	return count;
}

bool matches(const std::string& message, const std::vector<std::string>& vec) {
	for (const auto& i : vec) {
		if (i == message) {
			return true;
		}
	}
	return false;
}

int count_looping_matches(std::vector<std::string>& messages, std::vector<std::string>& a42, std::vector<std::string>& a31) {
	int count = 0;
	size_t n = a42[0].size();
	size_t m = a31[0].size();
	for (const auto& msg : messages) {
		size_t j = 0;
		while (matches(msg.substr(j, n), a42)) {
			j += n;
		}
		if (j/n > (msg.size()-j) / m) {
			while (matches(msg.substr(j, m), a31)) {
				j += m;
				if (j == msg.size()) {
					++count; break;
				}
			}
		}
	}
	return count;
}

std::vector< std::vector<int>> add_to_result(const std::vector< std::vector<int>> &out, const std::vector<std::vector<int>>& vec) {
	std::vector< std::vector<int>> ret;
	for (int i = 0; i < vec.size(); ++i) {
		for (int j = 0; j < out.size(); ++j) {
			auto tmp = out[j];
			tmp.insert(tmp.end(), vec[i].cbegin(), vec[i].cend());
			ret.push_back(tmp);		
		}
	}
	return ret;
}

std::vector<std::vector<int>> find_output(const std::unordered_map<int, std::vector<std::vector<int>>>& input, int val) {
	if (val < 0) {
		return { { val } };
	}
	std::vector<std::vector<int>> out;
	
	for (int i = 0; i < input.at(val).size();++i) {
		std::vector<std::vector<int>> tmp{ {} };
		for (int j : input.at(val)[i]) {		
			auto vec = find_output(input, j);
			tmp =add_to_result(tmp, vec);
		}
		for (int j = 0; j < tmp.size(); ++j) {
			out.push_back({});
			out[out.size()-1].insert(out[out.size() - 1].end(), tmp[j].cbegin(), tmp[j].cend());
		}
	} 
	return out;
}
	

TEST_CASE("test add_to_result") {
	SUBCASE("case 1") {
		std::vector<std::vector<int>> out{ {},{} };
		std::vector<std::vector<int>> vec{ {-1} };
		auto awnser = add_to_result(out, vec);
	}
	SUBCASE("case 2") {
		std::vector<std::vector<int>> out{ {} };
		std::vector<std::vector<int>> vec{ {-1, -2}, { -2, -1 } };
		auto awnser = add_to_result(out, vec);;
		CHECK(to_string(awnser[0]) == "ab");
		CHECK(to_string(awnser[1]) == "ba");
	}
	SUBCASE("case 3") {
		std::vector<std::vector<int>> out{ {-2, -2, -1}, { -1, -1, -2 } };
		std::vector<std::vector<int>> vec{ {-1, -2}, { -2, -1 } };
		auto awnser = add_to_result(out, vec);;
		CHECK(to_string(awnser[0]) == "bbaab");
		CHECK(to_string(awnser[1]) == "aabab");
		CHECK(to_string(awnser[2]) == "bbaba");
		CHECK(to_string(awnser[3]) == "aabba");
	}
}

std::unordered_map<int, std::vector<std::vector<int>>> parse_input(const std::vector<std::string>& input ) {
	std::unordered_map<int, std::vector<std::vector<int>>> parsed_input;
	for (const auto& i : input) {
		auto str_vec=split_string(i, std::regex(": "));
		auto rule = split_string(str_vec[1], std::regex(" "));
		std::vector<std::vector<int>> v{ {} };
		size_t pos = 0;
		for (const auto& r : rule) {
			if (r == "|") {
				++pos;
				v.push_back({});
			}
			else if (r == "\"a\"") {
				v[pos].push_back(-1);
			}
			else if (r == "\"b\"") {
				v[pos].push_back(-2);
			}
			else {
				v[pos].push_back(std::stoi(r));
			}		
		}
		parsed_input[std::stoi(str_vec[0])] = v;
	}
	return parsed_input;
}

TEST_CASE("test find_output") {
	SUBCASE("simplest case") {
		std::unordered_map<int, std::vector<std::vector<int>>> rules{
			{0,{{1}}},
			{1,{{-1}}}
		};
		auto awnser = find_output(rules,0);
		CHECK(to_string(awnser[0]) == "a");
	}
	SUBCASE("basic case") {
		std::unordered_map<int, std::vector<std::vector<int>>> rules{
			{ 0, {{1,2}}},
			{ 1, {{-1}} },
			{ 2, {{-2}} }
		};
		auto awnser = find_output(rules,0);
		CHECK(to_string(awnser[0]) == "ab");
	}
	SUBCASE("one more level of inderection") {
		std::unordered_map<int, std::vector<std::vector<int>>> rules{
			{0,{{1,3}}},
			{1,{{-1}}},
			{2,{{-2}}},
			{3,{{2}}},
		};
		auto awnser = find_output(rules,0);
		CHECK(to_string(awnser[0]) == "ab");
	}
	SUBCASE("#3 has two values") {
		std::unordered_map<int, std::vector<std::vector<int>>> rules{
			{0,{{1,3}}},
			{1,{{-1}}},
			{2,{{-2}} },
			{3,{{2,1}}},
		};
		auto awnser = find_output(rules,0);
		CHECK(to_string(awnser[0]) == "aba");
	}
	SUBCASE("branch") {
		std::unordered_map<int, std::vector<std::vector<int>>> rules{
			{0,{{1,2}}},
			{1,{{-1}}},
			{2,{{1,3},{3,1}} },
			{3,{{-2}}},
		};
		auto awnser = find_output(rules, 0);
		CHECK(to_string(awnser[0]) == "aab");
		CHECK(to_string(awnser[1]) == "aba");
	}
	SUBCASE("branch") {
		std::unordered_map<int, std::vector<std::vector<int>>> rules{
			{0,{{1,2}}},
			{1,{{4,4},{3,3}} },
			{2,{{3,4},{4,3}} },
			{3,{{-1}}},
			{4,{{-2}}}
		};
		auto awnser = find_output(rules,0);
		CHECK(to_string(awnser[0]) == "bbab");
		CHECK(to_string(awnser[1]) == "aaab");
		CHECK(to_string(awnser[2]) == "bbba");
		CHECK(to_string(awnser[3]) == "aaba");
	}
	SUBCASE("branches") {
		std::unordered_map<int, std::vector<std::vector<int>>> rules{
			{0,{{4,1,5}}},
			{1,{{2,3},{3,2}} },
			{2,{{4,4},{5,5}} },
			{3,{{4,5},{5,4}} },
			{4,{{-1}}},
			{5,{{-2}}}
		};
		std::vector<std::string> messages{
			"ababbb",
			"bababa",
			"abbbab",
			"aaabbb",
			"aaaabbb"
		};
		auto output = find_output(rules, 0);
		auto awnser = output_to_vector_of_string(output);
		int matches = count_matches(messages, awnser);
		CHECK(matches == 2);
	}
	SUBCASE("test part2") {
		auto rules_input = read_file_to_vector_of_strings("day19_test_rules.txt");
		auto rules = parse_input(rules_input);
		std::vector<std::string> messages{
			"bbbabbaaaaababaaaaaa",
			"bbabbbbaabaabba",
			"ababaaaaaabaaab",
			"babbbbaabbbbbabbbbbbaabaaabaaa"
		};
		auto f42 = find_output(rules, 42);
		auto f31 = find_output(rules, 31);
		auto a42 = output_to_vector_of_string(f42);
		auto a31 = output_to_vector_of_string(f31);
		int matches = count_looping_matches(messages, a42, a31);
		CHECK(matches == 3);
	}
	SUBCASE("test part2") {
		auto rules_input = read_file_to_vector_of_strings("day19_test_rules.txt");
		auto rules = parse_input(rules_input);
		auto messages = read_file_to_vector_of_strings("day19_test_messages.txt");
		auto f42 = find_output(rules, 42);
		auto f31 = find_output(rules, 31);
		auto a42 = output_to_vector_of_string(f42);
		auto a31 = output_to_vector_of_string(f31);
		int matches=count_looping_matches(messages, a42,a31);
		CHECK(matches == 12);
	}
}

TEST_CASE("solve day19") {
	////Slow in debug mode
	//SUBCASE("part1") {	
	//	auto rules_input = read_file_to_vector_of_strings("day19_rules.txt");
	//	auto rules =parse_input(rules_input);
	//	auto messages = read_file_to_vector_of_strings("day19_messages.txt");
	//	auto output = find_output(rules, 0);
	//	auto awnser = output_to_vector_of_string(output);
	//	int matches = count_matches(messages, awnser);
	//	fmt::print(fg(fmt::color::pale_golden_rod), "19-1 awnser: {}\n", matches);
	//	CHECK(matches == 248);
	//}
	SUBCASE("part2") {
		auto rules_input = read_file_to_vector_of_strings("day19_rules.txt");
		auto rules = parse_input(rules_input);
		auto messages = read_file_to_vector_of_strings("day19_messages.txt");
		auto f42 = find_output(rules, 42);
		auto f31 = find_output(rules, 31);
		auto a42 = output_to_vector_of_string(f42);
		auto a31 = output_to_vector_of_string(f31);
		int matches = count_looping_matches(messages, a42, a31);
		fmt::print(fg(fmt::color::pale_golden_rod), "19-2 awnser: {}\n", matches);
		CHECK(matches == 381);
	}
}