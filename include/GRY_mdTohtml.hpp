#pragma once

struct GRY_MdMetadata;

namespace GRY_mdToHTML {
	/**
	 * @brief Write an HTML conversion of a Markdown file.
	 * 
	 * @param filename Path to Markdown file to convert
	 * @param defaultMetadata Default metadata to use, if any
	 * @return 0 on success, -1 on failure.
	 */
	int mdToHtml(const char* filename, const GRY_MdMetadata* defaultMetadata = nullptr);

	/**
	 * @copydoc mdToHtml
	 */
	int mdToHtml(const wchar_t* filename, const GRY_MdMetadata* defaultMetadata = nullptr);
};
