#include "gestormultimedia.h"

GestorMultimedia::GestorMultimedia(Camara& camara, int ancho_mapa, int alto_mapa) :
    camara(camara),
    ancho_mapa(ancho_mapa),
    alto_mapa(alto_mapa) {}

void GestorMultimedia::setDimensionMapa(int ancho, int alto) {
    ancho_mapa = ancho;
    alto_mapa = alto;
    escenario[FONDO]->setDimensiones(ancho_mapa, alto_mapa);
}

void GestorMultimedia::inicializar(Renderer& renderizador, Mixer& mixer) {

    // Animaciones

    // Animaciones de escenario/interfaz.

    escenario[AGUA] = std::make_shared<Animacion>(renderizador, "assets/sprites/water.png", 128, 100, 12, true, true);
    escenario[PANORAMA] = std::make_shared<Animacion>(renderizador, "assets/sprites/backforest.png", 640, 159, 1, true, false);
    escenario[FONDO] = std::make_shared<Animacion>(renderizador, "assets/sprites/back.png", 3000, 2000, 1, true, false);
    escenario[FONDOESPERA] = std::make_shared<Animacion>(renderizador, "assets/sprites/backwait.png", 3000, 2000, 1, false, false);
    escenario[IMAGENGUSANOESPERA] = std::make_shared<Animacion>(renderizador, "assets/sprites/backwaitworm.png", 1024, 768, 1, false, false);
    escenario[RETICULA] = std::make_shared<Animacion>(renderizador, "assets/sprites/crshairc.png", 60, 60, 32, true, true);
    escenario[VIGA_GRANDE] = std::make_shared<Animacion>(renderizador, "assets/sprites/grdl4.png", 140, 20, 1, true, false);
    escenario[VIGA_CHICA] = std::make_shared<Animacion>(renderizador, "assets/sprites/grds4.png", 70, 20, 1, true, false);
    escenario[FLECHA_GUSANO] = std::make_shared<Animacion>(renderizador, "assets/sprites/arrowdnc.png", 60, 60, 30, true, true);
    
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

    // Sonidos.

    // Sonidos de gusanos.

    sonidos[SONIDO_GUSANO_CAMINA_INICIO] = std::make_shared<Sonido>(mixer, "assets/sounds/Walk-Expand.wav");
    sonidos[SONIDO_GUSANO_CAMINA_FIN] = std::make_shared<Sonido>(mixer, "assets/sounds/Walk-Compress.wav");
    sonidos[SONIDO_GUSANO_SALTA] = std::make_shared<Sonido>(mixer, "assets/sounds/JUMP1.WAV");
    sonidos[SONIDO_GUSANO_PIRUETA] = std::make_shared<Sonido>(mixer, "assets/sounds/WORMSPRING.WAV");
    sonidos[SONIDO_CARGANDO_ARMA] = std::make_shared<Sonido>(mixer, "assets/sounds/ROCKETPOWERUP.WAV");
    sonidos[SONIDO_CARGANDO_LANZAMIENTO] = std::make_shared<Sonido>(mixer, "assets/sounds/THROWPOWERUP.WAV");
    sonidos[SONIDO_DISPARO] = std::make_shared<Sonido>(mixer, "assets/sounds/ROCKETRELEASE.WAV");
    sonidos[SONIDO_LANZAMIENTO] = std::make_shared<Sonido>(mixer, "assets/sounds/THROWRELEASE.WAV");
    sonidos[SONIDO_EXPLOSION] = std::make_shared<Sonido>(mixer, "assets/sounds/Explosion2.wav");
    sonidos[SONIDO_EXPLOSION_GRANDE] = std::make_shared<Sonido>(mixer, "assets/sounds/Explosion1.wav");
    sonidos[SONIDO_GRANADA_SANTA] = std::make_shared<Sonido>(mixer, "assets/sounds/HOLYGRENADE.WAV");
    sonidos[SONIDO_ATAQUE_AEREO] = std::make_shared<Sonido>(mixer, "assets/sounds/Airstrike.wav");
    sonidos[SONIDO_COMUNICACION] = std::make_shared<Sonido>(mixer, "assets/sounds/Communicator.wav");
    sonidos[SONIDO_TELETRANSPORTE] = std::make_shared<Sonido>(mixer, "assets/sounds/TELEPORT.WAV");
    sonidos[SONIDO_TICK] = std::make_shared<Sonido>(mixer, "assets/sounds/TIMERTICK.WAV");
    sonidos[SONIDO_DINAMITA] = std::make_shared<Sonido>(mixer, "assets/sounds/FUSE.WAV");
    sonidos[SONIDO_BATE] = std::make_shared<Sonido>(mixer, "assets/sounds/BaseBallBatImpact.wav");
    sonidos[SONIDO_AGUA] = std::make_shared<Sonido>(mixer, "assets/sounds/Splash.wav");
    sonidos[SONIDO_SIN_MUNICIONES] = std::make_shared<Sonido>(mixer, "assets/sounds/NoAmmo.mp3");

}

void GestorMultimedia::reproducirSonidoGusano(IteradorGusano& iterador, EstadoGusano& estado, ArmaProtocolo& arma) {
    // Si el iterador está en 0 es porque la animacion cambio, reproduzco sonidos donde
    // corresponda. Para las animaciones de gusano caminando se reproducen dos sonidos, 
    // uno al inicio y otro a la mitad.
    switch (estado) {
        case CAMINANDO:
            if (iterador.it % gusanos[std::make_pair(estado, arma)]->getFrames() == 0) {
                sonidos[SONIDO_GUSANO_CAMINA_INICIO]->reproducir();
            } else if (iterador.it % gusanos[std::make_pair(estado, arma)]->getFrames() == gusanos[std::make_pair(estado, arma)]->getFrames() / 2) {
                sonidos[SONIDO_GUSANO_CAMINA_FIN]->reproducir();
            }
            break;
        case SALTANDO:
             if (iterador.it == 0) {
                 sonidos[SONIDO_GUSANO_SALTA]->reproducir();
             }
             break;
        case HACE_PIRUETA:
             if (iterador.it == 0) {
                 sonidos[SONIDO_GUSANO_PIRUETA]->reproducir();
             }
             break;
        case DISPARANDO:
            if (iterador.it == 0) {
                if (arma == BATE_P)
                    sonidos[SONIDO_BATE]->reproducir();
                else if (arma == ATAQUE_AEREO_P)
                    sonidos[SONIDO_COMUNICACION]->reproducir();
                else if (arma == TELETRANSPORTACION_P)
                    sonidos[SONIDO_TELETRANSPORTE]->reproducir();
            }                
            break;
        case AHOGADO:
            if (iterador.it == 0) {
                sonidos[SONIDO_AGUA]->reproducir();
            }
            break;
        case MUERTO:
            if (iterador.it == 59) {
                sonidos[SONIDO_EXPLOSION]->reproducir();
            }
        default:
            break;
    }
}

void GestorMultimedia::reproducirSonidoProyectil(IteradorProyectil& iterador, ArmaProtocolo& proyectil, bool exploto) {
    // Si el iterador está en 0 es porque la animacion cambio, reproduzco sonidos donde
    // corresponda. Para las animaciones con loop, se divide el iterador por la cantidad
    // de frames de la animacion y se reproduce el sonido cuando el resto es 0.
    if (iterador.it == 0) {
        if (proyectil == BAZOOKA_P || proyectil == MORTERO_P) {
            if (exploto)
                sonidos[SONIDO_EXPLOSION]->reproducir();
            else
                sonidos[SONIDO_DISPARO]->reproducir();
        }
        else if (proyectil == GRANADA_VERDE_P || proyectil == GRANADA_ROJA_P || proyectil == GRANADA_SANTA_P || proyectil == BANANA_P || proyectil == DINAMITA_P)
            if (exploto) {
                if (proyectil == GRANADA_SANTA_P) {
                    sonidos[SONIDO_EXPLOSION_GRANDE]->reproducir();
                    sonidos[SONIDO_GRANADA_SANTA]->reproducir();
                }
                else if (proyectil == DINAMITA_P)
                    sonidos[SONIDO_EXPLOSION_GRANDE]->reproducir();
                else
                    sonidos[SONIDO_EXPLOSION]->reproducir();
            } else {
                sonidos[SONIDO_LANZAMIENTO]->reproducir();
            }
        else if (proyectil == ATAQUE_AEREO_P)
            sonidos[SONIDO_ATAQUE_AEREO]->reproducir();
    }
    if (proyectil == DINAMITA_P && iterador.it % proyectiles[std::make_pair(proyectil, false)]->getFrames() == 0) {
        sonidos[SONIDO_DINAMITA]->reproducir();
    }
}

void GestorMultimedia::dibujarAgua(int& pos_x, int& pos_y, int& it) {
    escenario[AGUA]->dibujar(camara, pos_x, pos_y, false, it, 1);
}

void GestorMultimedia::dibujarFondo() {
    int pos_x = ancho_mapa / 2;
    int pos_y = alto_mapa / 2;
    int it = 0;
    escenario[FONDO]->dibujar(camara, pos_x, pos_y, false, it, 1);
}

void GestorMultimedia::dibujarFondoEspera(int& ancho_pantalla, int& alto_pantalla) {
    int pos_x = ancho_pantalla / 2;
    int pos_y = alto_pantalla / 2;
    int it = 0;
    escenario[FONDOESPERA]->setDimensiones(ancho_pantalla, alto_pantalla);
    escenario[FONDOESPERA]->dibujar(camara, pos_x, pos_y, false, it, 1);
}

void GestorMultimedia::dibujarGusanoEspera(int& ancho_pantalla, int& alto_pantalla) {
    // Cambio el tamaño y la posicion segun el tamaño de la pantalla.
    // La imagen tiene relacion de aspecto 4:3, hago que siempre ocupe el 50% del ancho de la pantalla.
    int tam_x = ancho_pantalla / 2;
    int tam_y = tam_x * 3 / 4;
    escenario[IMAGENGUSANOESPERA]->setDimensiones(tam_x, tam_y);
    // Lo dibujo abajo a la izquierda.
    int pos_x = tam_x / 2;
    int pos_y = alto_pantalla - tam_y / 2;
    int it = 0;
    escenario[IMAGENGUSANOESPERA]->dibujar(camara, pos_x, pos_y, false, it, 1);
}


void GestorMultimedia::dibujarPanorama(int& pos_x, int& pos_y) {
    int it = 0;
    escenario[PANORAMA]->dibujar(camara, pos_x, pos_y, false, it, 1);
}

void GestorMultimedia::dibujarViga(int& pos_x, int& pos_y, int& largo, radianes& angulo) {
    int it = 0;
    radianes ang = -angulo;
    if (largo > 3) {
        escenario[VIGA_GRANDE]->setDimensiones(largo * PIXELS_POR_METRO, 0.8 * PIXELS_POR_METRO);
        escenario[VIGA_GRANDE]->dibujar(camara, pos_x, pos_y, false, it, 1, ang);
    } else {
        escenario[VIGA_CHICA]->setDimensiones(largo * PIXELS_POR_METRO, 0.8 * PIXELS_POR_METRO);
        escenario[VIGA_CHICA]->dibujar(camara, pos_x, pos_y, false, it, 1, ang);
    }    
}

void GestorMultimedia::dibujarGusano(id& id_gusano, EstadoGusano& estado, RepresentacionArma& arma, DireccionGusano& dir, int& pos_x, int& pos_y, ControlIteracion& iteraciones) {
    // Si la animacion cambio, reseteo el iterador.
    iteraciones.actualizarAnimacionGusano(id_gusano, gusanos[std::make_pair(estado, arma.arma)]);
    if(arma.tieneMira && (estado == QUIETO || estado == DISPARANDO)) {
        gusanos[std::make_pair(estado, arma.arma)]->dibujar(camara, pos_x, pos_y, dir == DERECHA, arma.anguloRad);
    } else {
        gusanos[std::make_pair(estado, arma.arma)]->dibujar(camara, pos_x, pos_y, dir == DERECHA, iteraciones.getIteracionGusano(id_gusano), 1);
    }
    reproducirSonidoGusano(iteraciones.getIteradorGusano(id_gusano), estado, arma.arma);
}

void GestorMultimedia::dibujarReticula(int& pos_x, int& pos_y, int& it) {
    escenario[RETICULA]->dibujar(camara, pos_x, pos_y, false, it, 1);
}

void GestorMultimedia::dibujarCursor(int& pos_x, int& pos_y, int& it) {
    escenario[RETICULA]->dibujar(camara, pos_x, pos_y, false, it, 1);
}

void GestorMultimedia::dibujarIconoArma(ArmaProtocolo arma, int& pos_x, int& pos_y) {
    int it = 0;
    iconos[arma]->dibujar(camara, pos_x, pos_y, false, it, 1);
}

void GestorMultimedia::dibujarProyectil(idProyectil& id_proyectil, ArmaProtocolo& proyectil, bool& es_fragmento, int& pos_x, int& pos_y, radianes& angulo, ControlIteracion& iteraciones) {
    // Si la animacion cambio, reseteo el iterador.
    iteraciones.actualizarAnimacionProyectil(id_proyectil, proyectiles[std::make_pair(proyectil, es_fragmento)]);
    if (proyectil == DINAMITA_P)
        proyectiles[std::make_pair(proyectil, es_fragmento)]->dibujar(camara, pos_x, pos_y, false, iteraciones.getIteracionProyectil(id_proyectil), 1, angulo);
    else
        proyectiles[std::make_pair(proyectil, es_fragmento)]->dibujar(camara, pos_x, pos_y, false, angulo);
    reproducirSonidoProyectil(iteraciones.getIteradorProyectil(id_proyectil), proyectil, false);
}

void GestorMultimedia::dibujarExplosion(idProyectil& id_proyectil, ArmaProtocolo& proyectil, bool& es_fragmento, int& pos_x, int& pos_y, ControlIteracion& iteraciones) {
    // Si la animacion cambio, reseteo el iterador.
    iteraciones.actualizarAnimacionProyectil(id_proyectil, escenario[EXPLOSION]);
    if (es_fragmento) {
        escenario[EXPLOSION]->setDimensiones(70, 70);
    } else if (proyectil == DINAMITA_P) {
        escenario[EXPLOSION]->setDimensiones(120, 120);
    } else if (proyectil == GRANADA_SANTA_P) {
        escenario[EXPLOSION]->setDimensiones(200, 200);
    } else {
        escenario[EXPLOSION]->setDimensiones(100, 100);
    }
    escenario[EXPLOSION]->dibujar(camara, pos_x, pos_y, false, iteraciones.getIteracionProyectil(id_proyectil), 1);
    reproducirSonidoProyectil(iteraciones.getIteradorProyectil(id_proyectil), proyectil, true);
}

void GestorMultimedia::dibujarFlechaGusano(int& pos_x, int& pos_y, int& it) {
    escenario[FLECHA_GUSANO]->dibujar(camara, pos_x, pos_y, false, it, 1);
}

void GestorMultimedia::reproducirSonido(TipoSonido tipo) {
    sonidos[tipo]->reproducir();
}