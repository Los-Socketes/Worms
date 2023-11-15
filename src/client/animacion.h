#ifndef ANIMACION_H_
#define ANIMACION_H_

#include <utility>
#include <string>
#include <SDL2pp/SDL2pp.hh>

#include "defs.h"
#include "camara.h"

using namespace SDL2pp;

class Animacion {
 private:
    Renderer& renderizador;
    int frames;
    std::pair<int, int> tam;
    std::pair<int, int> dimensiones;
    Texture textura;
    bool seguir_camara;
    
    void dibujarComun(Camara& camara, int pos_x, int pos_y, bool flip, int frame_actual);
 public:
   Animacion(Renderer& render, std::string ruta_textura, int tam_x, int tam_y, int frames, bool seguir_camara);
   void dibujar(Camara& camara, int pos_x, int pos_y, bool flip, int it, int velocidad);
   void dibujar(Camara& camara, int pos_x, int pos_y, bool flip, radianes angulo);
   void setDimensiones(int tam_x, int tam_y);
};

#endif // ANIMACION_H_