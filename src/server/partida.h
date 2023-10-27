#ifndef PARTIDA_HEADER
#define PARTIDA_HEADER

#include "gusano.h"
#include "jugador.h"
#include "thread.h"
#include <cstdint>
#include <sys/types.h>
#include <utility>
#include <vector>
#include "comunes.h"
#include "threadSafeList.h"

//El game loop ES nuestra funcion run
#define gameLoop run

enum class Accion { Mover, Saltar, Disparar };

class Partida : public Thread {
    std::string mapa;

    //Esto tiene que ser thread safe porque se modifica en hilos
    //distintos
    TSList<Jugador *> jugadores;

    TSList<Gusano *> gusanos;

    void turnoJugador();


public:
    Partida(std::string mapa);

    void anadirJugador(Jugador  *jugadorNuevo);

    void gameLoop();
};

#endif
