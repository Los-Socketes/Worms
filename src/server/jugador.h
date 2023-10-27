#ifndef JUGADOR_HEADER
#define JUGADOR_HEADER

#include "gusano.h"
#include "protocolo.h"
#include <vector>

class Jugador {
private:
    Protocolo protocolo;

    int gusanoActual;
    std::vector<Gusano*> gusanos;

public:
    Jugador(Protocolo&& socket);
    Gusano& getGusanoActual();

    void obtenerGusano(std::vector<Gusano*> gusanos);
};

#endif
