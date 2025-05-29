#include "GRY_SSG.hpp"

int main(int argc, char** argv) {
	const char* path = argc < 2 ? "." : argv[1];

	GRY_generateSite(path);

	return 0;
}
