#include "GRY_SSG.hpp"
#include "GRY_mdTohtml.hpp"
#include "GRY_MdMetadata.hpp"
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>

using filechar_t = std::filesystem::__cxx11::path::value_type;

static const char* defaultTxt = "Default.txt";

int GRY_generateSite(std::filesystem::path path, std::filesystem::path root, const GRY_MdMetadata* defaultMetadata, bool recursive);

int GRY_generateSiteDir(std::filesystem::path directory, std::filesystem::path root, bool recursive) {
	namespace fs = std::filesystem;

	int retVal = 0;

	GRY_MdMetadata metadata;
	const GRY_MdMetadata* metadataPtr = nullptr;
	fs::path metadataPath = directory / defaultTxt;
	if (fs::exists(metadataPath) && fs::is_regular_file(metadataPath)) {
		std::ifstream metadataFile(metadataPath);
		if (metadataFile.good()) {
			metadata.read(metadataFile, directory.c_str(), root.c_str());
			metadataPtr = &metadata;
		}
		else {
			std::cerr << "Failed to open default metadata file: \"" << metadataPath << "\".\n";
			std::cerr << "Continuing without using it.\n";
		}
	}

	for (const auto& item : fs::directory_iterator(directory)) {
		if ((fs::is_directory(item.status()) && recursive) ||
			(fs::is_regular_file(item.status()) && item.path().extension() == ".md")) {
			if (GRY_generateSite(item.path(), root, metadataPtr, recursive)) {
				retVal = -1;
			}
		}
	}

	return retVal;
}

int GRY_generateSite(std::filesystem::path path, std::filesystem::path root, const GRY_MdMetadata* defaultMetadata, bool recursive) {
	namespace fs = std::filesystem;

	if (!fs::exists(path)) {
		std::cerr << "Error: Directory or file " << path << " did not exist.\n";
		return -1;
	}

	if (fs::is_regular_file(path) && path.extension() == ".md") {
		return GRY_mdToHTML::mdToHtml(path.c_str(), root.c_str(), defaultMetadata);
	}

	if (!fs::is_directory(path)) {
		std::cerr << "Error: " << path << " was not a valid .md file or a directory.\n";
		return -1;
	}

	return GRY_generateSiteDir(path, root, recursive);
}

template <>
int GRY_generateSite(const filechar_t* path, bool recursive) {
	namespace fs = std::filesystem;

	fs::path root(path);
	if (!fs::is_directory(root)) {
		root.remove_filename();
	}

	return GRY_generateSite(fs::path(path), root, nullptr, recursive);
}
