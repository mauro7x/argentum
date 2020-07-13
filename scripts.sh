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
    echo "=           SCRIPTS - ARGENTUM ONLINE           ="
    echo "================================================="
    echo ""
}

function helpMessage() {
    echo "Scripts sobre creación de personaje:"
    echo "  0: Deshabilitar todos los scripts de creación de personaje."
    echo "  1: Crear personajes nuevos con items."
    echo ""
    echo "Otras opciones:"
    echo "  h: muestra este mensaje."
    echo "  q: cerrar."
    echo ""
}

function unknownInput() {
    echo "Opción desconocida (ingrese 'h' para ayuda, 'q' para salir)." 
    echo ""
}

#------------------------------------------------------------------------------
# Scripts

function resetItemScripts() {
    echo "Ejecutando script..."
    sudo cp -f /etc/argentum/server/config/scripts/new_player_data.json /etc/argentum/server/config/new_player_data.json
    echo "Se ha ejecutado el script correctamente."
    echo ""
}

function scriptItems1() {
    echo "Ejecutando script..."
    sudo cp -f /etc/argentum/server/config/scripts/new_player_data_with_items.json /etc/argentum/server/config/new_player_data.json
    echo "Se ha ejecutado el script correctamente."
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
        0)  
            echo ""
            resetItemScripts
            waitingInputMessage
        ;;
        1)
            echo ""
            scriptItems1
            waitingInputMessage
        ;;
        h)  
            echo ""
            helpMessage
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
