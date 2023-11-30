#ifndef BOX2DDEFS_HEADER
#define BOX2DDEFS_HEADER

#include <box2d/box2d.h>
#include "defs.h"

//Forward declaration
class Gusano;

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

// Constantes de gravedad.
// WARNING: Originalmente valia 10 no 100. Ver que onda.
#define FUERZAGRAVITARIAX 0.0f
#define FUERZAGRAVITARIAY -20.0f

#define METROS_SIN_DANIO 5.0f

#define DEGTORAD 0.0174532925199432957f

#define ANCHOVIGA 0.8f
// es entero para que no rompa
// Juampi: Lo vuelvo a poner en 6, 14 me parece mucho. Se van a
// superponer las vigas.
// #define LONGITUDVIGAGRANDE 14.0f
#define LONGITUDVIGAGRANDE 6.0f
#define LONGITUDVIGACHICA 3.0f

#define VELOCIDADMOVIMIENTO 5.0f

#define MASACUERPOESTATICO 0.0f

#define DISTANCIAENTREVIGAS 7    

#define TAMANOGUSANO 20
#define CANTIDADVIGAS 3

#define NOT !

typedef b2Vec2 inicioCaja;
typedef b2Vec2 finCaja;

#define inicio first
#define fin second

#define POTENCIASALTO 5.0f

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

//Gracias algebra 2
[[nodiscard]] inline int distanciaEntreVectores(b2Vec2 a, b2Vec2 b) {
    /* √((x_2-x_1)²+(y_2-y_1)²) */
    int distEnX;
    distEnX = a.x - b.x;

    int distEnY;
    distEnY = a.y - b.y;

    int cuadradoX;
    cuadradoX = distEnX * distEnX;

    int cuadradoY;
    cuadradoY = distEnY * distEnY;

    int distancia;
    distancia = sqrt(cuadradoX + cuadradoY);

    return distancia;
}

enum class TipoEntidad { GUSANO = 1, VIGA = 2, ARMA = 4, PROYECTIL = 8, OCEANO = 16, PROYECTILREAL = 32};

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

    int countdown;
};

struct Entidad {
    TipoEntidad tipo;
    // union {
        Gusano *gusano;
        // Viga *viga;
        // Arma *arma;
        ProyectilAsociado proyectil;
    Proyectil *proyectilReal;
    // };
};

#define TIEMPOESPERADINAMITA 99

#endif
