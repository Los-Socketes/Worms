#include "animacion.h"

Animacion::Animacion(Renderer& render, std::string ruta_textura, int frames) :
    renderizador(render),
    frames(frames),
    frame_actual(-1),
    textura(renderizador, Surface(ruta_textura).SetColorKey(true, 0)) {}

void Animacion::siguiente_frame(int pos_x, int pos_y, int dir_x) {
    frame_actual = (SDL_GetTicks() / 50) % frames;  
    if (dir_x == 0){
        // Si camina a la derecha.
        renderizador.Copy(textura, Rect(-frame_actual, frame_actual * 60, 60, 60), Rect(pos_x, pos_y, 60, 60));
    } else {
        // Si camina a la izquierda se invierte la imagen.
        renderizador.Copy(textura, Rect(-frame_actual, frame_actual * 60, 60, 60), Rect(pos_x, pos_y, 60, 60), 0, NullOpt, SDL_FLIP_HORIZONTAL);
    }  
}