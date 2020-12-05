#include "day3.h"
#include <fmt/core.h>
#include <tuple>
#include "utilities.h"
#include <doctest/doctest.h>
#include<numeric>
#include <regex>
#include <unordered_map>

auto read_data(const std::vector<std::string>& vec) {
	std::vector<std::unordered_map<std::string, std::string>> map_vector;
	std::unordered_map<std::string, std::string> map;
	for (const auto& str : vec) {
		if (str == "") {
			map_vector.push_back(map);
			map.clear();
		}
		else {
			auto const v = split_string(str, std::regex{ R"([:]+)" });
			if (v[0] != "cid") {
				map.emplace(std::make_pair(v[0], v[1]));
			}
		}
	}
	map_vector.push_back(map);
	return map_vector;
}

int solve_day4_part1(const std::vector<std::string>& vec) {
	int awnser = 0;	
	const auto passports =read_data(vec);
	for ( const auto& passport: passports) {
		awnser += (passport.size() == 7);
	}
	return awnser;
}

bool has_key(const std::unordered_map<std::string, std::string>& passport, const std::string& key) {
	return (passport.find(key) != passport.end());
}

template<typename T>
bool check_validity(const std::unordered_map<std::string, std::string>& passport, const std::string& key, T lambda) {
	if (!has_key(passport, key)) {return false;	}
	return lambda(passport.at(key));
}

bool check_eye_color(const std::string& eye_color) {
	const std::vector<const char*> vec{ "amb","blu", "brn", "gry", "grn", "hzl", "oth" };
	for (const auto& v : vec) {
		if (eye_color == v) { return true; };
	}
	return false;
}

bool check_passport_id(const std::string& passport_id) {
	if (passport_id.size() != 9) { return false; }
	const std::string str=std::regex_replace(passport_id, std::regex{ R"([^0-9])" }, "");
	return str.size() == 9;
}

bool check_hair_color(const std::string& color) {
	if (color.size() != 7 || color[0]!='#') { return false; }
	std::smatch m;
	std::regex_search(color, m,std::regex{ R"([0-9a-f]+)" });
	return m[0].length()==6;
}

bool check_height(const std::string& height) {
	if (height.size() < 3) { return false; }

	const std::string str = std::regex_replace(height, std::regex{ R"([^0-9(in)(cm)])" }, "");
	if (height.size() != str.size()) { return false; }

	auto fn=[&height](int min, int max)->bool {
		const std::string h = std::regex_replace(height, std::regex{ R"([^0-9])" }, "");
		return (h.size()==height.size()-2)? (std::stoi(h) >= min && std::stoi(h) <= max):false;
	};

	if (str.substr(str.size() - 2, 2) == "in") {
		return fn(59, 76);
	}
	else if (str.substr(str.size() - 2, 2) == "cm") {
		return fn(150, 193);
	}
	else {
		return false;
	}
}

int solve_day4_part2(const std::vector<std::string>& vec) {
	int awnser = 0;
	const auto passports = read_data(vec);
	for (const auto& passport : passports) {
		bool is_valid = (passport.size() == 7);
		is_valid &= check_validity(passport, "byr", [](std::string str) {return (std::stoi(str) <= 2002 && std::stoi(str) >= 1920); });
		is_valid &= check_validity(passport, "iyr", [](std::string str) {return (std::stoi(str) <= 2020 && std::stoi(str) >= 2010); });
		is_valid &= check_validity(passport, "eyr", [](std::string str) {return (std::stoi(str) <= 2030 && std::stoi(str) >= 2020); });
		is_valid &= check_validity(passport, "ecl", check_eye_color);
		is_valid &= check_validity(passport, "pid", check_passport_id);
		is_valid &= check_validity(passport, "hcl", check_hair_color);
		is_valid &= check_validity(passport, "hgt", check_height);
		awnser += is_valid;
	}
	return awnser;
}

TEST_CASE("test day4 part 1") {
	const auto vec = read_file_to_vector_of_strings("day4_input.txt", std::regex{ R"([\s]+)" });
	const auto awnser = solve_day4_part2(vec);
	fmt::print(fg(fmt::color::pale_golden_rod), "4-2 awnser: {}\n", awnser);
}

TEST_CASE("test day4 part 2") {
	const auto vec = read_file_to_vector_of_strings("day4_input.txt", std::regex{R"([\s]+)"});
	const auto awnser = solve_day4_part1(vec);
	fmt::print(fg(fmt::color::pale_golden_rod), "4-1 awnser: {}\n", awnser);
}

TEST_CASE("test has_key") {
	const std::unordered_map<std::string, std::string> m = {
		{"one","1"},
		{"two","2"}
	};
	CHECK(has_key(m, "one") == true);
	CHECK(has_key(m, "two") == true);
	CHECK(has_key(m, "three") == false);
}

TEST_CASE("test check_eye_color") {
	CHECK(check_eye_color("brn") == true);
	CHECK(check_eye_color("wat") == false);
}

TEST_CASE("test check_passport_id") {
	CHECK(check_passport_id("000000001") == true);
	CHECK(check_passport_id("aaaa77aaa") == false);
	CHECK(check_passport_id("0123456789") == false);
	CHECK(check_passport_id("01234567") == false);
	CHECK(check_passport_id("000000001aa") == false);
	CHECK(check_passport_id("") == false);
}

TEST_CASE("test check_hair_color") {
	CHECK(check_hair_color("#123abc") == true);
	CHECK(check_hair_color("#123abz") == false);
	CHECK(check_hair_color("123abc") == false);
	CHECK(check_hair_color("2123abc") == false);
	CHECK(check_hair_color("#123abcz") == false);
	CHECK(check_hair_color("") == false);
}

TEST_CASE("test check_height") {
	CHECK(check_height("60in") == true);
	CHECK(check_height("190cm") == true);
	CHECK(check_height("190in") == false);
	CHECK(check_height("190") == false);
	CHECK(check_height("cm190") == false);
	CHECK(check_height("190incm") == false);
	CHECK(check_height("") == false);
}