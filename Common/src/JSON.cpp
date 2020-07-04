#include "../includes/JSON.h"

//-----------------------------------------------------------------------------

json JSON::loadJsonFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (file.fail()) {
        throw Exception("Error opening file: %s", filepath.c_str());
    }

    json j;
    file >> j;
    if (file.fail()) {
        throw Exception("Error reading file: %s", filepath.c_str());
    }

    file.close();
    if (file.fail()) {
        throw Exception("Error closing file: %s", filepath.c_str());
    }

    return j;
}

//-----------------------------------------------------------------------------
