#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <fmt/core.h>
#include <fmt/color.h>
#include <regex>
class FileManager {
public:
	FileManager(const char* filename) {
		m_file.open(filename, std::ifstream::in);
		if (m_file.fail()) {
			fmt::print(fg(fmt::color::crimson), "Failed to open: {}\n", filename);
			throw std::ios_base::failure("Failed to open file");
		}
	}
	~FileManager() { m_file.close(); }
	std::ifstream& get() { return m_file; }
private:
	std::ifstream m_file;
};

inline std::vector<std::string> split_string(std::string str, const std::regex delims) {
	return std::vector<std::string>(
		std::sregex_token_iterator{ str.begin(), str.end(), delims, -1 },
		std::sregex_token_iterator{}
	);
}

inline std::vector<int> read_file_to_vector_of_ints(const char* filename)
{
	std::vector<int> v;
	FileManager fm(filename);
	for (std::string line; std::getline(fm.get(), line); ) {
		v.push_back(std::stoi(line));
	}
	return v;
}

inline std::vector<std::string> read_file_to_vector_of_strings(const char* filename)
{
	std::vector<std::string> v;
	FileManager fm(filename);
	for (std::string line; std::getline(fm.get(), line); ) {
		std::erase(line, '\r');
		v.push_back(line);
	}
	return v;
}

inline std::vector<std::string> read_file_to_vector_of_strings(const char* filename, const std::regex delims)
{
	std::vector<std::string> v;
	FileManager fm(filename);
	for (std::string line; std::getline(fm.get(), line); ) {
		std::erase(line, '\r');
		auto const vec = split_string(line, delims);
		v.insert(v.end(), vec.cbegin(), vec.cend());
	}
	return v;
}


inline std::vector<std::string> read_strings_split_on_empty_line(const char* filename)
{
	std::vector<std::string> v;
	FileManager fm(filename);
	std::string element = "";
	for (std::string line; std::getline(fm.get(), line); ) {
		std::erase(line, '\r');
		if (line == "") {
			v.push_back(element);
			element = "";
		}
		else {
			element += line;
		}
	}
	v.push_back(element);
	return v;
}

inline std::vector<std::vector<std::string>> read_groups_of_vectors_split_on_empty_line(const char* filename)
{
	std::vector<std::vector<std::string>> v;
	std::vector<std::string> u;
	FileManager fm(filename);
	for (std::string line; std::getline(fm.get(), line); ) {
		std::erase(line, '\r');
		if (line == "") {
			v.push_back(u);
			u.clear();
		}
		else {
			u.push_back(line);
		}
	}
	v.push_back(u);
	return v;
}
