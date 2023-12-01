#include "gestorSonidos.h"

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

std::shared_ptr<Sonido> GestorSonidos::getSonido(TipoSonido tipo) {
    return sonidos[tipo];
}