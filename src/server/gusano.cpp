#include "gusano.h"
#include "defs.h"
#include "protocolo.h"

Gusano::Gusano(std::pair<coordX, coordY> coords) {
    this->coords = coords;
    this->direccion = DERECHA;
    this->vida = 100;
    /*
      arma en mano
      equipo
    */
}

// void Gusano::detener(){
//     this->moviendose = false;
// }
// void Gusano::ponerEnMovimiento(){
//     this->moviendose = true;
// }


std::pair<cambioX, cambioY> Gusano::cambio(Accion accion) {
    std::pair<cambioX, cambioY> cambio(0.0f,0.0f);
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
        cambio.first = -1.0f;
        cambio.second = 0.0f;
        break;
    case Accion::MOV_DER:
        this->setDireccion(IZQUIERDA);
        cambio.first = 1.0f;
        cambio.second = 0.0f;
        break;
        //TODO Esto no se si es 100% correcto. Por ahora funca
    case Accion::MOV_SALTO:
        cambio.first = 1.0f;
        cambio.second = 1.0f;
        break;
    case Accion::MOV_PIRUETA:
        cambio.first = -1.0f;
        cambio.second = 1.0f;
        break;
    case Accion::MOV_QUIETO:
        cambio.first = 0.0f;
        cambio.second = 0.0f;
        break;
    }

    return cambio;
    

}

std::pair<coordX, coordY> Gusano::getCoords() {
    return this->coords;
}
void Gusano::setCoords(std::pair<coordX, coordY> nuevasCoords) {
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
