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
int GRY_generateSite(const CHAR_T* path, const GRY_MdMetadata* defaultMetadata, bool recursive);

int GRY_generateSiteDir(std::filesystem::path directory, bool recursive) {
	namespace fs = std::filesystem;

	int retVal = 0;

	GRY_MdMetadata metadata;
	const GRY_MdMetadata* metadataPtr = nullptr;
	fs::path metadataPath = directory / defaultTxt;
	if (fs::exists(metadataPath) && fs::is_regular_file(metadataPath)) {
		std::ifstream metadataFile(metadataPath);
		if (metadataFile.good()) {
			metadata.read(metadataFile, directory.c_str());
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
			if (GRY_generateSite(item.path().c_str(), metadataPtr, recursive)) {
				retVal = -1;
			}
		}
	}

	return retVal;
}

template <typename CHAR_T>
int GRY_generateSite(const CHAR_T* path, const GRY_MdMetadata* defaultMetadata, bool recursive) {
	namespace fs = std::filesystem;

	fs::path root(path);
	if (!fs::exists(root)) {
		std::cerr << "Error: Directory or file " << root << " did not exist.\n";
		return -1;
	}

	if (fs::is_regular_file(root) && root.extension() == ".md") {
		return GRY_mdToHTML::mdToHtml(root.c_str(), defaultMetadata);
	}

	if (!fs::is_directory(root)) {
		std::cerr << "Error: " << root << " was not a valid .md file or a directory.\n";
		return -1;
	}

	return GRY_generateSiteDir(root, recursive);
}

template <typename CHAR_T>
int GRY_generateSite(const CHAR_T* path, bool recursive) {
	return GRY_generateSite(path, nullptr, recursive);
}

template int GRY_generateSite(const char* path, bool recursive);
template int GRY_generateSite(const wchar_t* path, bool recursive);
template int GRY_generateSite(const char* path, const GRY_MdMetadata* defaultMetadata, bool recursive);
template int GRY_generateSite(const wchar_t* path, const GRY_MdMetadata* defaultMetadata, bool recursive);
