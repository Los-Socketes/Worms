#include "gestorsonidos.h"

GestorSonidos::GestorSonidos(Mixer& mixer) {
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
    // Sonidos de armas/proyectiles.
    sonidos[SONIDO_EXPLOSION] = std::make_shared<Sonido>(mixer, "assets/sounds/Explosion2.wav");
    sonidos[SONIDO_EXPLOSION_GRANDE] = std::make_shared<Sonido>(mixer, "assets/sounds/Explosion1.wav");
    sonidos[SONIDO_GRANADA_SANTA] = std::make_shared<Sonido>(mixer, "assets/sounds/HOLYGRENADE.WAV");
    sonidos[SONIDO_ATAQUE_AEREO] = std::make_shared<Sonido>(mixer, "assets/sounds/Airstrike.wav");
    sonidos[SONIDO_COMUNICACION] = std::make_shared<Sonido>(mixer, "assets/sounds/Communicator.wav");
    sonidos[SONIDO_TELETRANSPORTE] = std::make_shared<Sonido>(mixer, "assets/sounds/TELEPORT.WAV");
    sonidos[SONIDO_DINAMITA] = std::make_shared<Sonido>(mixer, "assets/sounds/FUSE.WAV");
    sonidos[SONIDO_BATE] = std::make_shared<Sonido>(mixer, "assets/sounds/BaseBallBatImpact.wav");
    // Sonidos de escenario.
    sonidos[SONIDO_TICK] = std::make_shared<Sonido>(mixer, "assets/sounds/TIMERTICK.WAV");
    sonidos[SONIDO_AGUA] = std::make_shared<Sonido>(mixer, "assets/sounds/Splash.wav");
    sonidos[SONIDO_SIN_MUNICIONES] = std::make_shared<Sonido>(mixer, "assets/sounds/NoAmmo.mp3");
}

/*
void GestorSonidos::reproducirSonidoGusano(IteradorGusano& iterador, EstadoGusano& estado, ArmaProtocolo& arma) {
    // Si el iterador está en 0 es porque la animacion cambio, reproduzco sonidos donde
    // corresponda. Para las animaciones de gusano caminando se reproducen dos sonidos, 
    // uno al inicio y otro a la mitad.
    switch (estado) {
        case CAMINANDO:
            if (iterador.it % gusanos[std::make_pair(estado, arma)]->getFrames() == 0) {
                gestor_sonidos.getSonido(SONIDO_GUSANO_CAMINA_INICIO)->reproducir();
            } else if (iterador.it % gusanos[std::make_pair(estado, arma)]->getFrames() == gusanos[std::make_pair(estado, arma)]->getFrames() / 2) {
                gestor_sonidos.getSonido(SONIDO_GUSANO_CAMINA_FIN)->reproducir();
            }
            break;
        case SALTANDO:
             if (iterador.it == 0) {
                gestor_sonidos.getSonido(SONIDO_GUSANO_SALTA)->reproducir();
             }
             break;
        case HACE_PIRUETA:
             if (iterador.it == 0) {
                gestor_sonidos.getSonido(SONIDO_GUSANO_PIRUETA)->reproducir();
             }
             break;
        case DISPARANDO:
            if (iterador.it == 0) {
                if (arma == BATE_P)
                    gestor_sonidos.getSonido(SONIDO_BATE)->reproducir();
                else if (arma == ATAQUE_AEREO_P)
                    gestor_sonidos.getSonido(SONIDO_ATAQUE_AEREO)->reproducir();
                else if (arma == TELETRANSPORTACION_P)
                    gestor_sonidos.getSonido(SONIDO_TELETRANSPORTE)->reproducir();
            }                
            break;
        case AHOGADO:
            if (iterador.it == 0) {
                gestor_sonidos.getSonido(SONIDO_AGUA)->reproducir();
            }
            break;
        case MUERTO:
            if (iterador.it == 59) {
                gestor_sonidos.getSonido(SONIDO_COMUNICACION)->reproducir();
            }
        default:
            break;
    }
}
*/
/*
void GestorSonidos::reproducirSonidoProyectil(IteradorProyectil& iterador, ArmaProtocolo& proyectil, bool exploto) {
    // Si el iterador está en 0 es porque la animacion cambio, reproduzco sonidos donde
    // corresponda. Para las animaciones con loop, se divide el iterador por la cantidad
    // de frames de la animacion y se reproduce el sonido cuando el resto es 0.
    if (iterador.it == 0) {
        if (proyectil == BAZOOKA_P || proyectil == MORTERO_P) {
            if (exploto)
                gestor_sonidos.getSonido(SONIDO_EXPLOSION)->reproducir();
            else
                gestor_sonidos.getSonido(SONIDO_DISPARO)->reproducir();
        }
        else if (proyectil == GRANADA_VERDE_P || proyectil == GRANADA_ROJA_P || proyectil == GRANADA_SANTA_P || proyectil == BANANA_P || proyectil == DINAMITA_P)
            if (exploto) {
                if (proyectil == GRANADA_SANTA_P) {
                    gestor_sonidos.getSonido(SONIDO_GRANADA_SANTA)->reproducir();
                    gestor_sonidos.getSonido(SONIDO_EXPLOSION_GRANDE)->reproducir();
                }
                else if (proyectil == DINAMITA_P)
                    gestor_sonidos.getSonido(SONIDO_EXPLOSION_GRANDE)->reproducir();
                else
                    gestor_sonidos.getSonido(SONIDO_EXPLOSION)->reproducir();
            } else {
                gestor_sonidos.getSonido(SONIDO_LANZAMIENTO)->reproducir();
            }
        else if (proyectil == ATAQUE_AEREO_P)
            gestor_sonidos.getSonido(SONIDO_ATAQUE_AEREO)->reproducir();
    }
    if (proyectil == DINAMITA_P && iterador.it % proyectiles[std::make_pair(proyectil, false)]->getFrames() == 0) {
        gestor_sonidos.getSonido(SONIDO_DINAMITA)->reproducir();
    }
}
*/

std::shared_ptr<Sonido> GestorSonidos::getSonido(TipoSonido tipo) {
    return sonidos[tipo];
}