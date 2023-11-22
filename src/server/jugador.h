#ifndef JUGADOR_HEADER
#define JUGADOR_HEADER

#include "defs.h"
#include "gusano.h"
#include <vector>

class Jugador {
private:

    int gusanoActualPos;

    std::vector<Gusano*> gusanos;

public:
    Jugador(std::vector<Gusano*> gusanos);

    bool esMiTurno;

    Gusano* getGusanoActual();

    std::map<id, RepresentacionGusano> getRepresentacionGusanos();

};

#endif
