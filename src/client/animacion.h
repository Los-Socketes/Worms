#ifndef ANIMACION_H_
#define ANIMACION_H_

#include <string>
#include <SDL2pp/SDL2pp.hh>

#include "defs.h"

using namespace SDL2pp;

class Animacion {
 private:
    Renderer& renderizador;
    int frames;
    Texture textura;

 public:
    Animacion(Renderer& render, std::string ruta_textura, int frames);

    void siguiente_frame(int pos_x, int pos_y, DireccionGusano dir, int it);
};

#endif // ANIMACION_H_