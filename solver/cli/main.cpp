#include "routecraft/version.hpp"

#include <iostream>
#include <string_view>

namespace {

void print_help(std::ostream& stream) {
    stream << "Usage: routecraft [--help|-h|--version]\n"
           << "\n"
           << "Available commands:\n"
           << "  --help, -h   Show this help message.\n"
           << "  --version    Show the scaffold version.\n"
           << "\n"
           << "The CVRP solver is not implemented yet.\n";
}

int usage_error(std::string_view message) {
    std::cerr << "Error: " << message << "\n";
    print_help(std::cerr);
    return 2;
}

}  // namespace

int main(int argc, char* argv[]) {
    if (argc == 1) {
        print_help(std::cerr);
        return 2;
    }

    if (argc != 2) {
        return usage_error("expected exactly one option");
    }

    const std::string_view argument{argv[1]};
    if (argument == "--help" || argument == "-h") {
        print_help(std::cout);
        return 0;
    }
    if (argument == "--version") {
        std::cout << "routecraft " << routecraft::version() << "\n";
        return 0;
    }
    if (argument == "solve") {
        return usage_error("the solve command is not implemented");
    }

    return usage_error("unsupported option or command");
}
