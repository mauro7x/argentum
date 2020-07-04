#!/bin/bash

echo "Bienvenido al desinstalador de Argentum Online!"
echo ""

echo "> Se necesitan permisos sudo para desinstalar:"
sudo rm -rf /etc/argentum
sudo rm -rf /var/argentum
sudo rm -rf /usr/bin/ao-server
sudo rm -rf /usr/bin/ao-client
sudo rm -rf /usr/lib/libao-common.so
sudo rm -rf build
echo ""

echo "Argentum Online desinstalado con éxito."
