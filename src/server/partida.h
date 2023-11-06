#ifndef PARTIDA_HEADER
#define PARTIDA_HEADER

#include "cliente.h"
#include "defs.h"
#include "gusano.h"
#include "thread.h"
#include <cstdint>
#include <map>
#include <sys/types.h>
#include <utility>
#include <vector>
#include "jugador.h"
#include "queue.h"
#include <condition_variable>
#include <mutex>
#include <box2d/box2d.h>

//El game loop ES nuestra funcion run
#define gameLoop run

#define MINJUGADORES 1

class Partida : public Thread {
    Queue<Direccion> acciones;

    //World de box2d de la partida
    b2World world;

    std::string mapa;

    std::vector<Cliente *> clientes;
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

    Gusano *anadirGusano(std::pair<coordX, coordY> coords);

public:
    Partida(const std::string mapa);

    void anadirCliente(Cliente *clienteNuevo);

    void gameLoop();
};

#endif
