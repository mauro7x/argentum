# =============================================================================
# Trabajo Práctico Final - Taller de Programación I
# =============================================================================

#------------------------------------------------------------------------------
# CONFIGURACION DEL PROYECTO

cmake_minimum_required(VERSION 2.8.12)
project(argentum)
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# FLAGS DE COMPILACION

# Estándar a utilizar
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Flags para el manejo de errores
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic -pedantic-errors")

# Optimizaciones
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# Debugging
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -DDEBUG -fno-inline")
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# ARCHIVOS FUENTE

FILE(GLOB_RECURSE common_src "Common/src/*.cpp")
FILE(GLOB_RECURSE client_src "Client/src/*.cpp")
FILE(GLOB_RECURSE server_src "Server/*.cpp")
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# TARGETS

# Common
add_library(ao-common SHARED ${common_src})

# Client
add_executable(ao-client ${client_src})
target_link_libraries(ao-client ao-common SDL2 SDL2_image SDL2_ttf SDL2_mixer)

# Server
add_executable(ao-server ${server_src})
target_link_libraries(ao-server ao-common)
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# INSTALACION

# Rutas
set(BINARIES_DIRPATH "/usr/bin")
set(SHAREDLIBS_DIRPATH "/usr/lib")
set(CONFIG_DIRPATH "/etc/argentum")
set(ASSETS_DIRPATH "/var/argentum")

# Common
install(TARGETS ao-common DESTINATION ${SHAREDLIBS_DIRPATH})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/Common/config DESTINATION ${CONFIG_DIRPATH}/common)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/Common/assets DESTINATION ${ASSETS_DIRPATH}/common)

# Cliente
install(TARGETS ao-client DESTINATION ${BINARIES_DIRPATH})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/Client/config DESTINATION ${CONFIG_DIRPATH}/client)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/Client/assets DESTINATION ${ASSETS_DIRPATH}/client)

# Server
install(TARGETS ao-server DESTINATION ${BINARIES_DIRPATH})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/Server/config DESTINATION ${CONFIG_DIRPATH}/server)
install(DIRECTORY DESTINATION ${ASSETS_DIRPATH}/server/assets/Database)
#------------------------------------------------------------------------------
