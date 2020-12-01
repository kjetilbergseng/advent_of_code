#pragma once
#include <fstream>
#include <string>
#include <vector>

class FileManager {
public:
	FileManager(const std::string& filename) {
		m_file.open(filename, std::ifstream::in);
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