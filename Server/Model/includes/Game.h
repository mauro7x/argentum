#ifndef __GAME_H__
#define __GAME_H__
//-----------------------------------------------------------------------------
#include <unordered_map>
//-----------------------------------------------------------------------------
#include "../../../Common/includes/MapContainer.h"
#include "../../../Common/includes/UnitData.h"
//-----------------------------------------------------------------------------
#include "Character.h"
#include "config_structs.h"
#include "Config.h"
#include "ItemsContainer.h"
//-----------------------------------------------------------------------------
class Game {
    private:
        //-----------------------------------------------------------------------------
        // Game components configuration files:
        //-----------------------------------------------------------------------------
        Config<RaceCfg> races;
        Config<KindCfg> kinds;

        //-----------------------------------------------------------------------------

        //-----------------------------------------------------------------------------
        // Game entities
        //-----------------------------------------------------------------------------
        MapContainer map_container;
        ItemsContainer items;
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
        // DEFINIR COMO VIENE EL PLAYERDATA SI ES NUEVO!.
        /*
         * Recibe un struct PlayerData con toda la información persistida
         * del character, o bien la información necesaria para crear un nuevo
         * character.
         * 
         * Retorna el id único de instancia de dicho character, mediante el cual
         * se interactuará con el mismo.
         * 
         * Lanza Exception si alguno de los id no mapea a ninguna raza/clase.
         */
        const int newCharacter(PlayerData& init_data);

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
