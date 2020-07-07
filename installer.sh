#!/bin/bash

#------------------------------------------------------------------------------
# Mensajes

function waitingInputMessage() {
    echo "-------------------------------------------------"
    printf "> Ingrese una opción: "
}

function initialMessage () {
    clear
    echo "================================================="
    echo "=                ARGENTUM ONLINE                ="
    echo "================================================="
    echo ""
}

function helpMessage() {
    echo "Opciones de instalación:"
    echo "  d: instala dependencias necesarias."
    echo "  i: instala el juego (no se instalan dependencias)."
    echo "  a: instala todo (dependencias y juego)."
    echo ""
    echo "Opciones de desinstalación:"
    echo "  u: desinstala el juego."
    echo ""
    echo "Opciones post-instalación:"
    echo "  reset_database: resetea las bases de datos del servidor."
    echo "  script_items: carga un script para que los personajes creados inicien con objetos."
    echo "  script_empty: anula el efecto de los scripts de creación de personajes."
    echo ""
    echo "Otras opciones:"
    echo "  h: muestra este mensaje."
    echo "  m: manual de usuario."
    echo "  q: cerrar."
    echo ""
}

function unknownInput() {
    echo "Opción desconocida (ingrese 'h' para ayuda, 'q' para salir)." 
    echo ""
}

function userManual() {
    echo "=== MANUAL DE USUARIO ==="
    echo "Instalación"
    echo "  * Para comenzar a jugar, es necesario instalar dependencias y el juego een sí. Seleccionando la opción 'a' del menú, se realizará este proceso de forma automática."
    echo "  * Las dependencias que se instalarán serán las necesarias para utilizar la librería gráfica SDL, y CMake para la compilación."
    echo ""
    echo "Ejecución"
    echo "  * Servidor: corriendo 'sudo ao-server port' se abrirá un servidor en el puerto dado. Es necesario ejecutar con sudo ya que el servidor modifica los archivos de la base de datos."
    echo "  * Cliente: corriendo 'argentum' se abrirá el juego."
    echo ""
    echo "Configuración"
    echo "  * Para configurar el cliente, se debe modificar el archivo 'config.json' en la ruta '/etc/argentum/client/config'. Desde aquí se puede modificar el tamaño de la ventana, si se desea jugar en modo fullscreen o modo ventana, y el framerate del mismo."
    echo "  * Para configurar el servidor, podemos acceder a los archivos en la ruta '/etc/argentum/server/config' y modificar el archivo que queramos." 
    echo ""
}

#------------------------------------------------------------------------------
# Compilación e instalación

function build() {
    sudo rm -rf build
    mkdir build
    cd build
    cmake ..
}

function installDependencies() {
    echo "=== INSTALACIÓN DE DEPENDENCIAS ==="
    echo ""
    echo ">> Instalando 'cmake'..."
    sudo apt-get install cmake
    echo ""
    echo ">> Instalando 'libsdl2-dev'..."
    sudo apt-get install libsdl2-dev
    echo ""
    echo ">> Instalando 'libsdl2-image-dev'..."
    sudo apt-get install libsdl2-image-dev
    echo ""
    echo ">> Instalando 'libsdl2-ttf-dev'..."
    sudo apt-get install libsdl2-ttf-dev
    echo ""
    echo ">> Instalando 'libsdl2-mixer-dev'..."
    sudo apt-get install libsdl2-mixer-dev
    echo ""
    echo "Instalación de dependencias finalizada."
    echo ""
}

function installGame() {
    echo "=== INSTALACIÓN DEL JUEGO ==="
    build
    sudo make install -j4
    echo ""
    echo "Instalación del juego finalizada."
    echo ""
}

function all() {
    installDependencies
    installGame
}

function uninstall() {
    echo "=== DESINSTALADOR ==="
    sudo rm -rf build
    sudo rm -rf /etc/argentum
    sudo rm -rf /var/argentum
    sudo rm -rf /usr/bin/ao-server
    sudo rm -rf /usr/bin/argentum
    sudo rm -rf /usr/lib/libao-common.so
    echo ""
    echo "Se ha desinstalado el juego con éxito."
    echo ""
}

#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Scripts post-instalacion

function scriptResetDatabase() {
    echo "=== SCRIPT ==="
    echo "> Se reseteara la base de datos."
    sudo rm -rf /var/argentum/server/assets/Database/playerdata.cfg
    sudo rm -rf /var/argentum/server/assets/Database/playerinfo.cfg
    echo "Se ha cargado el script correctamente."
    echo ""
}

function scriptItems() {
    echo "=== SCRIPT ==="
    echo "> Todos los personajes creados iniciarán con varios items y algo de oro."
    sudo cp -f /etc/argentum/server/config/scripts/new_player_data_with_items.json /etc/argentum/server/config/new_player_data.json
    echo "Se ha cargado el script correctamente."
    echo ""
}

function scriptEmpty() {
    echo "=== SCRIPT ==="
    echo "> Todos los personajes creados iniciarán sin items ni oro."
    sudo cp -f /etc/argentum/server/config/scripts/new_player_data.json /etc/argentum/server/config/new_player_data.json
    echo "Se ha cargado el script correctamente."
    echo ""
}

#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Loop ppal

# exit when any command fails
set -e

initialMessage
helpMessage
waitingInputMessage

while :
do
    read OPTION
    case $OPTION in
        d)  
            echo ""
            installDependencies
            waitingInputMessage
        ;;
        i)
            echo ""
            installGame
            waitingInputMessage
        ;;
        a)
            echo ""
            installDependencies
            installGame
            waitingInputMessage
        ;;
        u)
            echo ""
            uninstall
            waitingInputMessage
        ;;
        reset_database)
            echo ""
            scriptResetDatabase
            waitingInputMessage
        ;;
        script_items)
            echo ""
            scriptItems
            waitingInputMessage
        ;;
        script_empty)
            echo ""
            scriptEmpty
            waitingInputMessage
        ;;
        h)  
            echo ""
            helpMessage
            waitingInputMessage
        ;;
        m)
            echo ""
            userManual
            waitingInputMessage
        ;;
        q)  
            echo ""
            echo "Adiós!"
            echo "-------------------------------------------------"
            exit 0
        ;;
        *)  
            echo ""
            unknownInput
            waitingInputMessage
        ;;
    esac
done

#------------------------------------------------------------------------------
