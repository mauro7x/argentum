#ifndef __PATHS_H__
#define __PATHS_H__

//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Common paths

#define MAPS_FILEPATH "common/config/maps.json"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

namespace paths {

/* Traduce un path relativo de configuración a uno absoluto */
std::string config(const char* relative_path);

/* Traduce un path relativo de asset a uno absoluto */
std::string asset(const char* relative_path);

}  // namespace paths

//-----------------------------------------------------------------------------

#endif  // __PATHS_H__