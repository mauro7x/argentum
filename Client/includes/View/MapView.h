#ifndef __MAP_VIEW_H__
#define __MAP_VIEW_H__

//-----------------------------------------------------------------------------
#include "../../../Common/includes/MapContainer.h"
#include "Renderer.h"
#include "TileContainer.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class MapView {
   private:
    const Renderer* g_renderer;
    TileContainer tiles;
    MapContainer maps;
    Id current_map_id;

    /* Calcula el rectangulo sobre el que se debe graficar la textura. Esta
     * función es necesaria ya que el sistema de coordenadas en Tiled (el editor
     * de mapas utilizado) es inverso al que utiliza SDL, por lo que esta
     * función arregla esto. */
    SDL_Rect _getRenderQuad(const Texture& texture, const int x_tile,
                            const int y_tile, const int tile_w,
                            const int tile_h) const;

    /* Renderiza el piso */
    void _renderGround() const;

    /* Renderiza las decoraciones */
    void _renderDecoration() const;

    /* Renderiza los techos */
    void _renderRoofs(const int x_player, const int y_player) const;

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

    /* Devuelve si es un tile válido que forma parte del mapa */
    bool isValid(const int x, const int y) const;

    /* Devuelve si hay collision en la celda dada */
    bool collision(const int x, const int y) const;

    /* Obtiene la anchura en pixeles del mapa actual */
    int getWidth() const;

    /* Obtiene la altura en pixeles del mapa actual */
    int getHeight() const;

    /* Renderiza el mapa para un jugador en la posición recibida */
    void render(const int x_player, const int y_player) const;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~MapView();
};

//-----------------------------------------------------------------------------

#endif  // __MAP_VIEW_H__
