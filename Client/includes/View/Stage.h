#ifndef __STAGE_H__
#define __STAGE_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>

#include "../../../Common/includes/UnitData.h"
#include "Character.h"
#include "Creature.h"
#include "HUD.h"
#include "MapView.h"
#include "Player.h"
#include "UnitContainer.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

/* La idea es que esta clase contenga todo lo que se debe RENDERIZAR en la
 * pantalla del jugador. Al tener todo, tanto el mapa como a las distintas
 * entidades, puede ordenar la renderización de manera tal que el resultado sea
 * el deseado. */

class Stage {
   private:
    const MapView& g_map;

    /* Unidades a renderizar */
    const Player& g_player;
    const UnitContainer<Character, CharacterData>& g_characters;
    const UnitContainer<Creature, CreatureData>& g_creatures;

   public:
    /* Constructor */
    Stage(const MapView& map, const Player& player,
          const UnitContainer<Character, CharacterData>& characters,
          const UnitContainer<Creature, CreatureData>& creatures);

    /* Deshabilitamos el constructor por copia. */
    Stage(const Stage&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Stage& operator=(const Stage&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Stage(Stage&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Stage& operator=(Stage&& other) = delete;

    /* Renderiza la pantalla siguiendo un algoritmo que permita que las
     * entidades se grafiquen entre los objetos de decoración del mapa. */
    void render() const;

    /* Destructor */
    ~Stage();
};

//-----------------------------------------------------------------------------

#endif  // __STAGE_H__
