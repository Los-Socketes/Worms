#include "gestoranimaciones.h"

GestorAnimaciones::GestorAnimaciones(Camara& camara, int ancho_mapa, int alto_mapa) :
    camara(camara),
    ancho_mapa(ancho_mapa),
    alto_mapa(alto_mapa) {}

void GestorAnimaciones::setDimensionMapa(int ancho, int alto) {
    ancho_mapa = ancho;
    alto_mapa = alto;
}

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

    // Gusano con bate de baseball.
    gusanos[std::make_pair(QUIETO, BATE_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wbsbaim.png", 60, 60, 32, true);
    gusanos[std::make_pair(CAMINANDO, BATE_P)] = gusanos[std::make_pair(CAMINANDO, NADA_P)];
    gusanos[std::make_pair(SALTANDO, BATE_P)] = gusanos[std::make_pair(SALTANDO, NADA_P)];
    gusanos[std::make_pair(CAYENDO, BATE_P)] = gusanos[std::make_pair(CAYENDO, NADA_P)];
    gusanos[std::make_pair(DISPARANDO, BATE_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wbsbswn.png", 60, 60, 32, true);

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
    
    // Bazooka.
    proyectiles[std::make_pair(BAZOOKA_P, false)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/missile.png", 60, 60, 32, true, CENTRO, 0, 2 * M_PI);
    // Mortero
    proyectiles[std::make_pair(MORTERO_P, false)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/mortar.png", 60, 60, 32, true, CENTRO, 0, 2 * M_PI);
    proyectiles[std::make_pair(MORTERO_P, true)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/mortar.png", 60, 60, 32, true, CENTRO, 0, 2 * M_PI);
    proyectiles[std::make_pair(MORTERO_P, true)]->setDimensiones(30, 30);
    // Granada verde.
    proyectiles[std::make_pair(GRANADA_VERDE_P, false)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/grenade.png", 60, 60, 32, true, CENTRO, 0, 2 * M_PI);
    // Granada roja.
    proyectiles[std::make_pair(GRANADA_ROJA_P, false)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/cluster.png", 60, 60, 32, true, CENTRO, 0, 2 * M_PI);
    proyectiles[std::make_pair(GRANADA_ROJA_P, true)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/clustlet.png", 60, 60, 6, true, CENTRO, 0, 2 * M_PI);
    // Granada santa.
    proyectiles[std::make_pair(GRANADA_SANTA_P, false)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/hgrenade.png", 60, 60, 32, true, CENTRO, 0, 2 * M_PI);
    // Banana.
    proyectiles[std::make_pair(BANANA_P, false)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/banana.png", 60, 60, 32, true, CENTRO, 0, 2 * M_PI);
    proyectiles[std::make_pair(BANANA_P, true)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/banana.png", 60, 60, 32, true, CENTRO, 0, 2 * M_PI);
    proyectiles[std::make_pair(BANANA_P, true)]->setDimensiones(30, 30);
    // Dinamita.
    proyectiles[std::make_pair(DINAMITA_P, false)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/dynamite.png", 60, 60, 125, true);
    // Ataque aereo.
    proyectiles[std::make_pair(ATAQUE_AEREO_P, false)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/airmisl.png", 60, 60, 32, true, CENTRO, 0, 2 * M_PI);

    // Explosiones.
    escenario[EXPLOSION] = std::make_shared<Animacion>(renderizador, "assets/sprites/circle50.png", 100, 100, 8, true);
    
}

void GestorAnimaciones::dibujarAgua(int& pos_x, int& pos_y, int& it) {
    escenario[AGUA]->dibujar(camara, pos_x, pos_y, false, it, 1);
}

void GestorAnimaciones::dibujarFondo() {
    int pos_x = ancho_mapa / 2;
    int pos_y = alto_mapa / 2;
    int it = 0;
    escenario[FONDO]->dibujar(camara, pos_x, pos_y, false, it, 1);
}

void GestorAnimaciones::dibujarPanorama(int& pos_x, int& pos_y) {
    int it = 0;
    escenario[PANORAMA]->dibujar(camara, pos_x, pos_y, false, it, 1);
}

void GestorAnimaciones::dibujarViga(int& pos_x, int& pos_y, int& largo, radianes& angulo) {
    int it = 0;
    if (largo > 10) {
        escenario[VIGA_GRANDE]->setDimensiones(largo * PIXELS_POR_METRO, 0.8 * PIXELS_POR_METRO);
        escenario[VIGA_GRANDE]->dibujar(camara, pos_x, pos_y, false, it, 1, angulo);
    } else {
        escenario[VIGA_CHICA]->setDimensiones(largo * PIXELS_POR_METRO, 0.8 * PIXELS_POR_METRO);
        escenario[VIGA_CHICA]->dibujar(camara, pos_x, pos_y, false, it, 1, angulo);
    }    
}

void GestorAnimaciones::dibujarGusano(EstadoGusano& estado, RepresentacionArma& arma, DireccionGusano& dir, int& pos_x, int& pos_y, int& it) {
    if(arma.tieneMira && (estado == QUIETO || estado == DISPARANDO)) {
        gusanos[std::make_pair(estado, arma.arma)]->dibujar(camara, pos_x, pos_y, dir == DERECHA, arma.anguloRad);
    } else {
        gusanos[std::make_pair(estado, arma.arma)]->dibujar(camara, pos_x, pos_y, dir == DERECHA, it, 1);
    }
}

void GestorAnimaciones::dibujarReticula(int& pos_x, int& pos_y, int& it) {
    escenario[RETICULA]->dibujar(camara, pos_x, pos_y, false, it, 1);
}

void GestorAnimaciones::dibujarIconoArma(ArmaProtocolo arma, int& pos_x, int& pos_y) {
    int it = 0;
    iconos[arma]->dibujar(camara, pos_x, pos_y, false, it, 1);
}

void GestorAnimaciones::dibujarProyectil(ArmaProtocolo& proyectil, bool& es_fragmento, int& pos_x, int& pos_y, radianes& angulo, int& it) {
    if (proyectil == DINAMITA_P)
        proyectiles[std::make_pair(proyectil, es_fragmento)]->dibujar(camara, pos_x, pos_y, false, it, 1, angulo);
    else
        proyectiles[std::make_pair(proyectil, es_fragmento)]->dibujar(camara, pos_x, pos_y, false, angulo);
}

void GestorAnimaciones::dibujarExplosion(ArmaProtocolo& proyectil, bool& es_fragmento, int& pos_x, int& pos_y, int& it) {
    if (es_fragmento) {
        escenario[EXPLOSION]->setDimensiones(30, 30);
    } else if (proyectil == DINAMITA_P) {
        escenario[EXPLOSION]->setDimensiones(70, 70);
    } else if (proyectil == GRANADA_SANTA_P) {
        escenario[EXPLOSION]->setDimensiones(100, 100);
    } else {
        escenario[EXPLOSION]->setDimensiones(50, 50);
    }
    escenario[EXPLOSION]->dibujar(camara, pos_x, pos_y, false, it, 1);
}

