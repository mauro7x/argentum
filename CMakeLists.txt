# =============================================================================
# Trabajo Práctico Final - Taller de Programación I
# =============================================================================

#------------------------------------------------------------------------------
# CONFIGURACIÓN DEL PROYECTO

cmake_minimum_required(VERSION 2.8.12)
project(skeleton)
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# FLAGS DE COMPILACIÓN

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
# LIBRERÍAS NECESARIAS

add_library(common SHARED ${common_src})

# Threads
# agregar
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# CLIENTE

add_executable(client ${client_src})
target_link_libraries(client common SDL2 SDL2_image SDL2_ttf SDL2_mixer)
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# SERVIDOR
add_executable(server ${server_src})
target_link_libraries(server common)
#------------------------------------------------------------------------------
