#include <filesystem>
#include <fstream>
#include <iostream>

void read_file_to_str(const char* filename, std::string& destination)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);

	in.seekg(0, std::ios::end);
	destination.resize(in.tellg());
	in.seekg(0, std::ios::beg);
	in.read(&destination[0], destination.size());
	in.close();
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cerr << "Usage: prog [filename]" << std::endl;
		exit(1);
	}

	if (!std::filesystem::exists(argv[1]))
	{
		std::cerr << "File: " << argv[1] << " does not exist: " << std::endl;
	}

	std::string source;

	read_file_to_str(argv[1], source);
}
