#include "day7.h"
#include <fmt/core.h>
#include <tuple>
#include "utilities.h"
#include <doctest/doctest.h>
#include<numeric>
#include <assert.h> 

struct Bag {
	std::string name;
	std::vector<std::tuple<std::string, int>> contains;
};

bool can_hold(Bag bag, std::vector<std::string> valid_bags) {
	for (const auto& v : valid_bags) {
		for (const auto& [name,num] : bag.contains) {
			if (name == v) { return true; }
		}
	}
	return false;
}

int count_containing_bags(const std::vector<Bag>& bags, std::string bag_type) {
	auto it = std::find_if(bags.cbegin(), bags.cend(), 
		[&bag_type](Bag bag) {return bag.name == bag_type; });
	int awnser = 0;
	if (it != bags.cend()) {
		for (const auto& [name, num] : (*it).contains) {
			awnser += count_containing_bags(bags, name)*num + num;
		}
	}
	return awnser;
}

size_t solve_day7_part_1(const std::vector<Bag>& bags, std::string bag_type) {
	std::vector<std::string> valid_bags{ bag_type };
	for (size_t found = 0; found<valid_bags.size(); ){
		found = valid_bags.size();
		for (const auto& bag : bags) {
			bool was_found = std::any_of(valid_bags.cbegin(), valid_bags.cend(), 
				[&bag](const std::string& str) {return bag.name == str; });
			if (!was_found && can_hold(bag, valid_bags)) {
				valid_bags.push_back(bag.name);
			}
		}
	}
	return valid_bags.size()-1;
}

std::vector<Bag> parse_input(std::vector<std::string> input) {
	std::vector<Bag> bags;
	for (const auto& line : input) {
		auto cleaned=std::regex_replace(line, std::regex{ R"(bags|bag|[.]|[" "]+)" }, "");
		auto v=split_string(cleaned, std::regex{ R"(contain)" });
		auto contains = split_string(v[1], std::regex{ R"(,)" });
		std::vector<std::tuple<std::string, int>> tpl;
		for (const auto& bag : contains) {
			auto type = std::regex_replace(bag, std::regex{ R"([0-9]+)" }, "");
			auto number = std::regex_replace(bag, std::regex{ R"([^0-9]+)" }, "");
			if (number == "") { number = '0'; }
			tpl.emplace_back(type,std::stoi(number));
		}
		bags.push_back(Bag{ v[0], tpl });
	}
	return bags;
}

TEST_CASE("solve day7 part 1") {
	const auto input = read_file_to_vector_of_strings("day7_input.txt");
	auto bags=parse_input(input);
	const auto awnser = solve_day7_part_1(bags, "shinygold");
	fmt::print(fg(fmt::color::pale_golden_rod), "7-1 awnser: {}\n", awnser);
	CHECK(awnser == 268);
}

TEST_CASE("solve day7 part 2") {
	const auto input = read_file_to_vector_of_strings("day7_input.txt");
	auto bags = parse_input(input);
	const auto awnser = count_containing_bags(bags, "shinygold");
	fmt::print(fg(fmt::color::pale_golden_rod), "7-2 awnser: {}\n", awnser);
	CHECK(awnser == 7867);
}

TEST_CASE("test day7 part 1") {
	const std::vector<Bag> input =
	{ 
		Bag{"light red", {{"bright white",1}, {"muted yellow",2}}},
		Bag{"dark orange", {{"bright white",3}, {"muted yellow",4}}},
		Bag{"bright white", {{"shiny gold",1}}},
		Bag{"muted yellow", {{"shiny gold",2}, {"faded blue",9}}},
		Bag{"shiny gold", {{"dark olive",1}, {"vibrant plum",2}}},
		Bag{"dark olive", {{"faded blue",3}, {"dotted black",4}}},
		Bag{"vibrant plum", {{"faded blue",5}, {"dotted black",6}}},
		Bag{"faded blue", {{"no other",0}}},
		Bag{"dotted black", {{"no other",0}}}
	};

	SUBCASE("part 1") {
		const auto awnser = solve_day7_part_1(input, "shiny gold");
		CHECK(awnser == 4);
	}
	SUBCASE("part 2") {
		const auto awnser = count_containing_bags(input, "shiny gold");
		CHECK(awnser == 32);
	}
}