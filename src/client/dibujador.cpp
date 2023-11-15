#include "dibujador.h"

Dibujador::Dibujador(Camara& camara, EstadoDelJuego& estado_juego, int ancho_mapa, int alto_mapa) :
    camara(camara),
    estado_juego(estado_juego),
    ancho_mapa(ancho_mapa),
    alto_mapa(alto_mapa),
    gestor_animaciones(camara, ancho_mapa, alto_mapa),
    fuente("assets/fonts/AdLibRegular.ttf", 12) {}

void Dibujador::dibujarVida(Renderer& renderizador, std::pair<int, int> posicion, int vida) {
    // Acomodo la posicion para que quede centrada en el gusano.
    int pos_x = posicion.first + 22;
    int pos_y = posicion.second - 2;

    std::optional<Rect> rect_interseccion = camara.getRectangulo().GetIntersection(Rect(pos_x, pos_y, 16, 16));
    
    int coord_x = pos_x - camara.getPosicionX();
    int coord_y = pos_y - camara.getPosicionY();
    
    // Si no hay interseccion no se renderiza.
    if (!rect_interseccion) {
        return;
    }

    // Dibujo un rectangulo negro transparente detrás de la vida.
    renderizador.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
    renderizador.SetDrawColor(0, 0, 0, 50);
    renderizador.FillRect(coord_x - 2, coord_y - 2, coord_x + 18, coord_y + 14);

    // Dibujo el borde de la vida.
    fuente.SetOutline(2);
    Texture textura_vida_outline(renderizador, fuente.RenderText_Blended(std::to_string(vida), {0, 0, 0, 255}));
    renderizador.Copy(textura_vida_outline, NullOpt, Rect(coord_x, coord_y, 16, 16));

    // Dibujo la vida.
    // TODO: colores segun jugador.
    fuente.SetOutline(0);
    Texture textura_vida(renderizador, fuente.RenderText_Blended(std::to_string(vida), {255, 255, 0, 255}));
    renderizador.Copy(textura_vida, NullOpt, Rect(coord_x, coord_y, 16, 16));
}


void Dibujador::inicializarAnimaciones(Renderer& renderizador) {
    gestor_animaciones.inicializar(renderizador);    
}

void Dibujador::dibujar(Renderer& renderizador, int it, radianes angulo) {
    renderizador.Clear();

    dibujarMapa();
    dibujarAguaDetras(it);
    dibujarGusanos(renderizador, it, angulo);
    //dibujarProyectiles(it);
    dibujarAguaDelante(it);

    renderizador.Present();
}

void Dibujador::dibujarMapa() {
    // Dibujo la imagen de fondo.
    gestor_animaciones.dibujarFondo();
    // Dibujo el panorama del fondo.
    for (int i = 0; i < (ancho_mapa / 640 + 1); i++) {
        gestor_animaciones.dibujarPanorama(i * 640, alto_mapa - 259);
    }
}


void Dibujador::dibujarGusanos(Renderer& renderizador, int it, radianes angulo) {
    // Recorro el mapa de jugador -> gusanos.
    for (auto& jugador : estado_juego.gusanos) {
        // Recorro los gusanos del jugador.
        for (auto& gusano : jugador.second) {
            // TODO: dibujar barra de potencia si esta disparando.
            // Dibujo al gusano.
            gestor_animaciones.dibujarGusano(gusano.estado, gusano.armaEquipada, gusano.dir, gusano.posicion.first, gusano.posicion.second, it, angulo);
            // Dibujo la vida del gusano.
            dibujarVida(renderizador, gusano.posicion, gusano.vida);
            // Dibujo la reticula del gusano si esta apuntando.
            if (gusano.estado == QUIETO && gusano.armaEquipada != NADA_P) {
                gestor_animaciones.dibujarReticula(
                    gusano.posicion.first + sin(angulo) * 60,
                    gusano.posicion.second + cos(angulo) * 60,
                    it);
            }
                
        }
    }

}

// TODO: implementar.
// void Dibujador::dibujarProyectiles(int it) {}


void Dibujador::dibujarAguaDetras(int it) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < (ancho_mapa / 128 + 1); j++) {
            gestor_animaciones.dibujarAgua(j * 128, alto_mapa - 120 + 10 * (i + 1), it + 3*(i+1));
        }
    }
}

void Dibujador::dibujarAguaDelante(int it) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < (ancho_mapa / 128 + 1); j++) {
            gestor_animaciones.dibujarAgua(j * 128, alto_mapa - 100 + 10 * (i + 1), it + 3*(i+3));
        }
    }
}