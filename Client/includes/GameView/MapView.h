#ifndef __MAP_VIEW_H__
#define __MAP_VIEW_H__

//-----------------------------------------------------------------------------
#include <list>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/MapContainer.h"
#include "../../../Common/includes/defs.h"
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../SDL/Renderer.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Camera.h"
#include "ItemSpriteContainer.h"
#include "TileContainer.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
static const int rendering_x_tiles_padding = 5;
static const int rendering_y_tiles_padding = 5;
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class MapView {
   private:
    const Renderer* g_renderer;
    const Camera& g_camera;
    TileContainer tiles;
    MapContainer maps;
    const ItemSpriteContainer& item_sprites;

    // Area a renderizar
    int x_min = 0, x_max = 0, y_min = 0, y_max = 0;

    // Mapa actual
    Map* current_map = NULL;
    Id current_map_id = 0;
    int w_tiles = 0, h_tiles = 0;

    //-------------------------------------------------------------------------
    // Métodos privados

    /* Calcula el rectangulo sobre el que se debe graficar la textura. Esta
     * función es necesaria ya que el sistema de coordenadas en Tiled (el editor
     * de mapas utilizado) es inverso al que utiliza SDL, por lo que esta
     * función arregla esto. */
    SDL_Rect _getRenderQuad(const Texture& texture, const int x_tile,
                            const int y_tile) const;

    //-------------------------------------------------------------------------

   public:
    /* Constructor */
    MapView(const Renderer* renderer, const Camera& camera,
            const ItemSpriteContainer& item_sprites);

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

    /* Settea la región de renderizado en base a la cámara */
    void setRenderArea();

    /* Selecciona el mapa indicado por el id. Si cambia, devuelve true. */
    bool selectMap(const Id id);

    /* Seleccionar un NPC en un tile */
    void selectNPC(const int x_tile, const int y_tile);

    /* Des-selecciona al NPC en un tile */
    void clearSelectedNPC(const int x_tile, const int y_tile);

    /* Seleccionar el PORTAL de un tile */
    void selectPortal(const int x_tile, const int y_tile);

    /* Des-selecciona el PORTAL de un tile */
    void clearSelectedPortal(const int x_tile, const int y_tile);

    /* Settea el ocupante de una celda */
    void occupyTile(InstanceId id, const int x_tile, const int y_tile);

    /* Settea el item de una celda */
    void addItem(Id id, uint32_t amount, const int x_tile, const int y_tile);

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
    void renderRow(const int row, std::list<InstanceId>& units_to_render) const;

    /* Renderiza los techos */
    void renderRoofs() const;

    /* Renderiza sombra en outdoor */
    void renderShadowOutdoor() const;

    //-------------------------------------------------------------------------
    // Métodos de lectura

    /* Devuelve el ID de la unidad ocupante de una celda */
    InstanceId getOccupant(const int x, const int y) const;

    /* Devuelve el ID del NPC de una celda */
    Id getNPC(const int x, const int y) const;

    /* Devuelve si la celda dada pertenece a un indoor */
    bool indoor(const int x, const int y) const;

    /* Devuelve si en la celda dada hay un portal */
    bool portal(const int x, const int y) const;

    /* Obtiene la fila minima y la máxima */
    void getRowRange(int& row_min, int& row_max) const;

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
