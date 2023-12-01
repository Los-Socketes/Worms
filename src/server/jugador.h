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
    Jugador();

    void setGusanos(std::vector<Gusano*>gusanos);
    void anadirGusano(Gusano *nuevoGusano);
    void darMasVidaAGusanos();
    Gusano* getGusanoActual();
    Gusano *getGusanoDeTurno();

    std::map<id, RepresentacionGusano> getRepresentacionGusanos();

    [[nodiscard]] SituacionJugador getSituacion();

    void avisarQueGane();

    void chequearSiPerdi();

};

#endif
