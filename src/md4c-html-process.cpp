#include "md4c-html-process.hpp"
#include <fstream>

void md4c_html_args::process_ofstream(const MD_CHAR* string, MD_SIZE size, void* data) {
	std::ofstream* outFile = (std::ofstream*)data;

	outFile->write(string, size);
}
