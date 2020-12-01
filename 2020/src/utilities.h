#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <fmt/core.h>
#include <fmt/color.h>

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

std::vector<int> read_file_to_vector(const char* filename)
{
	std::vector<int> v;
	FileManager fm(filename);
	for (std::string line; std::getline(fm.get(), line); ) {
		v.push_back(std::stoi(line));
	}
	return v;
}