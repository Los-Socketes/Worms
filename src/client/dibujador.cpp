#include "dibujador.h"

Dibujador::Dibujador(Camara& camara, EstadoDelJuego& estado_juego, int ancho_mapa, int alto_mapa) :
    camara(camara),
    estado_juego(estado_juego),
    ancho_mapa(ancho_mapa),
    alto_mapa(alto_mapa),
    gestor_animaciones(camara, ancho_mapa, alto_mapa) {}

void Dibujador::inicializarAnimaciones(Renderer& renderizador) {
    gestor_animaciones.inicializar(renderizador);    
}

void Dibujador::dibujar(Renderer& renderizador, int it, radianes angulo) {
    renderizador.Clear();

    dibujarMapa();
    dibujarAguaDetras(it);
    dibujarGusanos(it, angulo);
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


void Dibujador::dibujarGusanos(int it, radianes angulo) {
    // Recorro el mapa de jugador -> gusanos.
    for (auto& jugador : estado_juego.gusanos) {
        // Recorro los gusanos del jugador.
        for (auto& gusano : jugador.second) {
            // Dibujo el gusano.
            // TODO: dibujar vida, reticula si esta apuntando y barra de potencia si esta disparando.
            gestor_animaciones.dibujarGusano(gusano.estado, gusano.armaEquipada, gusano.dir, gusano.posicion.first, gusano.posicion.second, it, angulo);
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