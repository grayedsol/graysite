#include "GRY_MdMetadata.hpp"
#include <string.h>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <fstream>
#include <limits>
#include <filesystem>

#define TOKEN_SIZE 127

static const char* GRY_MdMetadataStrings[GRY_MdMetadata::Key::SIZE] = {
	"Type",
	"Title",
	"Stylesheet",
	"Description",
	"Author",
	"Header",
	"Footer"
};

static int GRY_MdMetadataAssign(GRY_MdMetadata* data, const char* key, const char* value, bool overwrite) {
	for (int i = 0; i < GRY_MdMetadata::Key::SIZE; i++) {
		if (strcmp(GRY_MdMetadataStrings[i], key) == 0) {
			if (!overwrite && data->data[i]) { return 0; }

			delete[] data->data[i];
			data->data[i] = strcpy(new char[strlen(value) + 1], value);
			return 0;
		}
	}
	return -1;
}

static int GRY_MdMetadataLoad(GRY_MdMetadata* data, const char* key, std::filesystem::path path, bool overwrite) {
	for (int i = 0; i < GRY_MdMetadata::Key::SIZE; i++) {
		if (strcmp(GRY_MdMetadataStrings[i], key) == 0) {
			if (!overwrite && data->data[i]) { return 0; }

			delete[] data->data[i];
			data->data[i] = NULL;
			std::ifstream inputFile(path);
			if (inputFile.fail()) {
				std::cerr << "Error opening file \"" << path << "\".\n";
				return -1;
			}
			inputFile.ignore(std::numeric_limits<std::streamsize>::max());
			std::streamsize length = inputFile.gcount();
			inputFile.clear();
			inputFile.seekg(0, std::ios_base::beg);
			char* newString = new char[length+1];
			inputFile.read(newString, length);
			newString[length] = '\0';
			data->data[i] = newString;
			return 0;
		}
	}
	return -1;
}

GRY_MdMetadata::GRY_MdMetadata() {
	for (int i = 0; i < Key::SIZE; i++) {
		data[i] = NULL;
	}
}

GRY_MdMetadata::~GRY_MdMetadata() {
	for (int i = 0; i < Key::SIZE; i++) {
		delete[] data[i];
	}
}

GRY_MdMetadata::GRY_MdMetadata(const GRY_MdMetadata& other) {
	for (int i = 0; i < Key::SIZE; i++) {
		this->data[i] = strcpy(new char[strlen(other.data[i]) + 1], other.data[i]);
	}
}

GRY_MdMetadata& GRY_MdMetadata::operator=(const GRY_MdMetadata& other) {
	for (int i = 0; i < Key::SIZE; i++) {
		if (!other.data[i]) { continue; }
		delete[] this->data[i];
		this->data[i] = strcpy(new char[strlen(other.data[i]) + 1], other.data[i]);
	}
	return *this;
}

void swap(GRY_MdMetadata &lhs, GRY_MdMetadata &rhs) {
	using std::swap;
	swap(lhs.data, rhs.data);
}

GRY_MdMetadata::GRY_MdMetadata(GRY_MdMetadata&& other) noexcept {
	swap(*this, other);
}

const char* GRY_MdMetadata::get(Key key) const {
	return data[key];
}

void GRY_MdMetadata::set(Key key, const char* value) {
	delete[] data[key];
	data[key] = strcpy(new char[strlen(value) + 1], value);
}

template <typename CHAR_T>
int GRY_MdMetadata::read(std::ifstream& file, const CHAR_T* directory, bool overwrite) {
	namespace fs = std::filesystem;

	std::string key;
	char value[TOKEN_SIZE + 1];

	while (file.good()) {
		if (file.peek() == '\n') { break; }
		file >> key;
		if (key == "---") {
			std::cerr << "No YAML support yet.\n";
			return -1;
		}
		if (file.get() != ' ') { break; }
		file.getline(value, TOKEN_SIZE);

		// This means there is no metadata or they didn't use a colon
		if (!key.ends_with(':')) {
			return 0;
		}

		key.pop_back();

		if (value[0] == '\"' && value[strlen(value) - 1] == '\"') {
			value[strlen(value) - 1] = '\0';
			fs::path path = fs::path(directory) / (value + 1);
			if (GRY_MdMetadataLoad(this, key.c_str(), path, overwrite)) {
				std::cerr << "Metadata loading error for key-value pair: " << key << ": " << value + 1 << '\n';
				return -1;
			}
		}
		else {
			if (GRY_MdMetadataAssign(this, key.c_str(), value, overwrite)) {
				std::cerr << "Metadata error for key-value pair: " << key << ": " << value << '\n';
				return -1;
			}
		}
	}

	return 0;
}

template int GRY_MdMetadata::read(std::ifstream& file, const char* directory, bool overwrite);
template int GRY_MdMetadata::read(std::ifstream& file, const wchar_t* directory, bool overwrite);
