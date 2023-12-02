#include "entidadEscenario.h"

EntidadEscenario::EntidadEscenario(Renderer& renderizador,
    GestorAnimaciones& gestor_animaciones,
    Camara& camara,
    int& ancho_mapa,
    int& alto_mapa) :
    Entidad(gestor_animaciones),
    renderizador(renderizador),
    camara(camara),
    ancho_mapa(ancho_mapa),
    alto_mapa(alto_mapa) {
        pos_x = ancho_mapa / 2;
        pos_y = alto_mapa / 2;
        gestor_animaciones.getAnimacionEscenario(FONDO)->setDimensiones(ancho_mapa, alto_mapa);
    }

void EntidadEscenario::dibujarBloque(int& posicion_x, int& posicion_y) {
    std::optional<Rect> rect_interseccion = camara.getRectangulo().GetIntersection(Rect(posicion_x, posicion_y, 640, 200));
    
    int coord_x = posicion_x - camara.getPosicionX();
    int coord_y = posicion_y - camara.getPosicionY();
    
    // Si no hay interseccion no se renderiza.
    if (!rect_interseccion) {
        return;
    }

    renderizador.SetDrawBlendMode(SDL_BLENDMODE_NONE);
    renderizador.SetDrawColor(0, 0, 0, 255);
    renderizador.FillRect(coord_x, coord_y, coord_x + 640, coord_y + 200);
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
    for (int i = 0; i <= (ancho_mapa / 640 + 1); i++) {
        posicion.first = i * 640;
        posicion.second = alto_mapa - 179;
        gestor_animaciones.getAnimacionEscenario(PANORAMA)->dibujar(camara, posicion.first, posicion.second, false, it, 1);
    }
    // Cubro el exceso de panorama en bordes con rectángulos negros.
    int posicion_x = ancho_mapa;
    int posicion_y = alto_mapa - 200;
    dibujarBloque(posicion_x, posicion_y);
    posicion_x = -641;
    dibujarBloque(posicion_x, posicion_y);
}