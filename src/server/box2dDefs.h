#ifndef BOX2DDEFS_HEADER
#define BOX2DDEFS_HEADER

#include <box2d/box2d.h>
#include "defs.h"

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

#define TAMANOGUSANO 20

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

#define TIEMPOESPERADINAMITA 99

#endif
