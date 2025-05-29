#include "GRY_SSG.hpp"
#include <iostream>

int main(int argc, char** argv) {
	if (argc < 2) {
		std::cout << "Usage: ./site-generator [options] <filename-or-directory>\n";
		std::cout << "Type \"./site-generator -h\" for options.\n";
		return 0;
	}
	if (argv[1][0] == '-') {
		if (argv[1][1] == '\0' || argv[1][2] != '\0') {
			std::cout << "Invalid option " << argv[1] << ".\n";
			return -1;
		}
		if (argv[1][1] == 'h') {
			std::cout << "Options:\n";
			std::cout << "\t-r <directory>\t= Search through directories recursively.\n";
			std::cout << "\t-h\t\t= Display this menu.\n";
			return 0;
		}
		if (argv[1][1] == 'r') {
			if (argc < 3) {
				std::cout << "Error: Missing <directory> argument.\n";
				return -1;
			}
			return GRY_generateSite(argv[2], true);
		}
		std::cout << "Unknown option " << argv[1] << ".\n";
		return -1;
	}

	return GRY_generateSite(argv[1]);
}
