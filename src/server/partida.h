#ifndef PARTIDA_HEADER
#define PARTIDA_HEADER

#include "gusano.h"
#include "jugador.h"
#include "protocolo.h"
#include "thread.h"
#include <cstdint>
#include <sys/types.h>
#include <utility>
#include <vector>
#include "comunes.h"

//El game loop ES nuestra funcion run
#define gameLoop run

enum class Accion { Mover, Saltar, Disparar };

class Partida : public Thread {
    int32_t id;

    std::vector<Jugador> jugadores;

    std::vector<Gusano*> gusanos;

    void turnoJugador();
public:
    Partida();

    void anadirJugador(Protocolo&& jugadorNuevo);

    void gameLoop();
};

#endif
