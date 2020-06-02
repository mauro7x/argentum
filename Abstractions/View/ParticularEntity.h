#ifndef __PARTICULAR_ENTITY_H__
#define __PARTICULAR_ENTITY_H__

//-----------------------------------------------------------------------------
#include "Entity.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ParticularEntity : public Entity {
   private:
    // atributos extra
    // metodos privados

   public:
    /* Constructor */
    ParticularEntity(SDL_Renderer* renderer);

    //-------------------------------------------------------------------------
    // Interfaz genérica de Entidad

    /* Inicializar la media necesaria */
    void loadMedia() override;

    /* Manejar evento del usuario */
    void handleEvent(const SDL_Event& e) override;

    /* Acción en cada frame */
    void act() override;

    /* Renderizarse */
    void render() const override;

    /* Liberar sus recursos (SI ES NECESARIO) */
    void free() override;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~ParticularEntity();
};

//-----------------------------------------------------------------------------

#endif  // __PARTICULAR_ENTITY_H__
