#include "entidadAgua.h"

EntidadAgua::EntidadAgua(GestorAnimaciones& gestor_animaciones,
    Camara& camara,
    int& ancho_mapa,
    int& alto_mapa,
    int capas,
    int pos_y) :
    Entidad(gestor_animaciones),
    camara(camara),
    ancho_mapa(ancho_mapa),
    alto_mapa(alto_mapa),
    capas(capas),
    pos_y(pos_y) {}

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
}