#ifndef GESTORSONIDOS_H_
#define GESTORSONIDOS_H_

#include <map>
#include <utility>
#include <string>
#include <memory>
#include <SDL2pp/SDL2pp.hh>

#include "defs.h"
#include "sonido.h"

enum TipoSonido {
    SONIDO_GUSANO_CAMINA_INICIO,
    SONIDO_GUSANO_CAMINA_FIN,
    SONIDO_GUSANO_SALTA,
    SONIDO_GUSANO_PIRUETA,
    SONIDO_CARGANDO_ARMA,
    SONIDO_CARGANDO_LANZAMIENTO,
    SONIDO_DISPARO,
    SONIDO_LANZAMIENTO,
    SONIDO_EXPLOSION,
    SONIDO_EXPLOSION_GRANDE,
    SONIDO_GRANADA_SANTA,
    SONIDO_ATAQUE_AEREO,
    SONIDO_COMUNICACION,
    SONIDO_TELETRANSPORTE,
    SONIDO_TICK,
    SONIDO_DINAMITA,
    SONIDO_BATE,
    SONIDO_AGUA,
    SONIDO_SIN_MUNICIONES
};

// Clase que se encarga de cargar los sonidos y devolverlos para ser reproducidos.
class GestorSonidos {
 private:
    std::map<TipoSonido, std::shared_ptr<Sonido>> sonidos;
 public:
    GestorSonidos(Mixer& mixer);
    std::shared_ptr<Sonido> getSonido(TipoSonido tipo);
};

#endif // GESTORSONIDOS_H_
