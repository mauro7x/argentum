#ifndef __PATHS_H__
#define __PATHS_H__

//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Common paths

#define MAPS_FILEPATH "../Config/Common/maps.json"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Client paths

#ifdef __CLIENT_H__
#include "../../Client/includes/paths.h"
#endif  // __CLIENT_H__
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Server paths

#ifdef __SERVER_H__
#include "../../Server/includes/paths.h"
#endif  // __SERVER_H__
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