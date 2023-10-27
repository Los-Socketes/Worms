#include "gusano.h"
#include "protocolo.h"

typedef int cambioX;
typedef int cambioY;

Gusano::Gusano(std::pair<int, int> coords) {
    this->coords = coords;
    this->vida = 100;
}

std::pair<int, int> Gusano::cambio(Direccion accion) {
    std::pair<cambioX, cambioY> cambio(0,0);
    /*Arranca abajo a la izquierda.
     *X:
     *	>: +1
     *	<: -1
     *Y:
     *	^: +1
     *	v: -1
    */
    switch (accion) {
        case IZQUIERDA:
	  cambio.first = -1;
	  cambio.second = 0;
	  break;
        case DERECHA:
	  cambio.first = 1;
	  cambio.second = 0;
	  break;
        //TODO Esto no se si es 100% correcto. Por ahora funca
        case SALTO:
	  cambio.first = 1;
	  cambio.second = 1;
	  break;
        case PIRUETA:
	  cambio.first = -1;
	  cambio.second = 1;
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
