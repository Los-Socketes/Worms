#include "dibujador.h"

Dibujador::Dibujador(Camara& camara, EstadoDelJuego& estado_juego, int ancho_pantalla, int alto_pantalla, int ancho_mapa, int alto_mapa) :
    camara(camara),
    estado_juego(estado_juego),
    ancho_pantalla(ancho_pantalla),
    alto_pantalla(alto_pantalla),
    ancho_mapa(ancho_mapa),
    alto_mapa(alto_mapa) {}

void Dibujador::inicializarAnimaciones(Renderer& renderizador) {

    // Animaciones de agua y fondo.
    animaciones["gradiente"] = std::make_shared<Animacion>(renderizador, "assets/sprites/back.png", 3000, 2000, 1, false);
    animaciones["gradiente"]->setDimensiones(ancho_mapa, alto_mapa);
    animaciones["agua"] = std::make_shared<Animacion>(renderizador, "assets/sprites/water.png", 128, 100, 12, false);
    animaciones["fondo"] = std::make_shared<Animacion>(renderizador, "assets/sprites/backforest.png", 640, 159, 1, true);

    // Animaciones de gusanos.
    animaciones["quieto"] = std::make_shared<Animacion>(renderizador, "assets/sprites/wblink1.png", 60, 60, 6, false);
    animaciones["caminando"] = std::make_shared<Animacion>(renderizador, "assets/sprites/wwalk.png", 60, 60, 15, true);
    animaciones["inicio salto"] = std::make_shared<Animacion>(renderizador, "assets/sprites/wjump.png", 60, 60, 10, false);
    animaciones["saltando"] = std::make_shared<Animacion>(renderizador, "assets/sprites/wflyup.png", 60, 60, 2, false);
    animaciones["cayendo"] = std::make_shared<Animacion>(renderizador, "assets/sprites/wfall.png", 60, 60, 2, false);

    // Animaciones de armas.

    // Bate de baseball.
    animaciones["sacando bate"] = std::make_shared<Animacion>(renderizador, "assets/sprites/wbsblnk.png", 60, 60, 10, false);
    animaciones["apuntando bate"] = std::make_shared<Animacion>(renderizador, "assets/sprites/wbsbaim.png", 60, 60, 32, false);
    animaciones["batiendo"] = std::make_shared<Animacion>(renderizador, "assets/sprites/wbsbswn.png", 60, 60, 32, false);
    animaciones["guardando bate"] = std::make_shared<Animacion>(renderizador, "assets/sprites/wbsbbak.png", 60, 60, 10, false);
    animaciones["guardando bate batido"] = std::make_shared<Animacion>(renderizador, "assets/sprites/wbsbbk2.png", 60, 60, 10, false);

}

void Dibujador::dibujar(Renderer& renderizador, int it) {
    renderizador.Clear();

    dibujarMapa();
    dibujarAguaDetras(it);
    dibujarGusanos(it);
    //dibujarProyectiles(it);
    dibujarAguaDelante(it);

    renderizador.Present();
}

void Dibujador::dibujarMapa() {
    // Dibujo la imagen de fondo.
    animaciones["gradiente"]->dibujar(camara, 0, 0, false, 0, 1);
    // Dibujo el panorama del fondo.
    for (int i = 0; i < (ancho_mapa / 640 + 1); i++) {
        animaciones["fondo"]->dibujar(camara, i * 640, alto_mapa - 210, false, 0, 1);
    }
}

void Dibujador::dibujarGusanos(int it) {
    for (auto& jugador : estado_juego.gusanos) {
        for (auto& gusano : jugador.second) {
            switch (gusano.estado) {
                case QUIETO:
                    if (gusano.armaEquipada == BATE_P)
                        animaciones["apuntando bate"]->dibujar(camara, gusano.posicion.first, gusano.posicion.second, gusano.dir == DERECHA, 16, 1);
                    else
                        animaciones["quieto"]->dibujar(camara, gusano.posicion.first, gusano.posicion.second, gusano.dir == DERECHA, 0, 1); 
                    break;
                case CAMINANDO:
                    animaciones["caminando"]->dibujar(camara, gusano.posicion.first, gusano.posicion.second, gusano.dir == DERECHA, it, 1);
                    break;
                case SALTANDO:
                    animaciones["saltando"]->dibujar(camara, gusano.posicion.first, gusano.posicion.second, gusano.dir == DERECHA, it, 1);
                    break;
                case CAYENDO:
                    animaciones["cayendo"]->dibujar(camara, gusano.posicion.first, gusano.posicion.second, gusano.dir == DERECHA, it, 1);
                    break;
                case DISPARANDO:
                    if (gusano.armaEquipada == BATE_P)
                        animaciones["batiendo"]->dibujar(camara, gusano.posicion.first, gusano.posicion.second, gusano.dir == DERECHA, 16, 1);
                    else
                        animaciones["quieto"]->dibujar(camara, gusano.posicion.first, gusano.posicion.second, gusano.dir == DERECHA, 0, 1); 
                    break;
                default:
                    animaciones["quieto"]->dibujar(camara, gusano.posicion.first, gusano.posicion.second, gusano.dir == DERECHA, 0, 1);
                    break;
            }
        }
    }
}
// TODO: implementar.
// void Dibujador::dibujarProyectiles(int it) {}

void Dibujador::dibujarAguaDetras(int it) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < (ancho_mapa / 128 + 1); j++) {
            animaciones["agua"]->dibujar(camara, j * 128, alto_mapa - 100 + 10 * (i + 1), false, it + 3*(i+1), 2);
        }
    }
}

void Dibujador::dibujarAguaDelante(int it) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < (ancho_mapa / 128 + 1); j++) {
            animaciones["agua"]->dibujar(camara, j * 128, alto_mapa - 80 + 10 * (i + 1), false, it + 3*(i+3), 2);
        }
    }
}