#include "camara.h"

Camara::Camara(int x, int y, int ancho, int alto, int ancho_mapa, int alto_mapa) :
    posicion(x, y),
    dimension(ancho, alto),
    dimension_mapa(ancho_mapa, alto_mapa) {}

void Camara::mover(int& deltaX, int& deltaY) {
    int nueva_pos_x = posicion.first + deltaX;
    int nueva_pos_y = posicion.second + deltaY;

    if (nueva_pos_x < 0) {
        nueva_pos_x = 0;
    } else if (nueva_pos_x + dimension.first > dimension_mapa.first) {
        nueva_pos_x = dimension_mapa.first - dimension.first;
    }

    if (nueva_pos_y < 0) {
        nueva_pos_y = 0;
    } else if (nueva_pos_y + dimension.second > dimension_mapa.second) {
        nueva_pos_y = dimension_mapa.second - dimension.second;
    }

    posicion.first = nueva_pos_x;
    posicion.second = nueva_pos_y;
}

std::pair<coordX, coordY> Camara::traducirCoordenadas(int x, int y) {
    // Teniendo la conversion metro a pixel como:
    // x_pixeles = x_metro * PIXELS_POR_METRO - camara.x
    // y_pixeles = alto_mapa - y_metro * PIXELS_POR_METRO - camara.y
    // La inversa es:
    // x_metro = (x_pixeles + camara.x) / PIXELS_POR_METRO
    // y_metro = (alto_mapa - y_pixeles - camara.y) / PIXELS_POR_METRO
    return std::make_pair((x + posicion.first) / PIXELS_POR_METRO,
        (dimension_mapa.second - y - posicion.second) / PIXELS_POR_METRO);
}

int Camara::getPosicionX() {
    return posicion.first;
}

int Camara::getPosicionY() {
    return posicion.second;
}

int Camara::getAncho() {
    return dimension.first;
}

int Camara::getAlto() {
    return dimension.second;
}

void Camara::setPosicion(int& x, int& y) {
    posicion.first = x;
    posicion.second = y;
}

void Camara::setDimension(int ancho, int alto) {
    dimension.first = ancho;
    dimension.second = alto;
}

void Camara::setDimensionMapa(int& ancho, int& alto) {
    dimension_mapa.first = ancho * PIXELS_POR_METRO;
    dimension_mapa.second = alto * PIXELS_POR_METRO;
}

Rect Camara::getRectangulo() {
    return Rect(posicion.first, posicion.second, dimension.first, dimension.second);
}