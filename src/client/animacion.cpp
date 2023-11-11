#include "animacion.h"

Animacion::Animacion(Renderer& render, std::string ruta_textura, int tam_x, int tam_y, int frames, bool fix) :
    renderizador(render),
    frames(frames),
    tam(tam_x, tam_y),
    dimensiones(tam_x, tam_y),
    textura(renderizador, Surface(ruta_textura).SetColorKey(true, 0)),
    fix(fix) {}

void Animacion::dibujar(Camara& camara, int pos_x, int pos_y, bool flip, int it, int velocidad) {
    std::optional<Rect> rect_interseccion = camara.getRectangulo().GetIntersection(Rect(pos_x, pos_y, tam.first, tam.second));
    
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
        renderizador.Copy( 
            textura,
            Rect(fix_x, frame_actual * tam.second, tam.first, tam.second),
            Rect(coord_x, coord_y, dimensiones.first, dimensiones.second),
            0,
            NullOpt,
            SDL_FLIP_HORIZONTAL);
    } else {
        renderizador.Copy( 
            textura,
            Rect(fix_x, frame_actual * tam.second, tam.first, tam.second),
            Rect(coord_x, coord_y, dimensiones.first, dimensiones.second),
            0,
            NullOpt,
            SDL_FLIP_NONE);
    }
}

void Animacion::setDimensiones(int tam_x, int tam_y){
    dimensiones.first = tam_x;
    dimensiones.second = tam_y;
}