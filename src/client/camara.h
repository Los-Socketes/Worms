#ifndef CAMARA_H_
#define CAMARA_H_

#include <utility>
#include <SDL2pp/SDL2pp.hh>

using namespace SDL2pp;

class Camara {
 private:
    std::pair<int, int> posicion;  // Pos x, y.
    std::pair<int, int> dimension;  // Ancho, alto.
    std::pair<int, int> dimension_mapa;  // Ancho, alto del mapa.

 public:
    Camara(int x, int y, int ancho, int alto, int ancho_mapa, int alto_mapa);
    
    void mover(int& deltaX, int& deltaY);

    int getPosicionX();
    int getPosicionY();
    int getAncho();
    int getAlto();

    void setPosicion(int& x, int& y);
    void setDimension(int ancho, int alto);
    void setDimensionMapa(int& ancho, int& alto);

    Rect getRectangulo();
};

#endif // CAMARA_H_