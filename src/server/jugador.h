#ifndef JUGADOR_HEADER
#define JUGADOR_HEADER

#include "gusano.h"
#include "protocolo.h"
#include "queue.h"
#include "reciever.h"
#include "sender.h"
#include <vector>

class Jugador {
private:
    Protocolo protocolo;
    Sender enviador;
    Reciever recibidor;

    int gusanoActual;
    std::vector<Gusano*> gusanos;

    //TODO Esto seria de accion en el futuro. O anadir otra queue (eh)
    //o cambiar
    Queue<Direccion> acciones;

public:
    Jugador(Protocolo&& socket);
    Gusano& getGusanoActual();

    void obtenerGusanosIniciales(std::vector<Gusano*> gusanos);

    bool obtenerAccion(Direccion& primeraAccion);
};

#endif
