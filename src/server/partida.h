#ifndef PARTIDA_HEADER
#define PARTIDA_HEADER

#include "jugador.h"
#include "protocolo.h"
#include <sys/types.h>
#include <vector>

class Partida {
    std::vector<Jugador> jugadores;
public:
    void anadirJugador(Protocolo&& jugadorNuevo);
};

#endif
