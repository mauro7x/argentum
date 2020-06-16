#ifndef __MAP_VIEW_H__
#define __MAP_VIEW_H__

//-----------------------------------------------------------------------------
#include "../../../Common/includes/MapContainer.h"
#include "../../../Common/includes/types.h"
#include "Renderer.h"
#include "TileContainer.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class MapView {
   private:
    const Renderer* g_renderer;
    TileContainer tiles;
    MapContainer maps;

    /* Current map info */
    Id current_map_id;
    int w_tiles, h_tiles;
    int tile_w, tile_h;

    /* Calcula el rectangulo sobre el que se debe graficar la textura. Esta
     * función es necesaria ya que el sistema de coordenadas en Tiled (el editor
     * de mapas utilizado) es inverso al que utiliza SDL, por lo que esta
     * función arregla esto. */
    SDL_Rect _getRenderQuad(const Texture& texture, const int x_tile,
                            const int y_tile, const int tile_w,
                            const int tile_h) const;

   public:
    /* Constructor */
    MapView(const Renderer* renderer);

    /* Deshabilitamos el constructor por copia. */
    MapView(const MapView&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    MapView& operator=(const MapView&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    MapView(MapView&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    MapView& operator=(MapView&& other) = delete;

    //-------------------------------------------------------------------------

    /* Carga los archivos necesarios */
    void loadMedia();

    /* Selecciona el mapa indicado por el id */
    void select(Id id);

    //-------------------------------------------------------------------------
    // RENDERIZACIÓN

    /* Renderiza el piso completo */
    void renderGround() const;

    /* Renderiza una fila determinada de decoración */
    void renderDecoration(const int row) const;

    /* Renderiza una fila determinada de NPCs */
    void renderNPCs(const int row) const;

    /* Renderiza los techos completos */
    void renderRoofs() const;

    /* Oscurece el mapa fuera de las construcciones */
    void renderShadowOutdoor() const;

    //-------------------------------------------------------------------------

    /* Devuelve el ID de la unidad ocupante de una celda */
    int getOccupant(const int x, const int y) const;

    /* Devuelve si es un tile válido que forma parte del mapa */
    bool isValid(const int x, const int y) const;

    /* Devuelve si hay collision en la celda dada */
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
