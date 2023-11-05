#ifndef PARTIDA_HEADER
#define PARTIDA_HEADER

#include "defs.h"
#include "gusano.h"
#include "thread.h"
#include <cstdint>
#include <map>
#include <sys/types.h>
#include <utility>
#include <vector>
#include "comunes.h"
#include "TSList.h"
#include "jugador.h"
#include <condition_variable>
#include <mutex>

//El game loop ES nuestra funcion run
#define gameLoop run

#define MINJUGADORES 1

// enum class Accion { Mover, Saltar, Disparar };

class Partida : public Thread {
    Queue<Direccion> acciones;

    std::string mapa;

    std::vector<Jugador *> jugadores;
    std::mutex mtx;
    std::condition_variable seUnioJugador;

    // std::vector<Gusano *> gusanos;

    std::map<std::pair<coordX, coordY>, Gusano *> coordsGusanos;

    //TODO: Aca estaria Box2d
    std::pair<coordX, coordY> gravedad(std::pair<cambioX, cambioY> cambioDeseado,
			   std::pair<coordX, coordY> posInicial
			   );

    void enviarEstadoAJugadores();

    Accion obtenerAccion(Direccion accionObtenida, bool obtuvoNueva,
		     Accion& ultimaAccion);

public:
    Partida(const std::string mapa);

    void anadirJugador(Jugador  *jugadorNuevo);

    void gameLoop();
};

#endif
