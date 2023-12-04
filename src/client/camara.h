#ifndef CAMARA_H_
#define CAMARA_H_

#include <utility>
#include <atomic>
#include <SDL2pp/SDL2pp.hh>

#include "defs.h"

#define FPS 30
#define SEGUNDOS_ESPERA_MOUSE 3

using namespace SDL2pp;

// Clase que representa la camara del cliente. A partir de la posicion de la camara
// y sus dimensiones, se puede calcular el rectangulo que se ve en la pantalla y donde
// se debe dibujar cada objeto.
class Camara {
 private:
    std::pair<int, int> posicion;  // Pos x, y.
    std::pair<int, int> dimension;  // Ancho, alto.
    std::pair<int, int> dimension_mapa;  // Ancho, alto del mapa.
    std::pair<int, int> objetivo;  // Pos x, y del objetivo a seguir.

    int tiempo_espera_mouse;

    // Mueven la camara en deltaX y deltaY, chequeando que no se salga del mapa.
    void moverEnEjeX(int& deltaX);
    void moverEnEjeY(int& deltaY);
 public:
    Camara(int x, int y, int ancho, int alto, int ancho_mapa, int alto_mapa);
    
    void moverConMouse(int& deltaX, int& deltaY);
    void moverHaciaObjetivo();

    // Traduce las coordenadas del mapa a las coordenadas en pixeles del escenario.
    std::pair<int, int> traducirCoordenadas(coordX& x, coordY& y);
    // Traduce las coordenadas en pantalla a las coordenadas del mapa.
    std::pair<coordX, coordY> traducirCoordenadasInversa(int x, int y);

    void actualizarObjetivo(coordX& x, coordY& y);

    int getPosicionX();
    int getPosicionY();
    int getAncho();
    int getAlto();

    void setPosicion(int& x, int& y);
    void setDimension(int ancho, int alto);
    void setDimensionMapa(coordX& ancho, coordY& alto);

    bool usadaPorMouse();

    Rect getRectangulo();
};

#endif // CAMARA_H_