#include "animacion.h"

Animacion::Animacion(Renderer& render, std::string ruta_textura, int tam_x, int tam_y, int frames, bool seguir_camara) :
    renderizador(render),
    frames(frames),
    tam(tam_x, tam_y),
    dimensiones(tam_x, tam_y),
    textura(renderizador, Surface(ruta_textura).SetColorKey(true, 0)),
    seguir_camara(seguir_camara){
    textura.SetBlendMode(SDL_BLENDMODE_BLEND);
    }

void Animacion::dibujarComun(Camara& camara, int pos_x, int pos_y, radianes angulo, bool flip, int frame_actual) {
    int coord_x = pos_x - dimensiones.first / 2;
    int coord_y = pos_y - dimensiones.second / 2;

    if (seguir_camara) {
        std::optional<Rect> rect_interseccion = camara.getRectangulo().GetIntersection(Rect(coord_x, coord_y, tam.first, tam.second));

        coord_x -= camara.getPosicionX();
        coord_y -= camara.getPosicionY();

        // Si no hay interseccion no se renderiza.
        if (!rect_interseccion) {
            return;
        }
    }

    int flip_flag = SDL_FLIP_NONE;
    if (flip)
        flip_flag = SDL_FLIP_HORIZONTAL;

    renderizador.Copy( 
        textura,
        Rect(0, frame_actual * tam.second, tam.first, tam.second),
        Rect(coord_x, coord_y, dimensiones.first, dimensiones.second),
        angulo * 180 / M_PI,
        NullOpt,
        flip_flag);
}

void Animacion::dibujar(Camara& camara, int pos_x, int pos_y, bool flip, int it, int velocidad) {
    int frame_actual = (it / velocidad) % frames;
    dibujarComun(camara, pos_x, pos_y, 0, flip, frame_actual);
}

void Animacion::dibujar(Camara& camara, int pos_x, int pos_y, bool flip, radianes angulo) {
    // El angulo 0 corresponde al primer frame y el angulo 180 al ultimo.
    int frame_actual = floor((frames * angulo) / M_PI);
    dibujarComun(camara, pos_x, pos_y, 0, flip, frame_actual);
}

void Animacion::dibujar(Camara& camara, int pos_x, int pos_y, bool flip, int it, int velocidad, radianes angulo) {
    int frame_actual = (it / velocidad) % frames;
    dibujarComun(camara, pos_x, pos_y, angulo, flip, frame_actual);
}

void Animacion::setDimensiones(int tam_x, int tam_y){
    dimensiones.first = tam_x;
    dimensiones.second = tam_y;
}

