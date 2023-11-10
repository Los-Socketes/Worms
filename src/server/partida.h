#ifndef PARTIDA_HEADER
#define PARTIDA_HEADER

#include "cliente.h"
#include "defs.h"
#include "gusano.h"
#include "thread.h"
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
    Queue<Accion> acciones;

    //World de box2d de la partida
    b2World world;

    std::string mapa;

    std::vector<Cliente *> clientes;
    std::mutex mtx;
    std::condition_variable seUnioJugador;

    std::vector<Gusano *> gusanos;

    std::vector<Jugador *> jugadores;

    void enviarEstadoAJugadores();

    [[nodiscard]] Accion obtenerAccion(Accion accionObtenida, bool obtuvoNueva,
		     Accion& ultimaAccion);

    [[nodiscard]] Gusano *anadirGusano(std::pair<coordX, coordY> coords);

    void anadirViga(radianes angulo, int longitud, std::pair<coordX, coordY> posicionInicial);

public:
    Partida(const std::string mapa);

    [[nodiscard]] idJugador anadirCliente(Cliente *clienteNuevo);

    void gameLoop();
};

#endif
