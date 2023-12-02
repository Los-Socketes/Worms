#include "gestorAnimaciones.h"

GestorAnimaciones::GestorAnimaciones(Renderer& renderizador, int& ancho_mapa, int& alto_mapa) {
    // Animaciones

    // Animaciones de escenario/interfaz.

    escenario[AGUA] = std::make_shared<Animacion>(renderizador, "assets/sprites/water.png", 128, 100, 12, true, true);
    escenario[PANORAMA] = std::make_shared<Animacion>(renderizador, "assets/sprites/backforest.png", 640, 159, 1, true, false);
    escenario[FONDO] = std::make_shared<Animacion>(renderizador, "assets/sprites/back.png", 3000, 2000, 1, true, false);
    escenario[FONDO]->setDimensiones(ancho_mapa, alto_mapa);
    escenario[FONDOESPERA] = std::make_shared<Animacion>(renderizador, "assets/sprites/backwait.png", 3000, 2000, 1, false, false);
    escenario[IMAGENGUSANOESPERA] = std::make_shared<Animacion>(renderizador, "assets/sprites/backwaitworm.png", 1024, 768, 1, false, false);
    escenario[RETICULA] = std::make_shared<Animacion>(renderizador, "assets/sprites/crshairc.png", 60, 60, 32, true, true);
    escenario[VIGA_GRANDE] = std::make_shared<Animacion>(renderizador, "assets/sprites/grdl4.png", 140, 20, 1, true, false);
    escenario[VIGA_CHICA] = std::make_shared<Animacion>(renderizador, "assets/sprites/grds4.png", 70, 20, 1, true, false);
    escenario[FLECHA_GUSANO] = std::make_shared<Animacion>(renderizador, "assets/sprites/arrowdnc.png", 60, 60, 30, true, true);
    
    escenario[VOLUMEN_0] = std::make_shared<Animacion>(renderizador, "assets/sprites/volume0.png", 100, 100, 1, false, false);
    escenario[VOLUMEN_0]->setDimensiones(35, 35);
    escenario[VOLUMEN_1] = std::make_shared<Animacion>(renderizador, "assets/sprites/volume1.png", 100, 100, 1, false, false);
    escenario[VOLUMEN_1]->setDimensiones(35, 35);
    escenario[VOLUMEN_2] = std::make_shared<Animacion>(renderizador, "assets/sprites/volume2.png", 100, 100, 1, false, false);
    escenario[VOLUMEN_2]->setDimensiones(35, 35);
    escenario[VOLUMEN_3] = std::make_shared<Animacion>(renderizador, "assets/sprites/volume3.png", 100, 100, 1, false, false);
    escenario[VOLUMEN_3]->setDimensiones(35, 35);
    escenario[VOLUMEN_MUTEADO] = std::make_shared<Animacion>(renderizador, "assets/sprites/volumemute.png", 100, 100, 1, false, false);
    escenario[VOLUMEN_MUTEADO]->setDimensiones(35, 35);

    // Provisiones.
    escenario[PROVISION_MEDICINA] = std::make_shared<Animacion>(renderizador, "assets/sprites/mcrate1.png", 60, 60, 12, true, true);
    escenario[PROVISION_MEDICINA_CAYENDO] = std::make_shared<Animacion>(renderizador, "assets/sprites/mcrate2.png", 74, 74, 20, true, true);
    escenario[PROVISION_MUNICION] = std::make_shared<Animacion>(renderizador, "assets/sprites/wcrate1.png", 60, 60, 1, true, false);
    escenario[PROVISION_MUNICION_CAYENDO] = std::make_shared<Animacion>(renderizador, "assets/sprites/wcrate2.png", 74, 74, 20, true, true);

    // Gusano ganador.
    escenario[GUSANO_GANADOR] = std::make_shared<Animacion>(renderizador, "assets/sprites/wwinner.png", 60, 60, 14, true, true);

    // Iconos de armas.
    iconos[NADA_P] = std::make_shared<Animacion>(renderizador, "assets/sprites/inothing.png", 32, 32, 1, false, false);
    iconos[BATE_P] = std::make_shared<Animacion>(renderizador, "assets/sprites/ibaseball.png", 32, 32, 1, false, false);
    iconos[BAZOOKA_P] = std::make_shared<Animacion>(renderizador, "assets/sprites/ibazooka.png", 32, 32, 1, false, false);
    iconos[MORTERO_P] = std::make_shared<Animacion>(renderizador, "assets/sprites/imortar.png", 32, 32, 1, false, false);
    iconos[GRANADA_VERDE_P] = std::make_shared<Animacion>(renderizador, "assets/sprites/igrenade.png", 32, 32, 1, false, false);
    iconos[GRANADA_ROJA_P] = std::make_shared<Animacion>(renderizador, "assets/sprites/icluster.png", 32, 32, 1, false, false);
    iconos[DINAMITA_P] = std::make_shared<Animacion>(renderizador, "assets/sprites/idynamite.png", 32, 32, 1, false, false);
    iconos[BANANA_P] = std::make_shared<Animacion>(renderizador, "assets/sprites/ibanana.png", 32, 32, 1, false, false);
    iconos[GRANADA_SANTA_P] = std::make_shared<Animacion>(renderizador, "assets/sprites/ihgrenade.png", 32, 32, 1, false, false);
    iconos[ATAQUE_AEREO_P] = std::make_shared<Animacion>(renderizador, "assets/sprites/iairstrke.png", 32, 32, 1, false, false);
    iconos[TELETRANSPORTACION_P] = std::make_shared<Animacion>(renderizador, "assets/sprites/iteleport.png", 32, 32, 1, false, false);

    // Animaciones de gusanos.

    // Gusano sin armas.    
    gusanos[std::make_pair(QUIETO, NADA_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wblink1.png", 60, 60, 1, true, false);
    gusanos[std::make_pair(CAMINANDO, NADA_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wwalk.png", 60, 60, 15, true, true);
    gusanos[std::make_pair(SALTANDO, NADA_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wflyup.png", 60, 60, 2, true, true);
    gusanos[std::make_pair(CAYENDO, NADA_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wfall.png", 60, 60, 2, true, true);
    gusanos[std::make_pair(DISPARANDO, NADA_P)] = gusanos[std::make_pair(QUIETO, NADA_P)];
    gusanos[std::make_pair(HERIDO, NADA_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wroll.png", 60, 60, 32, true, true);
    gusanos[std::make_pair(HACE_PIRUETA, NADA_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wbackflp.png", 60, 60, 20, true, false);
    gusanos[std::make_pair(MUERTO, NADA_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wdie.png", 60, 60, 69, true, false);
    gusanos[std::make_pair(AHOGADO, NADA_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wgrave.png", 60, 60, 1, true, false);

    // Asigno todas las animaciones que no cambian.
    // Recorro todas las armas.
    for (int i = 1; i < 11; i++) {
        // Recorro todos los estados.
        for (int j = 0; j < 8; j++) {
            gusanos[std::make_pair(EstadoGusano(j), ArmaProtocolo(i))] = gusanos[std::make_pair(EstadoGusano(j), NADA_P)];
        }
    } 

    // Reasigno las animaciones que cambian.
    
    // Gusano con bazooka.
    gusanos[std::make_pair(QUIETO, BAZOOKA_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wbaz.png", 60, 60, 32, true, false);
    gusanos[std::make_pair(DISPARANDO, BAZOOKA_P)] = gusanos[std::make_pair(QUIETO, BAZOOKA_P)];

    // Gusano con mortero.
    gusanos[std::make_pair(QUIETO, MORTERO_P)] = gusanos[std::make_pair(QUIETO, BAZOOKA_P)];
    gusanos[std::make_pair(DISPARANDO, MORTERO_P)] = gusanos[std::make_pair(QUIETO, BAZOOKA_P)];

    // Gusano con granada verde.
    gusanos[std::make_pair(QUIETO, GRANADA_VERDE_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wthrgrn.png", 60, 60, 32, true, false);

    // Gusano con granada roja.
    gusanos[std::make_pair(QUIETO, GRANADA_ROJA_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wthrcls.png", 60, 60, 32, true, false);

    // Gusano con granada santa.
    gusanos[std::make_pair(QUIETO, GRANADA_SANTA_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wthrhgr.png", 60, 60, 32, true, false);

    // Gusano con dinamita.
    gusanos[std::make_pair(QUIETO, DINAMITA_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wdynbak.png", 60, 60, 1, true, false);

    // Gusano con banana.
    gusanos[std::make_pair(QUIETO, BANANA_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wthrban.png", 60, 60, 32, true, false);

    // Gusano con bate de baseball.
    gusanos[std::make_pair(QUIETO, BATE_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wbsbaim.png", 60, 60, 32, true, false);
    gusanos[std::make_pair(DISPARANDO, BATE_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wbsbswn.png", 60, 60, 32, true, false);

    // Gusano con ataque aereo.
    gusanos[std::make_pair(QUIETO, ATAQUE_AEREO_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wairtlk.png", 60, 60, 1, true, false);
    gusanos[std::make_pair(DISPARANDO, ATAQUE_AEREO_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wairtlk.png", 60, 60, 10, true, true);

    // Gusano con teletransportacion.
    gusanos[std::make_pair(QUIETO, TELETRANSPORTACION_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wteltlk.png", 60, 60, 10, true, true);
    gusanos[std::make_pair(DISPARANDO, TELETRANSPORTACION_P)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/wteldsv.png", 60, 60, 97, true, false);

    // Proyectiles.    
    
    // Bazooka.
    proyectiles[std::make_pair(BAZOOKA_P, false)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/missile.png", 60, 60, 32, true, false, CENTRO, 0, 2 * M_PI);
    // Mortero
    proyectiles[std::make_pair(MORTERO_P, false)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/mortar.png", 60, 60, 32, true, false, CENTRO, 0, 2 * M_PI);
    proyectiles[std::make_pair(MORTERO_P, true)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/mortar.png", 60, 60, 32, true, false, CENTRO, 0, 2 * M_PI);
    proyectiles[std::make_pair(MORTERO_P, true)]->setDimensiones(30, 30);
    // Granada verde.
    proyectiles[std::make_pair(GRANADA_VERDE_P, false)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/grenade.png", 60, 60, 32, true, false, CENTRO, 0, 2 * M_PI);
    // Granada roja.
    proyectiles[std::make_pair(GRANADA_ROJA_P, false)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/cluster.png", 60, 60, 32, true, false, CENTRO, 0, 2 * M_PI);
    proyectiles[std::make_pair(GRANADA_ROJA_P, true)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/clustlet.png", 60, 60, 6, true, false, CENTRO, 0, 2 * M_PI);
    // Granada santa.
    proyectiles[std::make_pair(GRANADA_SANTA_P, false)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/hgrenade.png", 60, 60, 32, true, false, CENTRO, 0, 2 * M_PI);
    // Banana.
    proyectiles[std::make_pair(BANANA_P, false)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/banana.png", 60, 60, 32, true, false, CENTRO, 0, 2 * M_PI);
    proyectiles[std::make_pair(BANANA_P, true)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/banana.png", 60, 60, 32, true, false, CENTRO, 0, 2 * M_PI);
    proyectiles[std::make_pair(BANANA_P, true)]->setDimensiones(30, 30);
    // Dinamita.
    proyectiles[std::make_pair(DINAMITA_P, false)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/dynamite.png", 60, 60, 6, true, true);
    // Ataque aereo.
    proyectiles[std::make_pair(ATAQUE_AEREO_P, false)] = std::make_shared<Animacion>(
        renderizador, "assets/sprites/airmisl.png", 60, 60, 32, true, false, CENTRO, 0, 2 * M_PI);

    // Explosiones.
    escenario[EXPLOSION] = std::make_shared<Animacion>(renderizador, "assets/sprites/circle50.png", 100, 100, 9, true, false);
}


std::shared_ptr<Animacion> GestorAnimaciones::getAnimacionGusano(EstadoGusano& estado, ArmaProtocolo& arma) {
    return gusanos[std::make_pair(estado, arma)];
}

std::shared_ptr<Animacion> GestorAnimaciones::getAnimacionProyectil(ArmaProtocolo& proyectil, bool& es_fragmento) {
    return proyectiles[std::make_pair(proyectil, es_fragmento)];
}

std::shared_ptr<Animacion> GestorAnimaciones::getAnimacionEscenario(ItemEscenario item) {
    return escenario[item];
}

std::shared_ptr<Animacion> GestorAnimaciones::getImagenIcono(ArmaProtocolo arma) {
    return iconos[arma];
}