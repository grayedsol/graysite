#pragma once
#include "md4c-html.h"

struct md4c_html_args {
	const MD_CHAR* input;
	MD_SIZE input_size;
	unsigned parser_flags;
	unsigned user_flags;

	static void process_ofstream(const MD_CHAR* string, MD_SIZE size, void* data);
};
