#include "gestoranimaciones.h"

GestorAnimaciones::GestorAnimaciones(Camara& camara, int ancho_mapa, int alto_mapa) :
    camara(camara),
    ancho_mapa(ancho_mapa),
    alto_mapa(alto_mapa) {}

void GestorAnimaciones::inicializar(Renderer& renderizador) {

    // Animaciones de escenario/interfaz.

    escenario[AGUA] = std::make_shared<Animacion>(renderizador, "assets/sprites/water.png", 128, 100, 12, true);
    escenario[PANORAMA] = std::make_shared<Animacion>(renderizador, "assets/sprites/backforest.png", 640, 159, 1, true);
    escenario[FONDO] = std::make_shared<Animacion>(renderizador, "assets/sprites/back.png", 3000, 2000, 1, true);
    escenario[FONDO]->setDimensiones(ancho_mapa, alto_mapa);
    escenario[RETICULA] = std::make_shared<Animacion>(renderizador, "assets/sprites/crshairc.png", 60, 60, 32, true);
    escenario[VIGA_GRANDE] = std::make_shared<Animacion>(renderizador, "assets/sprites/grdl4.png", 140, 20, 1, true);
    escenario[VIGA_CHICA] = std::make_shared<Animacion>(renderizador, "assets/sprites/grds4.png", 70, 20, 1, true);

    // Iconos de armas.
    iconos[NADA_P] = std::make_shared<Animacion>(renderizador, "assets/sprites/inothing.png", 32, 32, 1, false);
    iconos[BATE_P] = std::make_shared<Animacion>(renderizador, "assets/sprites/ibaseball.png", 32, 32, 1, false);
    iconos[BAZOOKA_P] = std::make_shared<Animacion>(renderizador, "assets/sprites/ibazooka.png", 32, 32, 1, false);
    iconos[MORTERO_P] = std::make_shared<Animacion>(renderizador, "assets/sprites/imortar.png", 32, 32, 1, false);
    iconos[GRANADA_VERDE_P] = std::make_shared<Animacion>(renderizador, "assets/sprites/igrenade.png", 32, 32, 1, false);
    iconos[GRANADA_ROJA_P] = std::make_shared<Animacion>(renderizador, "assets/sprites/icluster.png", 32, 32, 1, false);
    iconos[DINAMITA_P] = std::make_shared<Animacion>(renderizador, "assets/sprites/idynamite.png", 32, 32, 1, false);
    iconos[BANANA_P] = std::make_shared<Animacion>(renderizador, "assets/sprites/ibanana.png", 32, 32, 1, false);
    iconos[GRANADA_SANTA_P] = std::make_shared<Animacion>(renderizador, "assets/sprites/ihgrenade.png", 32, 32, 1, false);
    iconos[ATAQUE_AEREO_P] = std::make_shared<Animacion>(renderizador, "assets/sprites/iairstrke.png", 32, 32, 1, false);
    iconos[TELETRANSPORTACION_P] = std::make_shared<Animacion>(renderizador, "assets/sprites/iteleport.png", 32, 32, 1, false);

    // Animaciones de gusanos.

    // Gusano sin armas.    
    gusanos[std::make_pair(QUIETO, NADA_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wblink1.png", 60, 60, 1, true);
    gusanos[std::make_pair(CAMINANDO, NADA_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wwalk.png", 60, 60, 15, true);
    gusanos[std::make_pair(SALTANDO, NADA_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wflyup.png", 60, 60, 2, true);
    gusanos[std::make_pair(CAYENDO, NADA_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wfall.png", 60, 60, 2, true);
    gusanos[std::make_pair(DISPARANDO, NADA_P)] = gusanos[std::make_pair(QUIETO, NADA_P)];
    
    // Gusano con bate de baseball.
    gusanos[std::make_pair(QUIETO, BATE_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wbsbaim.png", 60, 60, 32, true);
    gusanos[std::make_pair(CAMINANDO, BATE_P)] = gusanos[std::make_pair(CAMINANDO, NADA_P)];
    gusanos[std::make_pair(SALTANDO, BATE_P)] = gusanos[std::make_pair(SALTANDO, NADA_P)];
    gusanos[std::make_pair(CAYENDO, BATE_P)] = gusanos[std::make_pair(CAYENDO, NADA_P)];
    gusanos[std::make_pair(DISPARANDO, BATE_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wbsbswn.png", 60, 60, 32, true);

    // Gusano con bazooka.
    gusanos[std::make_pair(QUIETO, BAZOOKA_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wbaz.png", 60, 60, 32, true);
    gusanos[std::make_pair(CAMINANDO, BAZOOKA_P)] = gusanos[std::make_pair(CAMINANDO, NADA_P)];
    gusanos[std::make_pair(SALTANDO, BAZOOKA_P)] = gusanos[std::make_pair(SALTANDO, NADA_P)];
    gusanos[std::make_pair(CAYENDO, BAZOOKA_P)] = gusanos[std::make_pair(CAYENDO, NADA_P)];
    gusanos[std::make_pair(DISPARANDO, BAZOOKA_P)] = gusanos[std::make_pair(QUIETO, BAZOOKA_P)];

    // Gusano con mortero.
    gusanos[std::make_pair(QUIETO, MORTERO_P)] = gusanos[std::make_pair(QUIETO, BAZOOKA_P)];
    gusanos[std::make_pair(CAMINANDO, MORTERO_P)] = gusanos[std::make_pair(CAMINANDO, NADA_P)];
    gusanos[std::make_pair(SALTANDO, MORTERO_P)] = gusanos[std::make_pair(SALTANDO, NADA_P)];
    gusanos[std::make_pair(CAYENDO, MORTERO_P)] = gusanos[std::make_pair(CAYENDO, NADA_P)];
    gusanos[std::make_pair(DISPARANDO, MORTERO_P)] = gusanos[std::make_pair(QUIETO, BAZOOKA_P)];

    // Gusano con granada verde.
    gusanos[std::make_pair(QUIETO, GRANADA_VERDE_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wthrgrn.png", 60, 60, 32, true);
    gusanos[std::make_pair(CAMINANDO, GRANADA_VERDE_P)] = gusanos[std::make_pair(CAMINANDO, NADA_P)];
    gusanos[std::make_pair(SALTANDO, GRANADA_VERDE_P)] = gusanos[std::make_pair(SALTANDO, NADA_P)];
    gusanos[std::make_pair(CAYENDO, GRANADA_VERDE_P)] = gusanos[std::make_pair(CAYENDO, NADA_P)];
    gusanos[std::make_pair(DISPARANDO, GRANADA_VERDE_P)] = gusanos[std::make_pair(QUIETO, NADA_P)];

    // Gusano con granada roja.
    gusanos[std::make_pair(QUIETO, GRANADA_ROJA_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wthrcls.png", 60, 60, 32, true);
    gusanos[std::make_pair(CAMINANDO, GRANADA_ROJA_P)] = gusanos[std::make_pair(CAMINANDO, NADA_P)];
    gusanos[std::make_pair(SALTANDO, GRANADA_ROJA_P)] = gusanos[std::make_pair(SALTANDO, NADA_P)];
    gusanos[std::make_pair(CAYENDO, GRANADA_ROJA_P)] = gusanos[std::make_pair(CAYENDO, NADA_P)];
    gusanos[std::make_pair(DISPARANDO, GRANADA_ROJA_P)] = gusanos[std::make_pair(QUIETO, NADA_P)];

    // Gusano con granada santa.
    gusanos[std::make_pair(QUIETO, GRANADA_SANTA_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wthrhgr.png", 60, 60, 32, true);
    gusanos[std::make_pair(CAMINANDO, GRANADA_SANTA_P)] = gusanos[std::make_pair(CAMINANDO, NADA_P)];
    gusanos[std::make_pair(SALTANDO, GRANADA_SANTA_P)] = gusanos[std::make_pair(SALTANDO, NADA_P)];
    gusanos[std::make_pair(CAYENDO, GRANADA_SANTA_P)] = gusanos[std::make_pair(CAYENDO, NADA_P)];
    gusanos[std::make_pair(DISPARANDO, GRANADA_SANTA_P)] = gusanos[std::make_pair(QUIETO, NADA_P)];

    // Gusano con dinamita.
    gusanos[std::make_pair(QUIETO, DINAMITA_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wdynbak.png", 60, 60, 1, true);
    gusanos[std::make_pair(CAMINANDO, DINAMITA_P)] = gusanos[std::make_pair(CAMINANDO, NADA_P)];
    gusanos[std::make_pair(SALTANDO, DINAMITA_P)] = gusanos[std::make_pair(SALTANDO, NADA_P)];
    gusanos[std::make_pair(CAYENDO, DINAMITA_P)] = gusanos[std::make_pair(CAYENDO, NADA_P)];
    gusanos[std::make_pair(DISPARANDO, DINAMITA_P)] = gusanos[std::make_pair(QUIETO, NADA_P)];

    // Gusano con banana.
    gusanos[std::make_pair(QUIETO, BANANA_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wthrban.png", 60, 60, 32, true);
    gusanos[std::make_pair(CAMINANDO, BANANA_P)] = gusanos[std::make_pair(CAMINANDO, NADA_P)];
    gusanos[std::make_pair(SALTANDO, BANANA_P)] = gusanos[std::make_pair(SALTANDO, NADA_P)];
    gusanos[std::make_pair(CAYENDO, BANANA_P)] = gusanos[std::make_pair(CAYENDO, NADA_P)];
    gusanos[std::make_pair(DISPARANDO, BANANA_P)] = gusanos[std::make_pair(QUIETO, NADA_P)];

    // Gusano con ataque aereo.
    gusanos[std::make_pair(QUIETO, ATAQUE_AEREO_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wairtlk.png", 60, 60, 1, true);
    gusanos[std::make_pair(CAMINANDO, ATAQUE_AEREO_P)] = gusanos[std::make_pair(CAMINANDO, NADA_P)];
    gusanos[std::make_pair(SALTANDO, ATAQUE_AEREO_P)] = gusanos[std::make_pair(SALTANDO, NADA_P)];
    gusanos[std::make_pair(CAYENDO, ATAQUE_AEREO_P)] = gusanos[std::make_pair(CAYENDO, NADA_P)];
    gusanos[std::make_pair(DISPARANDO, ATAQUE_AEREO_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wairtlk.png", 60, 60, 10, true);

    // Gusano con teletransportacion.
    gusanos[std::make_pair(QUIETO, TELETRANSPORTACION_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wteltlk.png", 60, 60, 10, true);
    gusanos[std::make_pair(CAMINANDO, TELETRANSPORTACION_P)] = gusanos[std::make_pair(CAMINANDO, NADA_P)];
    gusanos[std::make_pair(SALTANDO, TELETRANSPORTACION_P)] = gusanos[std::make_pair(SALTANDO, NADA_P)];
    gusanos[std::make_pair(CAYENDO, TELETRANSPORTACION_P)] = gusanos[std::make_pair(CAYENDO, NADA_P)];
    gusanos[std::make_pair(DISPARANDO, TELETRANSPORTACION_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wteldsv.png", 60, 60, 48, true);

    // Proyectiles.    
    


}

void GestorAnimaciones::dibujarAgua(int pos_x, int pos_y, int it) {
    escenario[AGUA]->dibujar(camara, pos_x, pos_y, false, it, 1);
}

void GestorAnimaciones::dibujarFondo() {
    escenario[FONDO]->dibujar(camara, ancho_mapa / 2, alto_mapa / 2, false, 0, 1);
}

void GestorAnimaciones::dibujarPanorama(int pos_x, int pos_y) {
    escenario[PANORAMA]->dibujar(camara, pos_x, pos_y, false, 0, 1);
}

void GestorAnimaciones::dibujarViga(int pos_x, int pos_y, int largo, radianes angulo) {
    if (largo > 3) {
        escenario[VIGA_GRANDE]->setDimensiones(largo * PIXELS_POR_METRO, PIXELS_POR_METRO);
        escenario[VIGA_GRANDE]->dibujar(camara, pos_x + (largo * PIXELS_POR_METRO) / 2, pos_y - PIXELS_POR_METRO / 2, false, 0, 1, angulo);
    } else {
        escenario[VIGA_CHICA]->setDimensiones(largo * PIXELS_POR_METRO, PIXELS_POR_METRO);
        escenario[VIGA_CHICA]->dibujar(camara, pos_x, pos_y, false, 0, 1, angulo);
    }    
}

void GestorAnimaciones::dibujarGusano(EstadoGusano estado, ArmaProtocolo arma, DireccionGusano dir, int pos_x, int pos_y, int it, radianes angulo) {
    if(arma != NADA_P && (estado == QUIETO || estado == DISPARANDO)) {
        gusanos[std::make_pair(estado, arma)]->dibujar(camara, pos_x, pos_y, dir == DERECHA, angulo);
    } else {
        gusanos[std::make_pair(estado, arma)]->dibujar(camara, pos_x, pos_y, dir == DERECHA, it, 1);
    }
}

void GestorAnimaciones::dibujarReticula(int pos_x, int pos_y, int it) {
    escenario[RETICULA]->dibujar(camara, pos_x, pos_y, false, it, 1);
}

void GestorAnimaciones::dibujarIconoArma(ArmaProtocolo arma, int pos_x, int pos_y) {
    iconos[arma]->dibujar(camara, pos_x, pos_y, false, 0, 1);
}

// TODO: implementar.
// void GestorAnimaciones::dibujarProyectil(ProyectilProtocolo protectil??, radianes angulo??, int pos_x, int pos_y, int it) {}

// TODO: implementar.
// void GestorAnimaciones::dibujarExplosion(int pos_x, int pos_y, int it) {}
