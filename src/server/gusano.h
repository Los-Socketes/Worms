#ifndef GUSANO_HEADER
#define GUSANO_HEADER

#include <box2d/box2d.h>
#include "defs.h"
#include "arma.h"
#include "box2dDefs.h"

//Arma que el gusano quiere crear
#define ArmaDeseada ArmaProtocolo


struct Proyectil {
    ArmaProtocolo armaOrigen;
    b2Vec2 posicion;
    int id;
    int countdown;
    b2Vec2 *cuerpo;
    bool exploto;
};

struct Ataque {
    b2Vec2 posicion;
    int tiempoEspera;
    ArmaDeseada arma;
    Proyectil *proyectilAsociado;
};


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
    // int tiempoEspera;

    void setDireccion(DireccionGusano nuevaDireccion);

    //TODO Cambiar nombre
    void realizarMovimiento(Direccion direccionDeseada);

public:

    Gusano();

    void giveId(int idGusano);

    void giveGun(ArmaProtocolo arma);

    void teletransportarse();

    void preparar(Accion& accion);

    //TODO: Esta funcion recibe el arma o tipo de dano
    void recibirDano();

    // [[nodiscard]] std::pair<cambioX, cambioY> cambio(Accion accion);

    //WARNING: Esta funcion tiene que se ser llamada CERCA, sino AL
    //AL LADO del constructor. Gracias por nada c++, null pointers para
    //todos >:(
    void setCuerpo(b2Body* nuevoCuerpo);

    void setEstado(EstadoGusano nuevoEstado);

    [[nodiscard]] Ataque ejecutar(Accion accion);

    [[nodiscard]] std::pair<coordX, coordY> getCoords();

    [[nodiscard]] RepresentacionGusano getRepresentacion();

    [[nodiscard]] std::pair<inicioCaja, finCaja>  getAreaGolpe();
};

#endif
