#ifndef JUGADOR_HEADER
#define JUGADOR_HEADER

#include "defs.h"
#include "gusano.h"
// #include "queue.h"
#include <vector>

//Forward declaration por include circulares
// class MonitorPartida;

class Jugador {
private:

    int gusanoActualPos;
    std::vector<Gusano*> gusanos;

    //TODO Esto seria de accion en el futuro. O anadir otra queue (eh)
    //o cambiar
    // Queue<Direccion>* acciones;

public:
    Jugador();
    Gusano* getGusanoActual();

    void obtenerGusanosIniciales(std::vector<Gusano*> gusanos);

    // void obtenerAccesoAAcciones(Queue<Direccion>* acciones);

    // bool obtenerAccion(Direccion& primeraAccion);

};

#endif
