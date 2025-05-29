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
	template<typename CHAR_T>
	int mdToHtml(const CHAR_T* filename, const CHAR_T* root, const GRY_MdMetadata* defaultMetadata = nullptr);
};
