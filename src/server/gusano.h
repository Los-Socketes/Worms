#ifndef GUSANO_HEADER
#define GUSANO_HEADER

#include <box2d/box2d.h>
#include "defs.h"

//Arma que el gusano quiere crear
#define ArmaDeseada ArmaProtocolo


class Gusano {
private:
    hp vida;
    DireccionGusano direccion;

    int idGusano;

    //Arma armaequipada;
    ArmaProtocolo armaEquipada;

    EstadoGusano estado;

    //Conceptos de b2box
    b2Body& cuerpo;

    void setDireccion(DireccionGusano nuevaDireccion);

    //TODO Cambiar nombre
    void realizarMovimiento(Direccion direccionDeseada);

public:
    Gusano(b2Body& cuerpo);

    void giveId(int idGusano);

    void giveGun(ArmaProtocolo arma);

    // [[nodiscard]] std::pair<cambioX, cambioY> cambio(Accion accion);

    [[nodiscard]] ArmaDeseada ejecutar(Accion accion);

    [[nodiscard]] std::pair<coordX, coordY> getCoords();

    [[nodiscard]] RepresentacionGusano getRepresentacion();

    [[nodiscard]] std::pair<coordX, coordY> getCoordsArma();
};

#endif
