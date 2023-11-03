#include "animacion.h"

Animacion::Animacion(Renderer& render, std::string ruta_textura, int frames) :
    renderizador(render),
    frames(frames),
    textura(renderizador, Surface(ruta_textura).SetColorKey(true, 0)) {}

void Animacion::siguiente_frame(int pos_x, int pos_y, DireccionGusano dir, int it) {
    int frame_actual = it % frames;  
    if (dir == DERECHA){
        // Si camina a la derecha.
        renderizador.Copy(textura, Rect(-frame_actual, frame_actual * 60, 60, 60), Rect(pos_x, pos_y, 60, 60));
    } else {
        // Si camina a la izquierda se invierte la imagen.
        renderizador.Copy(textura, Rect(-frame_actual, frame_actual * 60, 60, 60), Rect(pos_x, pos_y, 60, 60), 0, NullOpt, SDL_FLIP_HORIZONTAL);
    }  
}