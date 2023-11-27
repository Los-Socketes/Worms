#ifndef JUGADOR_HEADER
#define JUGADOR_HEADER

#include "defs.h"
#include "gusano.h"
#include <vector>

class Jugador {
private:

    int gusanoActualPos;

    std::vector<Gusano*> gusanos;

    SituacionJugador miSituacion;

public:
    Jugador(std::vector<Gusano*> gusanos);

    Gusano* getGusanoActual();
    Gusano *getGusanoDeTurno();

    std::map<id, RepresentacionGusano> getRepresentacionGusanos();

    [[nodiscard]] SituacionJugador getSituacion();

    void avisarQueGane();

    void chequearSiPerdi();

};

#endif
