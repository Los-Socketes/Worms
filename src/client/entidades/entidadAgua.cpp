#include "entidadAgua.h"

EntidadAgua::EntidadAgua(Renderer& renderizador,
    GestorAnimaciones& gestor_animaciones,
    Camara& camara,
    int& ancho_mapa,
    int& alto_mapa,
    int capas,
    int pos_y) :
    Entidad(gestor_animaciones),
    renderizador(renderizador),
    camara(camara),
    ancho_mapa(ancho_mapa),
    alto_mapa(alto_mapa),
    capas(capas),
    pos_y(pos_y) {}

void EntidadAgua::dibujarBloque(int& posicion_x, int& posicion_y) {
    std::optional<Rect> rect_interseccion = camara.getRectangulo().GetIntersection(Rect(posicion_x, posicion_y, 128, capas * 100));
    
    int coord_x = posicion_x - camara.getPosicionX();
    int coord_y = posicion_y - camara.getPosicionY();
    
    // Si no hay interseccion no se renderiza.
    if (!rect_interseccion) {
        return;
    }

    renderizador.SetDrawBlendMode(SDL_BLENDMODE_NONE);
    renderizador.SetDrawColor(0, 0, 0, 255);
    renderizador.FillRect(coord_x, coord_y, coord_x + 128, coord_y + capas * 100);
}

void EntidadAgua::setDimensionesMapa(int ancho_mapa, int alto_mapa) {
    this->ancho_mapa = ancho_mapa;
    this->alto_mapa = alto_mapa;
}

void EntidadAgua::dibujar() {
    std::pair<int, int> posicion;
    int iteracion;
    for (int i = 0; i < capas; i++) {
        for (int j = 0; j < (ancho_mapa / 128 + 1); j++) {
            posicion.first = j * 128 + 64;
            posicion.second = alto_mapa - pos_y + 10 * (i + 1);
            iteracion = it + 3*(i+1);
            gestor_animaciones.getAnimacionEscenario(AGUA)->dibujar(camara, posicion.first, posicion.second, false, iteracion, 1);
        }
    }
    // Cubro el exceso de agua en los bordes con rectángulos negros.
    int posicion_x = ancho_mapa;
    int posicion_y = alto_mapa - pos_y - 50;
    dibujarBloque(posicion_x, posicion_y);
    posicion_x = -129;
    dibujarBloque(posicion_x, posicion_y);
}