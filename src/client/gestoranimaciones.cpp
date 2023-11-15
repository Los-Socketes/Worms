#include "gestoranimaciones.h"

GestorAnimaciones::GestorAnimaciones(Camara& camara, int ancho_mapa, int alto_mapa) :
    camara(camara),
    ancho_mapa(ancho_mapa),
    alto_mapa(alto_mapa) {}

void GestorAnimaciones::inicializar(Renderer& renderizador) {

    // Animaciones de escenario.
    escenario[AGUA] = std::make_shared<Animacion>(renderizador, "assets/sprites/water.png", 128, 100, 12, false);
    escenario[PANORAMA] = std::make_shared<Animacion>(renderizador, "assets/sprites/backforest.png", 640, 159, 1, false);
    escenario[FONDO] = std::make_shared<Animacion>(renderizador, "assets/sprites/back.png", 3000, 2000, 1, false);
    escenario[FONDO]->setDimensiones(ancho_mapa, alto_mapa);

    // Animaciones de gusanos.

    // Gusano sin armas.    
    gusanos[std::make_pair(QUIETO, NADA_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wblink1.png", 60, 60, 1, false);
    gusanos[std::make_pair(CAMINANDO, NADA_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wwalk.png", 60, 60, 15, true);
    //gusanos[std::make_pair(INICIO_SALTO, NADA_P)] = std::make_shared<Animacion>(
        //renderizador, "assets/sprites/wjump.png", 60, 60, 10, false);
    gusanos[std::make_pair(SALTANDO, NADA_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wflyup.png", 60, 60, 2, false);
    gusanos[std::make_pair(CAYENDO, NADA_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wfall.png", 60, 60, 2, false);
    gusanos[std::make_pair(DISPARANDO, NADA_P)] = gusanos[std::make_pair(QUIETO, NADA_P)];
    
    // Gusano con bate de baseball.
    gusanos[std::make_pair(QUIETO, BATE_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wbsbaim.png", 60, 60, 32, false);
    gusanos[std::make_pair(CAMINANDO, BATE_P)] = gusanos[std::make_pair(CAMINANDO, NADA_P)];
    //gusanos[std::make_pair(INICIO_SALTO, BATE_P)] = gusanos[std::make_pair(INICIO_SALTO, NADA_P)];
    gusanos[std::make_pair(SALTANDO, BATE_P)] = gusanos[std::make_pair(SALTANDO, NADA_P)];
    gusanos[std::make_pair(CAYENDO, BATE_P)] = gusanos[std::make_pair(CAYENDO, NADA_P)];
    gusanos[std::make_pair(DISPARANDO, BATE_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wbsbswn.png", 60, 60, 32, false);

}

void GestorAnimaciones::dibujarAgua(int pos_x, int pos_y, int it) {
    escenario[AGUA]->dibujar(camara, pos_x, pos_y, false, it, 1);
}

void GestorAnimaciones::dibujarFondo() {
    escenario[FONDO]->dibujar(camara, 0, 0, false, 0, 1);
}

void GestorAnimaciones::dibujarPanorama(int pos_x, int pos_y) {
    escenario[PANORAMA]->dibujar(camara, pos_x, pos_y, false, 0, 1);
}

void GestorAnimaciones::dibujarGusano(EstadoGusano estado, ArmaProtocolo arma, DireccionGusano dir, int pos_x, int pos_y, int it, radianes angulo) {
    if(arma != NADA_P && (estado == QUIETO || estado == DISPARANDO)) {
        gusanos[std::make_pair(estado, arma)]->dibujar(camara, pos_x, pos_y, dir == DERECHA, angulo);
    } else {
        gusanos[std::make_pair(estado, arma)]->dibujar(camara, pos_x, pos_y, dir == DERECHA, it, 1);
    }
}

// TODO: implementar.
// void GestorAnimaciones::dibujarProyectil(ProyectilProtocolo protectil??, radianes angulo??, int pos_x, int pos_y, int it) {}

// TODO: implementar.
// void GestorAnimaciones::dibujarExplosion(int pos_x, int pos_y, int it) {}
