#pragma once
#include <iosfwd>

struct GRY_MdMetadata {
	enum Key {
		Type,
		Title,
		Stylesheet,
		Description,
		Author,
		Header,
		Footer,
		SIZE
	};

	const char* data[Key::SIZE] = { 0 };

	/**
	 * @brief Constructor.
	 * 
	 */
	GRY_MdMetadata();

	/**
	 * @brief Destructor.
	 * 
	 */
	~GRY_MdMetadata();

	GRY_MdMetadata(const GRY_MdMetadata& other);

	GRY_MdMetadata& operator=(const GRY_MdMetadata& other);

	friend void swap(GRY_MdMetadata& lhs, GRY_MdMetadata& rhs);

	GRY_MdMetadata(GRY_MdMetadata&& other) noexcept;

	/**
	 * @brief Get the value of a meta data entry.
	 * 
	 * @param key Key for meta data value to retrieve
	 * @return Value of the meta data, may be NULL
	 */
	const char* get(Key key) const;

	/**
	 * @brief Set the value of a meta data entry.
	 * 
	 * @param key Key for the meta data value to set
	 * @param value Value of the meta data, will be copied
	 */
	void set(Key key, const char* value);

	/**
	 * @brief Read meta data from `file`.
	 * 
	 * @param file Input file stream of the markdown file
	 * @param mode Reading mode that determines which values will be overwritten
	 * @return 0 on success, or -1 on error
	 */
	template <typename CHAR_T>
	int read(std::ifstream& file, const CHAR_T* directory, const CHAR_T* root, bool overwrite = true);
};
