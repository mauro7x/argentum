#include "../includes/JSON.h"

//-----------------------------------------------------------------------------

json JSON::loadJsonFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (file.fail()) {
        throw Exception("Error opening file: %s", filepath);
    }

    json j;
    file >> j;
    if (file.fail()) {
        throw Exception("Error reading file: %s", filepath);
    }

    file.close();
    if (file.fail()) {
        throw Exception("Error closing file: %s", filepath);
    }

    return j;
}

//-----------------------------------------------------------------------------
