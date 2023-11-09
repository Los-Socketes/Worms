#include "animacion.h"

Animacion::Animacion(Renderer& render, std::string ruta_textura, int tam_x, int tam_y, int frames, bool fix) :
    renderizador(render),
    frames(frames),
    dimensiones(tam_x, tam_y),
    textura(renderizador, Surface(ruta_textura).SetColorKey(true, 0)),
    fix(fix) {}

void Animacion::dibujar(Camara& camara, int pos_x, int pos_y, bool flip, int it, int velocidad) {
    std::optional<Rect> rect_interseccion = camara.getRectangulo().GetIntersection(Rect(pos_x, pos_y, dimensiones.first, dimensiones.second));
    
    int coord_x = pos_x - camara.getPosicionX();
    int coord_y = pos_y - camara.getPosicionY();
    
    // Si no hay interseccion no se renderiza.
    if (!rect_interseccion) {
        return;
    }

    int frame_actual = (it / velocidad) % frames;  
    int fix_x = 0;
    if(fix){
        fix_x = -frame_actual;
    }
    if (flip){
        // Si camina a la derecha.
        renderizador.Copy( 
            textura,
            Rect(fix_x, frame_actual * dimensiones.second, dimensiones.first, dimensiones.second),
            Rect(coord_x, coord_y, dimensiones.first, dimensiones.second),
            0,
            NullOpt,
            SDL_FLIP_HORIZONTAL);
    } else {
        // Si camina a la izquierda se invierte la imagen.
        renderizador.Copy( 
            textura,
            Rect(fix_x, frame_actual * dimensiones.second, dimensiones.first, dimensiones.second),
            Rect(coord_x, coord_y, dimensiones.first, dimensiones.second),
            0,
            NullOpt,
            SDL_FLIP_NONE);
    }
}