#ifndef GUSANO_HEADER
#define GUSANO_HEADER

#include "protocolo.h"
#include <box2d/box2d.h>
#include <sys/types.h>
#include "defs.h"

typedef uint hp;

class Gusano {
private:
    hp vida;
    DireccionGusano direccion;
    // std::pair<coordX, coordY> coords;

    //Conceptos de b2box
    b2Body& cuerpo;

    void setDireccion(DireccionGusano nuevaDireccion);

public:
    Gusano(b2Body& cuerpo);

    std::pair<cambioX, cambioY> cambio(Accion accion);
    [[nodiscard]] DireccionGusano getDireccion();

    std::pair<coordX, coordY> getCoords();
    // void setCoords(std::pair<coordX, coordY> nuevasCoords);

    void detener();
    void ponerEnMovimiento();
};



#endif
