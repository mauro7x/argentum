#include "../includes/paths.h"

//-----------------------------------------------------------------------------
// Directorios absolutos

static const char* config_prefix = "/etc/argentum/";
static const char* asset_prefix = "/var/argentum/";
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

std::string paths::config(const char* relative_path) {
    std::string path;
    path += config_prefix;
    path += relative_path;
    return path;
}

std::string paths::asset(const char* relative_path) {
    std::string path;
    path += asset_prefix;
    path += relative_path;
    return path;
}

//-----------------------------------------------------------------------------
