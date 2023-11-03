#ifndef ANIMACION_H_
#define ANIMACION_H_

#include <string>
#include <SDL2pp/SDL2pp.hh>

using namespace SDL2pp;

class Animacion {
 private:
    Renderer& renderizador;
    int frames;
    int frame_actual;
    Texture textura;

 public:
    Animacion(Renderer& render, std::string ruta_textura, int frames);

    void siguiente_frame(int pos_x, int pos_y, int dir_x);
};

#endif // ANIMACION_H_