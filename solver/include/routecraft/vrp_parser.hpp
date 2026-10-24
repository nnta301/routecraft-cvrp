#pragma once

#include "routecraft/cvrp.hpp"

#include <filesystem>
#include <istream>
#include <stdexcept>
#include <string>

namespace routecraft {

class ParseError final : public std::runtime_error {
public:
    explicit ParseError(const std::string& message);
};

Instance parse_cvrplib(std::istream& input);

Instance parse_cvrplib_file(const std::filesystem::path& path);

}
