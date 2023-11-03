#include "gusano.h"
#include "defs.h"
#include "protocolo.h"

typedef int cambioX;
typedef int cambioY;

Gusano::Gusano(std::pair<int, int> coords) {
    this->coords = coords;
    this->direccion = DERECHA;
    this->vida = 100;
}

// void Gusano::detener(){
//     this->moviendose = false;
// }
// void Gusano::ponerEnMovimiento(){
//     this->moviendose = true;
// }


std::pair<int, int> Gusano::cambio(Accion accion) {
    std::pair<cambioX, cambioY> cambio(0,0);
    /*Arranca abajo a la izquierda.
     *X:
     *	>: +1
     *	<: -1
     *Y:
     *	^: +1
     *	v: -1
    */

//INICIO_IZQ, FIN_IZQ, INICIO_DER, FIN_DER, SALTO, PIRUETA, INVAL_DIR
    switch (accion) {
    case Accion::MOV_IZQ:
        this->setDireccion(DERECHA);
        cambio.first = -1;
        cambio.second = 0;
        break;
    case Accion::MOV_DER:
        this->setDireccion(IZQUIERDA);
        cambio.first = 1;
        cambio.second = 0;
        break;
        //TODO Esto no se si es 100% correcto. Por ahora funca
    case Accion::MOV_SALTO:
        cambio.first = 1;
        cambio.second = 1;
        break;
    case Accion::MOV_PIRUETA:
        cambio.first = -1;
        cambio.second = 1;
        break;
    case Accion::MOV_QUIETO:
        cambio.first = 0;
        cambio.second = 0;
        break;
    }

    return cambio;
    

}

std::pair<int, int> Gusano::getCoords() {
    return this->coords;
}
void Gusano::setCoords(std::pair<int, int> nuevasCoords) {
    this->coords = nuevasCoords;
}

void Gusano::setDireccion(DireccionGusano nuevaDireccion) {
    this->direccion = nuevaDireccion;
}

DireccionGusano Gusano::getDireccion() {
    DireccionGusano direccionActual;
    direccionActual = this->direccion;

    return direccionActual;
}
