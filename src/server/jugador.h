#ifndef JUGADOR_HEADER
#define JUGADOR_HEADER

#include "protocolo.h"
#include "socket.h"

class Jugador {
private:
    Protocolo protocolo;

public:
    Jugador(Socket&& socket);
};

#endif
