#ifndef __GAME_H__
#define __GAME_H__
//-----------------------------------------------------------------------------
#include <unordered_map>
//-----------------------------------------------------------------------------
#include "../../../Common/includes/MapContainer.h"
//-----------------------------------------------------------------------------
#include "Character.h"
#include "config_structs.h"
#include "Config.h"
//-----------------------------------------------------------------------------
class Game {
    private:
        //-----------------------------------------------------------------------------
        // Game components configuration files:
        //-----------------------------------------------------------------------------
        Config<RaceCfg> races;
        Config<KindCfg> kinds;
        Config<WeaponCfg> weapons;
        Config<WandCfg> wands;
        Config<SpellCfg> spells;
        Config<DefenceCfg> defences;
        Config<PotionCfg> potions;
        //-----------------------------------------------------------------------------

        //-----------------------------------------------------------------------------
        // Game entities
        //-----------------------------------------------------------------------------
        MapContainer map_container;
        std::unordered_map<int, Character> characters;
        std::unordered_map<int, Item*> items;
        // std::unordered_map<int, Creatures> creatures; Falta implementar
        //-----------------------------------------------------------------------------
        int next_instance_id;

    public:
        //-----------------------------------------------------------------------------
        Game();
        ~Game();

        Game(const Game&) = delete;
        Game& operator=(const Game&) = delete;
        Game(Game&& other) = delete;
        Game& operator=(Game&& other) = delete;
        //-----------------------------------------------------------------------------

        //-----------------------------------------------------------------------------
        /*
        * Recibe id_race e id_kind para crear un character de la raza
        * y clase establecidos.
        * 
        * Retorna el id único de instancia de dicho character.
        * 
        * Lanza Exception si alguno de los id no mapea a ninguna raza/clase.
        */
        const int newCharacter(const int id_race, const int id_kind);

        /*
        * Llamar a este metodo ante la desconexión de un character.
        * 
        * Recibe el id de instancia del character a eliminar.
        * Lo persiste, y luego lo elimina del juego.
        * 
        * Lanza Exception si el id especificado no corresponde a ningún
        * character en el juego.
        */
        void deleteCharacter(const int id);
        //-----------------------------------------------------------------------------

        
};
//-----------------------------------------------------------------------------
#endif  // __GAME_H__
