#include "day18.h"
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

int64_t add(int64_t lhs, int64_t rhs) {
	return lhs + rhs;
}
int64_t multiply(int64_t lhs, int64_t rhs) {
	return lhs * rhs;
}
std::vector<std::string> string_to_vector(std::string str) {
	str = std::regex_replace(str, std::regex(" "), "");
	str = std::regex_replace(str, std::regex("[+]"), ",+,");
	str = std::regex_replace(str, std::regex("[*]"), ",*,");
	return split_string(str, std::regex(","));
}


int64_t solve_basic_expression(std::string expression) {
	auto v = string_to_vector(expression);
	auto fn = add;

	return std::accumulate(v.cbegin(), v.cend(), 0ll, [&fn](auto lhs, auto rhs) {
		if (rhs == "+") { fn = add; return lhs; }
		if (rhs == "*") { fn = multiply; return lhs; }
		return fn(lhs, std::stoll(rhs));
		});
}

int64_t addition_precedence(std::string expression) {
	auto v = string_to_vector(expression);
	int64_t awnser = 0;
	std::vector<int64_t> v2;
	for (const auto& i : v) {
		if (i == "*") {
			v2.push_back(awnser);
			awnser = 0;
		}
		else if (i != "+") {
			awnser += std::stoll(i);
		}
	}
	v2.push_back(awnser);
	return std::accumulate(v2.cbegin(), v2.cend(), 1ll, std::multiplies<int64_t>());
}

std::pair<size_t, size_t> find_parentheses_open_close(const std::string& expression) {
	size_t begin =0;
	while (true) {
		begin += expression.substr(begin, expression.size() - begin).find_first_of('(');
		auto open= expression.substr(begin+1,expression.size()-begin-1).find_first_of("(");
		auto close = expression.substr(begin + 1, expression.size() - begin - 1).find_first_of(")");
		if (close < open) {
			return { begin,close+ begin + 1 };
		}
		begin += open+1;
	}
	throw std::runtime_error("failed to find begin and end parentecies");
}

template <typename T>
std::string solve_complex_expression(std::string expression, T fn) {
	expression = std::regex_replace(expression, std::regex(" "), "");
	if (expression == "") {
		return "";
	}
	auto count_begin = std::count(expression.begin(), expression.end(), '(');
	if (count_begin == 0) {
		return std::to_string(fn(expression));
	}
	auto [o,c]=find_parentheses_open_close(expression);
	return solve_complex_expression(
		expression.substr(0, o) +
		solve_complex_expression(expression.substr(o + 1, c - o - 1), fn) +
		expression.substr(c + 1, expression.size() - c - 1),
		fn
	);
}

template <typename T>
int64_t solve_day18(const std::vector<std::string>& input, T fn) {
	int64_t awnser = 0;
	for (auto i : input) {
		auto expr = solve_complex_expression(i, fn);
		awnser += fn(expr);
	}
	return awnser;
}

TEST_CASE("test find_parentheses_open_close") {
	SUBCASE("basic case") {
		auto [o, c] = find_parentheses_open_close("(1+2)");
		CHECK(o == 0);
		CHECK(c == 4);
	}
	SUBCASE("two") {
		auto [o, c] = find_parentheses_open_close("(1+2*5)+(5+2)");
		CHECK(o == 0);
		CHECK(c == 6);
	}
	SUBCASE("not starting with parentheses") {
		auto [o, c] = find_parentheses_open_close("1+3(1+2*5)+(5+2)");
		CHECK(o == 3);
		CHECK(c == 9);
	}
	SUBCASE("nested parentheses") {
		auto [o, c] = find_parentheses_open_close("1+3(1+(2*5))+(5+2)+5");
		CHECK(o == 6);
		CHECK(c == 10);
	}
}
TEST_CASE("test solve_basic_expression") {
	SUBCASE("add") {
		auto str = "1 + 2";
		auto awnser = solve_basic_expression(str);
		CHECK(awnser == 3);
	}
	SUBCASE("multiply") {
		auto str = "1 * 2";
		auto awnser = solve_basic_expression(str);
		CHECK(awnser == 2);
	}
	SUBCASE("add and multiply") {
		auto str = "1 + 2 * 3";
		auto awnser = solve_basic_expression(str);
		CHECK(awnser == 9);
	}
	SUBCASE("multiply and add") {
		auto str = "1 * 2 + 3";
		auto awnser = solve_basic_expression(str);
		CHECK(awnser == 5);
	}
}
TEST_CASE("test addition_precedence") {
	SUBCASE("add") {
		auto str = "1 + 2";
		auto awnser = addition_precedence(str);
		CHECK(awnser == 3);
	}
	SUBCASE("multiply") {
		auto str = "1 * 2";
		auto awnser = addition_precedence(str);
		CHECK(awnser == 2);
	}
	SUBCASE("add and multiply") {
		auto str = "1 + 2 * 3";
		auto awnser = addition_precedence(str);
		CHECK(awnser == 9);
	}
	SUBCASE("multiply and add") {
		auto str = "1 * 2 + 3";
		auto awnser = addition_precedence(str);
		CHECK(awnser == 5);
	}
	SUBCASE("multiply and add") {
		auto str = "7 * 8 * 6 + 8";
		auto awnser = addition_precedence(str);
		CHECK(awnser == 784);
	}
}
TEST_CASE("test solve_complex_expression") {
	SUBCASE("with parentheses") {
		auto str = "1 + (2 * 3)";
		auto expr = solve_complex_expression(str, solve_basic_expression);
		auto awnser= solve_basic_expression(expr);
		CHECK(awnser == 7);
	}
	SUBCASE("with nested parentheses") {
		auto str = "1 + (2 * (3+2))";
		auto expr = solve_complex_expression(str, solve_basic_expression);
		auto awnser = solve_basic_expression(expr);
		CHECK(awnser == 11);
	}
	SUBCASE("with two parentheses") {
		auto str = "1 + (2 * 3) + (3 + 2)";
		auto expr = solve_complex_expression(str, solve_basic_expression);
		auto awnser = solve_basic_expression(expr);
		CHECK(awnser == 12);
	}
	SUBCASE("not parentheses at the end") {
		auto str = "1 + (2 * 3) + (3 + 2) + 3 * 6";
		auto expr = solve_complex_expression(str, solve_basic_expression);
		auto awnser = solve_basic_expression(expr);
		CHECK(awnser == 90);
	}
	SUBCASE("with more complex case") {
		auto str = "((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2";
		auto expr = solve_complex_expression(str, solve_basic_expression);
		auto awnser = solve_basic_expression(expr);
		CHECK(awnser == 13632);
	}
	SUBCASE("with more complex case") {
		auto str = "5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))";
		auto expr = solve_complex_expression(str, solve_basic_expression);
		auto awnser = solve_basic_expression(expr);
		CHECK(awnser == 12240);
	}
	SUBCASE("with more complex case") {
		auto str = "(8 + (5 + 9 + 8 + 7)) * (4 * 8)";
		auto expr = solve_complex_expression(str, solve_basic_expression);
		auto awnser = solve_basic_expression(expr);
		CHECK(awnser == 1184);
	}
	SUBCASE("with more complex case") {
		auto str = "(7 * 8 * 6 + 8 * (7 + 2 + 3 + 2 + 2 * 3) + (8 * 9 * 8)) + (2 + 5 + (6 + 7 + 6)) + 5 + 8 + 5 * 4";
		auto expr = solve_complex_expression(str, solve_basic_expression);
		auto awnser = solve_basic_expression(expr);
		CHECK(awnser == 68528);
	}
	SUBCASE("with addition_precedence") {
		auto str = "1 + (2 * 3) + (4 * (5 + 6))";
		auto expr = solve_complex_expression(str, addition_precedence);
		auto awnser = solve_basic_expression(expr);
		CHECK(awnser == 51);
	}
	SUBCASE("with addition_precedence") {
		auto str = "5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))";
		auto expr = solve_complex_expression(str, addition_precedence);
		auto awnser = solve_basic_expression(expr);
		CHECK(awnser == 669060);
	}
	SUBCASE("with addition_precedence") {
		auto str = "((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2";
		auto expr = solve_complex_expression(str, addition_precedence);
		auto awnser = solve_basic_expression(expr);
		CHECK(awnser == 23340);
	}
}

TEST_CASE("solve day18") {
	const auto input = read_file_to_vector_of_strings("day18_input.txt");
	SUBCASE("part1") {		
		auto awnser = solve_day18(input, solve_basic_expression);
		fmt::print(fg(fmt::color::pale_golden_rod), "18-1 awnser: {}\n", awnser);
		CHECK(awnser == 4491283311856);
	}
	SUBCASE("part2") {
		auto awnser = solve_day18(input, addition_precedence);
		fmt::print(fg(fmt::color::pale_golden_rod), "18-2 awnser: {}\n", awnser);
		CHECK(awnser == 68852578641904);
	}
}