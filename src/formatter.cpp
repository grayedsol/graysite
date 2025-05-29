#include "formatter.hpp"
#include "GRY_MdMetadata.hpp"
#include "md4c-html-process.hpp"
#include <fstream>
#include <iostream>
#include <string>


int writeHead(std::ofstream& file, const GRY_MdMetadata* metadata) {
	int retVal = 0;

	file << "<head>\n";

	if (metadata->get(GRY_MdMetadata::Key::Title)) {
		file << "<title>" << metadata->get(GRY_MdMetadata::Key::Title) << "</title>\n";
	}

	if (metadata->get(GRY_MdMetadata::Key::Stylesheet)) {
		file << "<link rel=\"stylesheet\" href=\"" << metadata->get(GRY_MdMetadata::Key::Stylesheet) << "\">\n";
	}

	if (metadata->get(GRY_MdMetadata::Key::Description)) {
		file << "<meta name=\"description\" content=\"" << metadata->get(GRY_MdMetadata::Key::Description) << "\">\n";
	}

	if (metadata->get(GRY_MdMetadata::Key::Author)) {
		file << "<meta name=\"author\" content=\"" << metadata->get(GRY_MdMetadata::Key::Author) << "\">\n";
	}

	file << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";

	file << "</head>\n\n";

	return retVal;
}

int writeMain(std::ofstream& file, const GRY_MdMetadata* metadata, const md4c_html_args* htmlArgs) {
	int retVal = 0;

	file << "<main>\n";

	if (md_html(htmlArgs->input,
				htmlArgs->input_size,
				htmlArgs->process_ofstream,
				(void*)&file,
				htmlArgs->parser_flags,
				htmlArgs->user_flags)
		) {
		std::cerr << "md_html error.\n";
		retVal = -1;
	}

	file << "</main>\n\n";

	return retVal;
}

int writeBody(std::ofstream& file, const GRY_MdMetadata* metadata, const md4c_html_args* htmlArgs) {
	int retVal = 0;

	file << "<body>\n";

	if (metadata->get(GRY_MdMetadata::Key::Header)) {
		file << metadata->get(GRY_MdMetadata::Key::Header);
		file << '\n';
	}

	if (writeMain(file, metadata, htmlArgs)) {
		std::cerr << "Error writing <main>.\n";
		retVal = -1;
	}

	if (metadata->get(GRY_MdMetadata::Key::Footer)) {
		file << metadata->get(GRY_MdMetadata::Key::Footer);
		file << '\n';
	}

	file << "</body>\n\n";

	return retVal;
}

int writeHTML(std::ofstream& file, const GRY_MdMetadata* metadata, const md4c_html_args* htmlArgs) {
	int retVal = 0;

	file << "<!DOCTYPE html>\n";

	file << "<html>\n";

	if (writeHead(file, metadata)) {
		std::cerr << "Error writing <head>.\n";
		retVal = -1;
	}

	if (writeBody(file, metadata, htmlArgs)) {
		std::cerr << "Error writing <body>.\n";
		retVal = -1;
	}

	file << "</html>\n";

	return retVal;
}
