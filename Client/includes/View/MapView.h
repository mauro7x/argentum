#ifndef __MAP_VIEW_H__
#define __MAP_VIEW_H__

//-----------------------------------------------------------------------------
#include "../../../Common/includes/MapContainer.h"
#include "../../../Common/includes/defs.h"
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "ItemSpriteContainer.h"
#include "Renderer.h"
#include "TileContainer.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class MapView {
   private:
    const Renderer* g_renderer;
    TileContainer tiles;
    MapContainer maps;
    const ItemSpriteContainer& item_sprites;

    /* Current map info */
    Map* current_map = NULL;
    Id current_map_id = 0;
    int w_tiles = 0, h_tiles = 0;

    /* Calcula el rectangulo sobre el que se debe graficar la textura. Esta
     * función es necesaria ya que el sistema de coordenadas en Tiled (el editor
     * de mapas utilizado) es inverso al que utiliza SDL, por lo que esta
     * función arregla esto. */
    SDL_Rect _getRenderQuad(const Texture& texture, const int x_tile,
                            const int y_tile) const;

   public:
    /* Constructor */
    MapView(const Renderer* renderer, const ItemSpriteContainer& item_sprites);

    /* Deshabilitamos el constructor por copia. */
    MapView(const MapView&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    MapView& operator=(const MapView&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    MapView(MapView&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    MapView& operator=(MapView&& other) = delete;

    //-------------------------------------------------------------------------
    // Métodos de inicialización

    /* Carga los archivos necesarios */
    void loadMedia();

    //-------------------------------------------------------------------------
    // Métodos de escritura

    /* Selecciona el mapa indicado por el id. Si cambia, devuelve true. */
    bool selectMap(const Id id);

    /* Seleccionar un NPC en un tile */
    void selectNPC(const int x_tile, const int y_tile);

    /* Desselecciona al NPC en un tile */
    void clearSelectedNPC(const int x_tile, const int y_tile);

    /* Settea el ocupante de una celda */
    void occupyTile(InstanceId id, const int x_tile, const int y_tile);

    /* Settea el item de una celda */
    void addItem(Id id, const int x_tile, const int y_tile);

    /* Libera una celda */
    void clearTileOccupant(const int x_tile, const int y_tile);

    /* Libera una celda */
    void clearTileItem(const int x_tile, const int y_tile);

    /* Limpia el mapa de ocupantes y de items */
    void clear();

    //-------------------------------------------------------------------------
    // Métodos de renderizado

    /* Renderiza el piso completo */
    void renderGround() const;

    /* Renderiza la decoración, los npcs, y los items */
    void renderRow(const int row) const;

    /* Renderiza una fila determinada de decoración */
    // void renderDecoration(const int row) const;

    /* Renderiza una fila determinada de NPCs */
    // void renderNPCs(const int row) const;

    /* Renderiza una fila determinada de items droppeados */
    // void renderItems(const int row) const;

    /* Renderiza los techos completos */
    void renderRoofs() const;

    /* Oscurece el mapa fuera de las construcciones */
    void renderShadowOutdoor() const;

    //-------------------------------------------------------------------------
    // Métodos de lectura

    /* Devuelve el ID de la unidad ocupante de una celda */
    InstanceId getOccupant(const int x, const int y) const;

    /* Devuelve el ID del NPC de una celda */
    Id getNPC(const int x, const int y) const;

    /* Devuelve si la celda dada pertenece a un indoor */
    bool indoor(const int x, const int y) const;

    /* Obtiene la anchura en tiles del mapa actual */
    int widthInTiles() const;

    /* Obtiene la altura en tiles del mapa actual */
    int heightInTiles() const;

    /* Obtiene la anchura en pixeles del mapa actual */
    int widthInPx() const;

    /* Obtiene la altura en pixeles del mapa actual */
    int heightInPx() const;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~MapView();
};

//-----------------------------------------------------------------------------

#endif  // __MAP_VIEW_H__
