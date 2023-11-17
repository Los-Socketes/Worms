#ifndef GUSANO_HEADER
#define GUSANO_HEADER

#include <box2d/box2d.h>
#include "defs.h"
#include "arma.h"
#include "box2dDefs.h"

//Arma que el gusano quiere crear
#define ArmaDeseada ArmaProtocolo



class Gusano {
private:
    hp vida;
    DireccionGusano direccion;

    int idGusano;

    //Arma armaequipada;
    ArmaProtocolo armaEquipada;
    Arma armaSeleccionada;

    EstadoGusano estado;

    //Conceptos de b2box
    b2Body* cuerpo;

    void setDireccion(DireccionGusano nuevaDireccion);

    //TODO Cambiar nombre
    void realizarMovimiento(Direccion direccionDeseada);

public:

    Gusano();

    void giveId(int idGusano);

    void giveGun(ArmaProtocolo arma);

    // [[nodiscard]] std::pair<cambioX, cambioY> cambio(Accion accion);

    //WARNING: Esta funcion tiene que se ser llamada CERCA, sino AL
    //AL LADO del constructor. Gracias por nada c++, null pointers para
    //todos >:(
    void setCuerpo(b2Body* nuevoCuerpo);

    void setEstado(EstadoGusano nuevoEstado);

    [[nodiscard]] ArmaDeseada ejecutar(Accion accion);

    [[nodiscard]] std::pair<coordX, coordY> getCoords();

    [[nodiscard]] RepresentacionGusano getRepresentacion();

    [[nodiscard]] std::pair<coordX, coordY> getCoordsArma();

    [[nodiscard]] std::pair<inicioCaja, finCaja>  getAreaGolpe();
};

#endif
