#ifndef PARTIDA_HEADER
#define PARTIDA_HEADER

#include "gusano.h"
#include "thread.h"
#include <cstdint>
#include <map>
#include <sys/types.h>
#include <utility>
#include <vector>
#include "comunes.h"
#include "threadSafeList.h"
#include "jugador.h"

//El game loop ES nuestra funcion run
#define gameLoop run

//Forward declaration por include circulares
// class Jugador;


enum class Accion { Mover, Saltar, Disparar };

class Partida : public Thread {
    std::string mapa;

    //Esto tiene que ser thread safe porque se modifica en hilos
    //distintos
    TSList<Jugador *> jugadores;

    TSList<Gusano *> gusanos;

    std::map<std::pair<int, int>, Gusano *> coordsGusanos;

    //TODO: Aca estaria Box2d
    std::pair<int, int> gravedad(std::pair<int, int> cambioDeseado,
			   std::pair<int, int> posInicial
			   );

public:
    Partida(const std::string mapa);

    void anadirJugador(Jugador  *jugadorNuevo);

    void gameLoop();
};

#endif
