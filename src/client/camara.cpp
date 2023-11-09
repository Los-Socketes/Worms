#include "camara.h"

Camara::Camara(int x, int y, int ancho, int alto) :
    posicion(x, y),
    dimension(ancho, alto) {}

void Camara::mover(int deltaX, int deltaY, int ancho_mapa, int alto_mapa) {
    int nueva_pos_x = posicion.first + deltaX;
    int nueva_pos_y = posicion.second + deltaY;

    if (nueva_pos_x < 0) {
        nueva_pos_x = 0;
    } else if (nueva_pos_x + dimension.first > ancho_mapa) {
        nueva_pos_x = ancho_mapa - dimension.first;
    }

    if (nueva_pos_y < 0) {
        nueva_pos_y = 0;
    } else if (nueva_pos_y + dimension.second > alto_mapa) {
        nueva_pos_y = alto_mapa - dimension.second;
    }

    posicion.first = nueva_pos_x;
    posicion.second = nueva_pos_y;
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

void Camara::setPosicion(int x, int y) {
    posicion.first = x;
    posicion.second = y;
}

void Camara::setDimension(int ancho, int alto) {
    dimension.first = ancho;
    dimension.second = alto;
}

Rect Camara::getRectangulo() {
    return Rect(posicion.first, posicion.second, dimension.first, dimension.second);
}