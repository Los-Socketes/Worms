#ifndef GUSANO_HEADER
#define GUSANO_HEADER

#include <box2d/box2d.h>
#include "defs.h"

class Gusano {
private:
    hp vida;
    DireccionGusano direccion;

    int idGusano;

    //Conceptos de b2box
    b2Body& cuerpo;

    void setDireccion(DireccionGusano nuevaDireccion);

    b2Vec2 getVectorDeDireccion(Direccion direccionDeseada);

public:
    Gusano(b2Body& cuerpo);

    void giveId(int idGusano);

    // [[nodiscard]] std::pair<cambioX, cambioY> cambio(Accion accion);

    std::pair<cambioX, cambioY> cambio(Accion accion);

    [[nodiscard]] std::pair<coordX, coordY> getCoords();

    [[nodiscard]] RepresentacionGusano getRepresentacion();
};

#endif
