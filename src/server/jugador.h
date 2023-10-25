#ifndef JUGADOR_HEADER
#define JUGADOR_HEADER

#include "protocolo.h"

class Jugador {
private:
    Protocolo protocolo;

public:
    Jugador(Protocolo&& socket);
};

#endif
