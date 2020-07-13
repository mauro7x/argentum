#ifndef __TUTORIAL_VIEW_H__
#define __TUTORIAL_VIEW_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <iterator>
#include <list>
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../Common/includes/JSON.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "SDL/Mixer.h"
#include "SDL/Renderer.h"
#include "SDL/Texture.h"
#include "SDL/Widgets/Button.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "ConstantRateFunc.h"
#include "contexts.h"
#include "defs.h"
#include "paths.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class TutorialView : public ConstantRateFunc {
   private:
    // Contexto global actual
    Context& current_context;

    // Componentes globales
    const Renderer& renderer;

    // Widgets
    Button goback_btn, prev_btn, next_btn;

    // Offsets de renderizado
    SDL_Rect img_box = {0};

    // Texturas a renderizar
    Texture bg;

    // Lista de imagenes del tutorial
    std::list<Texture> imgs;
    std::list<Texture>::iterator it;

    //-----------------------------------------------------------------------------
    // Métodos privados

    /* Inicializa recursos internos */
    void _init();

    /* Carga la media necesaria */
    void _loadMedia();

    /* Vacía la cola de eventos de SDL */
    void _processSDLEvents();

    /* Renderiza la pantalla */
    void _render() const;

    /* Iteración del loop */
    void _func(const int it) override;

    //-----------------------------------------------------------------------------
    // Auxiliares

    /* Handler de eventos de SDL */
    void _handleEvent(const SDL_Event& e);

    /* Handler de solicitud de volver */
    void _handleGoBackButtonPressed();

    /* Obtiene la posición del mouse */
    SDL_Point _getMousePos(const SDL_Event& e) const;

    /* Verifica si una posicion está dentro de un rectangulo */
    bool _inside(const SDL_Point& pos, const SDL_Rect& box) const;

    //-----------------------------------------------------------------------------

   public:
    /* Constructor */
    TutorialView(Context& current_context, const Renderer& renderer);

    /* Destructor */
    ~TutorialView();
};

//-----------------------------------------------------------------------------

#endif  // __TUTORIAL_VIEW_H__
