#ifndef ANIMACION_H_
#define ANIMACION_H_

#include <utility>
#include <string>
#include <SDL2pp/SDL2pp.hh>

#include "defs.h"
#include "camara.h"

using namespace SDL2pp;

enum Centro {
   CENTRO,
   ESQUINA_SUP_IZQ,
   ESQUINA_SUP_DER,
   ESQUINA_INF_IZQ,
   ESQUINA_INF_DER
};

// Clase que se encarga de dibujar una animacion. Posee una textura con todos los frames
// de la animacion, y permite dibujar un frame dado un angulo o una iteracion del loop.
class Animacion {
 private:
    Renderer& renderizador;
    int frames;
    std::pair<int, int> tam;
    std::pair<int, int> dimensiones;
    Texture textura;
    bool seguir_camara;
    bool es_loop;
    Centro centro;
    // Angulo minimo y maximo para calcular el frame proporcionalmente al angulo
    // si se dibuja por angulo, por defecto es -pi/2 y pi/2.
    radianes min;
    radianes max;
    
    // Metodo comun para dibujar la animacion, llamado por los otros metodos dibujar.
    void dibujarComun(Camara& camara, int& pos_x, int& pos_y, radianes& angulo, bool& flip, int& frame_actual);
 public:
   Animacion(Renderer& render, const std::string& ruta_textura, int tam_x, int tam_y, int frames, bool seguir_camara, bool es_loop, Centro centro = CENTRO, radianes min = -M_PI/2, radianes max = M_PI/2);

   // Dibuja el frame dada una iteración del loop y una velocidad.
   void dibujar(Camara& camara, int& pos_x, int& pos_y, bool flip, int& it, int velocidad);
   // Dibuja el frame dado un angulo.
   void dibujar(Camara& camara, int& pos_x, int& pos_y, bool flip, radianes& angulo);
   // Dibuja el frame dada una iteración del loop, una velocidad y lo rota un angulo.
   void dibujar(Camara& camara, int& pos_x, int& pos_y, bool flip, int& it, int velocidad, radianes& angulo);

   int getFrames();
   void setDimensiones(int tam_x, int tam_y);
};

#endif // ANIMACION_H_