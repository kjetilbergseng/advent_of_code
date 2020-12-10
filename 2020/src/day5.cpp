#include "day3.h"
#include <fmt/core.h>
#include <tuple>
#include "utilities.h"
#include <doctest/doctest.h>
#include<numeric>
#include <assert.h> 

struct Seat {
	size_t row;
	size_t col;
	size_t id;
};
bool operator< (const Seat& lhs, const Seat& rhs) { return lhs.id < rhs.id; }
bool operator> (const Seat& lhs, const Seat& rhs) { return rhs < lhs; }
bool operator<=(const Seat& lhs, const Seat& rhs) { return !(lhs > rhs); }
bool operator>=(const Seat& lhs, const Seat& rhs) { return !(lhs < rhs); }

Seat find_seat(const std::string& code) {
	assert(code.size() > 3);
	Seat seat{ 0,0, 0 };
	for (size_t i = 0; i < code.size() - 3; ++i) {
		seat.row += (code[i] == 'B') *(128ull >> (i+1));
	}
	for (size_t i = 0; i < 3; ++i) {
		seat.col += (code[code.size() - 3 + i] == 'R') * (8ull >> (i + 1));
	}
	seat.id = seat.row * 8 + seat.col;
	return seat;
}

size_t find_max_id(std::vector<std::string> vec){
	size_t max_id = 0;
	for (const auto& str : vec) {
		auto seat=find_seat(str);
		if (seat.id > max_id) { max_id = seat.id; }
	}
	return max_id;
}

size_t find_missing_seat(std::vector<std::string> vec) {
	std::vector<Seat> seats;
	for (const auto& str : vec) {
		seats.push_back(find_seat(str));
	}
	std::sort(seats.begin(), seats.end());
	for (size_t i = 0;  const auto & seat : seats) {
		if (seat.id != seats[0].id + i) { return seats[0].id + i; }
		++i;
	}
	return 0;
}

TEST_CASE("solve day5 part 1") {
	const auto input=read_file_to_vector_of_strings("day5_input.txt");
	auto max=find_max_id(input);
	fmt::print(fg(fmt::color::pale_golden_rod), "5-1 awnser: {}\n", max);
	CHECK(max == 901);
}
TEST_CASE("solve day5 part 1") {
	const auto input = read_file_to_vector_of_strings("day5_input.txt");
	auto seat = find_missing_seat(input);
	fmt::print(fg(fmt::color::pale_golden_rod), "5-2 awnser: {}\n", seat);
	CHECK(seat == 661);
}

TEST_CASE("test day5 part 1") {
	{
		Seat seat = find_seat("BBBBBBBRRR");
		CHECK(seat.row == 127);
		CHECK(seat.col == 7);
		CHECK(seat.id == 1023);
	}
	{
		Seat seat = find_seat("BFFFBBFRRR");
		CHECK(seat.row == 70);
		CHECK(seat.col == 7);
		CHECK(seat.id == 567);
	}
}
