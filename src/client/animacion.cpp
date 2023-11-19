#include "animacion.h"

Animacion::Animacion(Renderer& render,
    std::string ruta_textura, int tam_x,
    int tam_y, int frames, bool seguir_camara,
    Centro centro) :
    renderizador(render),
    frames(frames),
    tam(tam_x, tam_y),
    dimensiones(tam_x, tam_y),
    textura(renderizador, Surface(ruta_textura).SetColorKey(true, 0)),
    seguir_camara(seguir_camara),
    centro(centro) {
    textura.SetBlendMode(SDL_BLENDMODE_BLEND);
    }

void Animacion::dibujarComun(Camara& camara, int& pos_x, int& pos_y, radianes& angulo, bool& flip, int& frame_actual) {

    int coord_x = pos_x;
    int coord_y = pos_y;

    if (centro == Centro::CENTRO) {
        coord_x -= dimensiones.first / 2;
        coord_y -= dimensiones.second / 2;
    } else if (centro == Centro::ESQUINA_INF_IZQ) {
        coord_y -= dimensiones.second;
    } else if (centro == Centro::ESQUINA_INF_DER) {
        coord_x -= dimensiones.first;
        coord_y -= dimensiones.second;
    } else if (centro == Centro::ESQUINA_SUP_DER) {
        coord_x -= dimensiones.first;
    }


    if (seguir_camara) {
        std::optional<Rect> rect_interseccion = camara.getRectangulo().GetIntersection(Rect(coord_x, coord_y, dimensiones.first, dimensiones.second));

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

void Animacion::dibujar(Camara& camara, int& pos_x, int& pos_y, bool flip, int& it, int velocidad) {
    int frame_actual = (it / velocidad) % frames;
    radianes rotacion = 0;
    dibujarComun(camara, pos_x, pos_y, rotacion, flip, frame_actual);
}

void Animacion::dibujar(Camara& camara, int& pos_x, int& pos_y, bool flip, radianes& angulo) {
    // El angulo 90 corresponde al primer frame y el angulo -90 al ultimo.
    int frame_actual = (angulo * 180 / M_PI + 90) / 180 * frames;
    radianes rotacion = 0;
    dibujarComun(camara, pos_x, pos_y, rotacion, flip, frame_actual);
}

void Animacion::dibujar(Camara& camara, int& pos_x, int& pos_y, bool flip, int& it, int velocidad, radianes& angulo) {
    int frame_actual = (it / velocidad) % frames;
    dibujarComun(camara, pos_x, pos_y, angulo, flip, frame_actual);
}

void Animacion::setDimensiones(int tam_x, int tam_y){
    dimensiones.first = tam_x;
    dimensiones.second = tam_y;
}

