#include "GRY_mdTohtml.hpp"
#include "GRY_MdMetadata.hpp"
#include "md4c-html-process.hpp"
#include "formatter.hpp"
#include <fstream>
#include <filesystem>
#include <iostream>

#define FILE_BUFFER_SIZE 65535

using filechar_t = std::filesystem::__cxx11::path::value_type;

template <>
int GRY_mdToHTML::mdToHtml(const filechar_t* filename, const filechar_t* root, const GRY_MdMetadata* defaultMetadata) {
	namespace fs = std::filesystem;

	fs::path path(filename);
	fs::path directory(filename);
	directory.remove_filename();

	if (!fs::exists(path) || !fs::is_regular_file(path) || path.extension() != ".md") {
		std::cerr << "Error: Invalid Markdown file: " << path << ".\n";
		return -1;
	}

	std::ifstream fileStream(path);
	GRY_MdMetadata metadata;
	if (defaultMetadata) { metadata = *defaultMetadata; }
	metadata.read(fileStream, directory.c_str(), root);

	char buffer[FILE_BUFFER_SIZE + 1] = { 0 };
	fileStream.read(buffer, FILE_BUFFER_SIZE);
	std::streamsize fileLength = fileStream.gcount();

	md4c_html_args htmlArgs;
	htmlArgs.input = buffer;
	htmlArgs.input_size = fileLength;
	htmlArgs.parser_flags = MD_FLAG_TABLES;
	htmlArgs.user_flags = 0;

	path.replace_extension(".html");

	std::ofstream outFile(path);

	if (writeHTML(outFile, &metadata, &htmlArgs)) {
		std::cerr << "Error writing " << path << ".\n";
		return -1;
	}

	std::cout << path << " written.\n";

	return 0;
}
