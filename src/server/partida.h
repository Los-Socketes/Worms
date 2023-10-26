#ifndef PARTIDA_HEADER
#define PARTIDA_HEADER

#include "jugador.h"
#include "protocolo.h"
#include <cstdint>
#include <sys/types.h>
#include <vector>

class Partida {
    int32_t id;

    std::vector<Jugador> jugadores;
public:
    Partida();

    void anadirJugador(Protocolo&& jugadorNuevo);
};

#endif
