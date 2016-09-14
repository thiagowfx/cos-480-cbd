#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char *argv[]) {
	if (argc < 3) {
		std::cerr << "usage: " << argv[0] << " " << "<input file (text)> <output file (binary)>" << std::endl;
		return EXIT_FAILURE;
	}

	std::ifstream input(argv[1]);
	std::ofstream output(argv[2]);

	if(!output) {
		std::cerr << "error: cannot open output file for writing: " << argv[2] << std::endl;
	}

	while(input) {
		std::string line;
		std::getline(input, line);
		output << line << std::endl;
	}

	output.close();
	input.close();

	return EXIT_SUCCESS;
}
