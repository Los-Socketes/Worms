#include "camara.h"

Camara::Camara(int x, int y, int ancho, int alto, int ancho_mapa, int alto_mapa) :
    posicion(x, y),
    dimension(ancho, alto),
    dimension_mapa(ancho_mapa, alto_mapa),
    objetivo(ancho_mapa / 2, alto_mapa / 2),
    tiempo_espera_mouse(0) {}

void Camara::moverEnEjeX(int& deltaX) {
    int nueva_pos_x = posicion.first + deltaX;

    // Si las dimensiones de la pantalla son menores a las del mapa, se limita el movimiento
    // para que no se salga del mapa. Si no, la camara queda en el centro de esa dimension.
    if (dimension_mapa.first > dimension.first) {
        if (nueva_pos_x < 0) {
            nueva_pos_x = 0;
        } else if (nueva_pos_x > dimension_mapa.first - dimension.first) {
            nueva_pos_x = dimension_mapa.first - dimension.first;
        }
    } else {
        nueva_pos_x = (dimension_mapa.first - dimension.first) / 2;
    }

    posicion.first = nueva_pos_x;
}

void Camara::moverEnEjeY(int& deltaY) {
    int nueva_pos_y = posicion.second + deltaY;

    // Si las dimensiones de la pantalla son menores a las del mapa, se limita el movimiento
    // para que no se salga del mapa. Si no, la camara queda en el centro de esa dimension.
    if (dimension_mapa.second > dimension.second) {
        if (nueva_pos_y < 0) {
            nueva_pos_y = 0;
        } else if (nueva_pos_y > dimension_mapa.second - dimension.second) {
            nueva_pos_y = dimension_mapa.second - dimension.second;
        }
    } else {
        nueva_pos_y = (dimension_mapa.second - dimension.second) / 2;
    }

    posicion.second = nueva_pos_y;
}

void Camara::moverConMouse(int& deltaX, int& deltaY) {
    tiempo_espera_mouse = SEGUNDOS_ESPERA_MOUSE * FPS;
    moverEnEjeX(deltaX);
    moverEnEjeY(deltaY);
}

void Camara::moverHaciaObjetivo() {
    // Si no se movio el mouse en los ultimos 3 segundos, se mueve la camara hacia el objetivo.
    if (tiempo_espera_mouse == 0) {
        // Calculo el vector entre la posicion de la camara y el objetivo.
        int deltaX = objetivo.first - posicion.first - floor(dimension.first / 2);
        int deltaY = objetivo.second - posicion.second - floor(dimension.second / 2);
        // La cámara se mueve lentamente en cada eje hacia el objetivo.
        // Si el objetivo esta a menos de 10 pixeles, se mueve directamente a la posicion del objetivo.
        if (abs(deltaX) < 5) {
            posicion.first = objetivo.first - floor(dimension.first / 2);
            if (posicion.first < 0) {
                posicion.first = 0;
            } else if (posicion.first > dimension_mapa.first - dimension.first) {
                posicion.first = dimension_mapa.first - dimension.first;
            }
        } else {
            deltaX = deltaX / 10;
            moverEnEjeX(deltaX);
        }
        if (abs(deltaY) < 5) {
            posicion.second = objetivo.second - floor(dimension.second / 2);
            if (posicion.second < 0) {
                posicion.second = 0;
            } else if (posicion.second > dimension_mapa.second - dimension.second) {
                posicion.second = dimension_mapa.second - dimension.second;
            }
        } else {
            deltaY = deltaY / 10;
            moverEnEjeY(deltaY);
        }
    } else {
        // Si el mouse se movio en los ultimos 3 segundos, se decrementa el contador.
        tiempo_espera_mouse--;
    }
}

std::pair<int, int> Camara::traducirCoordenadas(coordX& x, coordY& y) {
    // Paso de coordenadas en metros a coordenadas en pixeles.
    int coord_x = x * PIXELS_POR_METRO;
    int coord_y = dimension_mapa.second - y * PIXELS_POR_METRO;
    return std::make_pair(coord_x, coord_y);
}

std::pair<coordX, coordY> Camara::traducirCoordenadasInversa(int x, int y) {
    // Teniendo la conversion metro a pixel como:
    // x_pixeles = x_metro * PIXELS_POR_METRO - camara.x
    // y_pixeles = alto_mapa - y_metro * PIXELS_POR_METRO - camara.y
    // La inversa es:
    // x_metro = (x_pixeles + camara.x) / PIXELS_POR_METRO
    // y_metro = (alto_mapa - y_pixeles - camara.y) / PIXELS_POR_METRO
    return std::make_pair((x + posicion.first) / PIXELS_POR_METRO,
        (dimension_mapa.second - y - posicion.second) / PIXELS_POR_METRO);
}

void Camara::actualizarObjetivo(coordX& x, coordY& y) {
    objetivo = traducirCoordenadas(x, y);
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

void Camara::setDimensionMapa(coordX& ancho, coordY& alto) {
    dimension_mapa.first = ancho * PIXELS_POR_METRO;
    dimension_mapa.second = alto * PIXELS_POR_METRO;
}

bool Camara::usadaPorMouse() {
    return tiempo_espera_mouse > 0;
}

Rect Camara::getRectangulo() {
    return Rect(posicion.first, posicion.second, dimension.first, dimension.second);
}