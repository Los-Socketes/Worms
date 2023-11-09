#ifndef CAMARA_H_
#define CAMARA_H_

#include <utility>
#include <SDL2pp/SDL2pp.hh>

using namespace SDL2pp;

class Camara {
 private:
    std::pair<int, int> posicion;  // Pos x, y.
    std::pair<int, int> dimension;  // Ancho, alto.

 public:
    Camara(int x, int y, int ancho, int alto);
    
    void mover(int deltaX, int deltaY, int ancho_mapa, int alto_mapa);

    int getPosicionX();
    int getPosicionY();
    int getAncho();
    int getAlto();

    void setPosicion(int x, int y);
    void setDimension(int ancho, int alto);

    Rect getRectangulo();
};

#endif // CAMARA_H_