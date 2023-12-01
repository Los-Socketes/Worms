#include "entidadEscenario.h"

EntidadEscenario::EntidadEscenario(GestorAnimaciones& gestor_animaciones,
    Camara& camara,
    int& ancho_mapa,
    int& alto_mapa) :
    Entidad(gestor_animaciones),
    camara(camara),
    ancho_mapa(ancho_mapa),
    alto_mapa(alto_mapa) {
        pos_x = ancho_mapa / 2;
        pos_y = alto_mapa / 2;
        gestor_animaciones.getAnimacionEscenario(FONDO)->setDimensiones(ancho_mapa, alto_mapa);
    }

void EntidadEscenario::setDimensionesMapa(int ancho_mapa, int alto_mapa) {
    this->ancho_mapa = ancho_mapa;
    this->alto_mapa = alto_mapa;
    this->pos_x = ancho_mapa / 2;
    this->pos_y = alto_mapa / 2;
    gestor_animaciones.getAnimacionEscenario(FONDO)->setDimensiones(ancho_mapa, alto_mapa);
}

void EntidadEscenario::dibujar() {
    // Dibujo la imagen de fondo.
    gestor_animaciones.getAnimacionEscenario(FONDO)->dibujar(camara, pos_x, pos_y, false, it, 1);
    // Dibujo el panorama del fondo.
    std::pair<int, int> posicion;
    for (int i = 0; i < (ancho_mapa / 640 + 1); i++) {
        posicion.first = i * 640;
        posicion.second = alto_mapa - 179;
        gestor_animaciones.getAnimacionEscenario(PANORAMA)->dibujar(camara, posicion.first, posicion.second, false, it, 1);
    }
}