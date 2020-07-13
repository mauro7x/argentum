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
    echo "=        CONFIGURACIÓN - ARGENTUM ONLINE        ="
    echo "================================================="
    echo ""
}

function helpMessage() {
    echo "Archivos configurables:"
    echo "  1: [CLIENTE] Configuración general."
    echo "  2: [COMÚN] Configuración de los mapas."
    echo "  3: [SERVIDOR] Configuración general."
    echo "  4: [SERVIDOR] Configuración de fórmulas."
    echo "  5: [SERVIDOR] Configuración de razas."
    echo "  6: [SERVIDOR] Configuración de clases."
    echo "  7: [SERVIDOR] Configuración de monstruos."
    echo "  8: [SERVIDOR] Configuración de items."
    echo "  9: [SERVIDOR] Configuración de data inicial."
    echo "  10: [SERVIDOR] Configuración de NPCs."
    echo ""
    echo "Base de datos:"
    echo "  11: [SERVIDOR] Vaciar la base de datos."
    echo ""
    echo "Otras opciones:"
    echo "  r: resetea la configuración a la default."
    echo "  h: muestra este mensaje."
    echo "  q: cerrar."
    echo ""
}

function unknownInput() {
    echo "Opción desconocida (ingrese 'h' para ayuda, 'q' para salir)." 
    echo ""
}

#------------------------------------------------------------------------------
# Edición de archivos

function openWithVi() {
    echo "> Modificando archivo de configuración..."
    sudo vi $1
    echo ""
}

#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Scripts

function resetDatabase() {
    sudo rm -rf /var/argentum/server/assets/Database/playerdata.cfg
    sudo rm -rf /var/argentum/server/assets/Database/playerinfo.cfg
    echo "Se ha vaciado la base de datos correctamente."
    echo ""
}

function resetDefault() {
    # config del server
    sudo rm -rf /etc/argentum/server/config
    sudo cp -rf Server/config /etc/argentum/server
    
    # config común
    sudo rm -rf /etc/argentum/common/config
    sudo cp -rf Common/config /etc/argentum/common

    #config del cliente
    sudo rm -rf /etc/argentum/client/config
    sudo cp -rf Client/config /etc/argentum/client    

    echo "Se reseteo la configuración por defecto."
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
        1)  
            echo ""
            openWithVi "/etc/argentum/client/config/config.json"
            waitingInputMessage
        ;;
        2)
            echo ""
            openWithVi "/etc/argentum/common/config/maps.json"
            waitingInputMessage
        ;;
        3)
            echo ""
            openWithVi "/etc/argentum/server/config/config.json"
            waitingInputMessage
        ;;
        4)
            echo ""
            openWithVi "/etc/argentum/server/config/formulas_modifiers.json"
            waitingInputMessage
        ;;
        5)
            echo ""
            openWithVi "/etc/argentum/server/config/races.json"
            waitingInputMessage
        ;;
        6)
            echo ""
            openWithVi "/etc/argentum/server/config/kinds.json"
            waitingInputMessage
        ;;
        7)
            echo ""
            openWithVi "/etc/argentum/server/config/creatures.json"
            waitingInputMessage
        ;;
        8)
            echo ""
            openWithVi "/etc/argentum/server/config/items.json"
            waitingInputMessage
        ;;
        9)
            echo ""
            openWithVi "/etc/argentum/server/config/new_player_data.json"
            waitingInputMessage
        ;;
        10)
            echo ""
            openWithVi "/etc/argentum/server/config/npcs.json"
            waitingInputMessage
        ;;
        11)
            echo ""
            resetDatabase
            waitingInputMessage
        ;;
        r)
            echo ""
            resetDefault
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
