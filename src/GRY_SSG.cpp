#include "GRY_SSG.hpp"
#include "GRY_mdTohtml.hpp"
#include "GRY_MdMetadata.hpp"
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>

static const char* defaultTxt = "Default.txt";

template <typename CHAR_T>
int GRY_generateSite(const CHAR_T* directory) {
	namespace fs = std::filesystem;
	bool defaultMetadataExists = false;

	fs::path rootDir(directory);
	GRY_MdMetadata defaultMetadata;
	fs::path defaultFilePath = rootDir / defaultTxt;
	if (fs::exists(defaultFilePath) && fs::is_regular_file(defaultFilePath)) {
		std::ifstream defaultFile(defaultFilePath);
		if (defaultFile.good()) {
			defaultMetadata.read(defaultFile, directory);
			defaultMetadataExists = true;
		}
		else {
			std::cerr << "Failed to open default metadata file: \"" << defaultFilePath << "\".\n";
			std::cerr << "Continuing without using it.\n";
		}
	}

	const GRY_MdMetadata* metadataPtr = defaultMetadataExists ? &defaultMetadata : nullptr;
	if (fs::exists(rootDir) && fs::is_directory(rootDir)) {
		for (const auto& item : fs::directory_iterator(rootDir)) {
			if (fs::is_directory(item.status())) {
				std::basic_string str(item.path().c_str());
				GRY_generateSite(item.path().c_str());
			}
			else if (fs::is_regular_file(item.status())) {
				if (item.path().extension() == ".md") {
					GRY_mdToHTML::mdToHtml(item.path().c_str(), metadataPtr);
				}
			}
		}
	}
	else {
		std::cerr << "Error: Directory " << rootDir << " did not exist.\n";
		return 1;
	}

	return 0;
}

template int GRY_generateSite(const char* directory);
template int GRY_generateSite(const wchar_t* directory);
