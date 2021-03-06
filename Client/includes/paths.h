#ifndef __CLIENT_PATHS_H__
#define __CLIENT_PATHS_H__

//-----------------------------------------------------------------------------
#include "../../Common/includes/paths.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Config files

#define CONFIG_FILEPATH "client/config/config.json"
#define VIEWS_CONFIG_FILEPATH "client/config/views_config.json"
#define UNIT_SPRITES_FILEPATH "client/config/unit_sprites.json"
#define ITEM_SPRITES_FILEPATH "client/config/item_sprites.json"
#define SIGNUP_DATA_FILEPATH "client/config/signup_data.json"
#define AUDIO_FILEPATH "client/config/audio.json"
#define EFFECTS_FILEPATH "client/config/effects.json"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Icono de la ventana

#define WINDOW_ICON_FILEPATH "client/assets/Window/icon.png"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// HomeView assets

#define HOMEVIEW_BG_FP "client/assets/Views/HomeView/HomeView.png"
#define HOMEVIEW_TEXTBOX_FP "client/assets/Views/HomeView/BarraNoClickeada.png"
#define HOMEVIEW_TEXTBOX_ACTIVE_FP \
    "client/assets/Views/HomeView/BarraClickeada.png"
#define HOMEVIEW_TUTORIAL_BUTTON_FP \
    "client/assets/Views/HomeView/BotonTutorial.png"
#define HOMEVIEW_TUTORIAL_BUTTON_PRESSED_FP \
    "client/assets/Views/HomeView/BotonTutorialPresionado.png"
#define HOMEVIEW_CONNECT_BUTTON_FP \
    "client/assets/Views/HomeView/BotonConectar.png"
#define HOMEVIEW_CONNECT_BUTTON_PRESSED_FP \
    "client/assets/Views/HomeView/BotonConectarPresionado.png"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// TutorialView assets

#define TUTORIALVIEW_BG_FP "client/assets/Views/TutorialView/TutorialView.png"
#define TUTORIALVIEW_GOBACK_BUTTON_FP \
    "client/assets/Views/TutorialView/BotonVolver.png"
#define TUTORIALVIEW_GOBACK_BUTTON_PRESSED_FP \
    "client/assets/Views/TutorialView/BotonVolverPresionado.png"
#define TUTORIALVIEW_PREV_BUTTON_FP \
    "client/assets/Views/TutorialView/BotonPrev.png"
#define TUTORIALVIEW_PREV_BUTTON_PRESSED_FP \
    "client/assets/Views/TutorialView/BotonPrevPresionado.png"
#define TUTORIALVIEW_NEXT_BUTTON_FP \
    "client/assets/Views/TutorialView/BotonNext.png"
#define TUTORIALVIEW_NEXT_BUTTON_PRESSED_FP \
    "client/assets/Views/TutorialView/BotonNextPresionado.png"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ConnectionView assets

#define CONNECTIONVIEW_BG_FP \
    "client/assets/Views/ConnectionView/ConnectionView.png"
#define CONNECTIONVIEW_TEXTBOX_FP \
    "client/assets/Views/ConnectionView/BarraNoClickeada.png"
#define CONNECTIONVIEW_TEXTBOX_ACTIVE_FP \
    "client/assets/Views/ConnectionView/BarraClickeada.png"
#define CONNECTIONVIEW_PLAY_BUTTON_FP \
    "client/assets/Views/ConnectionView/BotonJugar.png"
#define CONNECTIONVIEW_PLAY_BUTTON_PRESSED_FP \
    "client/assets/Views/ConnectionView/BotonJugarPresionado.png"
#define CONNECTIONVIEW_SIGNUP_BUTTON_FP \
    "client/assets/Views/ConnectionView/BotonCrear.png"
#define CONNECTIONVIEW_SIGNUP_BUTTON_PRESSED_FP \
    "client/assets/Views/ConnectionView/BotonCrearPresionado.png"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SignUpView assets

#define SIGNUPVIEW_BG_FP "client/assets/Views/SignUpView/SignUpView.png"
#define SIGNUPVIEW_TEXTBOX_FP \
    "client/assets/Views/SignUpView/BarraNoClickeada.png"
#define SIGNUPVIEW_TEXTBOX_ACTIVE_FP \
    "client/assets/Views/SignUpView/BarraClickeada.png"

#define SIGNUPVIEW_SELECTIONBOX_BAR_FP \
    "client/assets/Views/SignUpView/BarraSoloLectura.png"
#define SIGNUPVIEW_SELECTIONBOX_SMALL_BAR_FP \
    "client/assets/Views/SignUpView/BarraSoloLecturaChica.png"

#define SIGNUPVIEW_SELECTIONBOX_PREV_FP \
    "client/assets/Views/SignUpView/BotonPrev.png"
#define SIGNUPVIEW_SELECTIONBOX_PREV_PRESSED_FP \
    "client/assets/Views/SignUpView/BotonPrevPresionado.png"

#define SIGNUPVIEW_SELECTIONBOX_NEXT_FP \
    "client/assets/Views/SignUpView/BotonNext.png"
#define SIGNUPVIEW_SELECTIONBOX_NEXT_PRESSED_FP \
    "client/assets/Views/SignUpView/BotonNextPresionado.png"

#define SIGNUPVIEW_GOBACK_BUTTON_FP \
    "client/assets/Views/SignUpView/BotonVolver.png"
#define SIGNUPVIEW_GOBACK_BUTTON_PRESSED_FP \
    "client/assets/Views/SignUpView/BotonVolverPresionado.png"
#define SIGNUPVIEW_CREATE_BUTTON_FP \
    "client/assets/Views/SignUpView/BotonCrear.png"
#define SIGNUPVIEW_CREATE_BUTTON_PRESSED_FP \
    "client/assets/Views/SignUpView/BotonCrearPresionado.png"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// HUD

#define HUD_UPPER_LAYER_FP "client/assets/HUD/UpperLayer.png"
#define HUD_CONSOLE_BASE_FP "client/assets/HUD/Console/base.png"
#define HUD_USER_INFO_BASE_FP "client/assets/HUD/UserInfo/base.png"
#define HUD_USER_INVENTORY_BASE_FP "client/assets/HUD/UserInventory/base.png"
#define HUD_USER_INVENTORY_SELECTED_FP \
    "client/assets/HUD/UserInventory/selected.png"
#define HUD_USER_STATS_BASE_FP "client/assets/HUD/UserStats/base.png"
#define HUD_PROGRESSBAR_BASE_FP "client/assets/HUD/ProgressBar/base.png"
#define HUD_PROGRESSBAR_BISEL_FP "client/assets/HUD/ProgressBar/bisel.png"
#define HUD_PROGRESSBAR_BORDER_FP \
    "client/assets/HUD/ProgressBar/black_border.png"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Fonts (se dejan comentadas las fuentes viejas por si queremos volver a
// utilizarlas en un futuro)

#define FONT_OLDLONDON_FP "client/assets/Fonts/OldLondon.ttf"
#define FONT_AUGUSTA_FP "client/assets/Fonts/Augusta.ttf"
// #define FONT_DUNGEON_FP "client/assets/Fonts/Dungeon.ttf"
// #define FONT_GAMEOVER_FP "client/assets/Fonts/GameOver.ttf"
// #define FONT_CINZELREGULAR_FP "client/assets/Fonts/CinzelRegular.ttf"
#define FONT_CINZELBOLD_FP "client/assets/Fonts/CinzelBold.ttf"
#define FONT_SANFORD_FP "client/assets/Fonts/Sanford.ttf"
// #define FONT_ANCIENTMEDIUM_FP "client/assets/Fonts/AncientMedium.ttf"
#define FONT_FORCEDSQUARE_FP "client/assets/Fonts/ForcedSquare.ttf"
#define FONT_TIMELESS_FP "client/assets/Fonts/Timeless.ttf"
//-----------------------------------------------------------------------------

#endif  // __CLIENT_PATHS_H__
