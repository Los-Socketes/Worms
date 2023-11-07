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
    Jugador(std::vector<Gusano*> gusanos);
    Gusano* getGusanoActual();

    // void obtenerAccesoAAcciones(Queue<Direccion>* acciones);

    // bool obtenerAccion(Direccion& primeraAccion);
    std::vector<RepresentacionGusano> getRepresentacionGusanos();

};

#endif
