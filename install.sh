#!/bin/bash

echo "Bienvenido al instalador del Argentum Online!"
echo ""

echo "> Se necesitan permisos sudo para la instalacion:"
sudo rm -rf build
mkdir build
cd build
echo ""

echo "> Generando CMakeFiles..."
cmake ..
echo ""

echo "> Instalando..."
sudo make install -j4
echo ""

echo "Instalación finalizada!"
echo "> Para abrir un servidor, ejecute: 'sudo ao-server <puerto>'."
echo "> Para iniciar un cliente, ejecute: 'ao-client'."
