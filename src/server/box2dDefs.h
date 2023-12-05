#ifndef BOX2DDEFS_HEADER
#define BOX2DDEFS_HEADER

#include <box2d/box2d.h>
#include "defs.h"

//Forward declaration
class Gusano;
class Provision;

/*
    *Coordenadas en Box2d:
    *Arranca abajo a la izquierda.
    *X:
    *	>: +1
    *	<: -1
    *Y:
    *	^: +1
    *	v: -1
*/

#define DEGTORAD 0.0174532925199432957f

#define ANCHOVIGA 0.8f

#define MASACUERPOESTATICO 0.0f

#define NOT !

typedef b2Vec2 inicioCaja;
typedef b2Vec2 finCaja;

#define inicio first
#define fin second

[[nodiscard]] inline std::pair<coordX, coordY> deb2VecACoord(b2Vec2 vec) {
    std::pair<coordX, coordY> representacionCoord;
    representacionCoord.enX = vec.x;
    representacionCoord.enY = vec.y;

    return representacionCoord;
}

[[nodiscard]] inline b2Vec2 deCoordAb2Vec(std::pair<coordX, coordY> coord) {
    b2Vec2 representacionVec;
    representacionVec.x = coord.enX;
    representacionVec.y = coord.enY;

    return representacionVec;
}

enum class TipoEntidad { GUSANO = 1, VIGA = 2, ARMA = 4, PROYECTIL = 8, OCEANO = 16, PROYECTILREAL = 32, PROVISION = 64};

// se usa para poder saber que tipo de proyectil es
// TODO: agregar si es fragmento
struct ProyectilAsociado {
    b2Body *proyectil;
    ArmaProtocolo arma;
    time_t horaDeCreacion;
    //Esto se usa para chequear cuando se destruye algo
    double tiempoMinimoDeVida;
    //Guardo la posicion donde se creo el proyectil
    b2Vec2 posInicial;
};

enum TipoProyectil { Ningun, Colision, Countdown };

struct Proyectil {
    ArmaProtocolo armaOrigen;
    TipoProyectil tipo;
    // b2Vec2 posicion;
    int id;
    b2Body *cuerpo;
    bool exploto;
    //Estos se usa para armas como la bazooka para saber cuando
    //colisionaron
    bool enElAire;
    bool colisiono;
    b2Fixture* fixture;
    bool esFragmento;

    int countdown;
};

struct Entidad {
    TipoEntidad tipo;

    //Aca queriamos hacer un union pero, por algun std::pair no nos
    //deja. Gracias por nada C++ >:(
    Gusano *gusano;
    ProyectilAsociado proyectil;
    Proyectil *proyectilReal;
    Provision *provision;
};

#endif
