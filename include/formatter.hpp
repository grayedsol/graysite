#pragma once
#include <iosfwd>

struct GRY_MdMetadata;
struct md4c_html_args;

int writeHTML(std::ofstream& file, const GRY_MdMetadata* metadata, const md4c_html_args* htmlArgs);
