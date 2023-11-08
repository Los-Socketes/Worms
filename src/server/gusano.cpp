#include "gusano.h"
#include <iostream>

Gusano::Gusano(b2Body &cuerpo)
    : cuerpo(cuerpo)
      {
    this->direccion = DERECHA;
    this->vida = 100;
}
void Gusano::giveId(int idGusano) {
    this->idGusano = idGusano;
}

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
    //WARNING Pongo este std::out solamente para que compile
    std::cout << accion.idGusano;
    tipoAccion accionDeseada;
    accionDeseada = accion.accion;
    switch (accionDeseada) {
    case ESTAQUIETO:
        break;
    case MOVERSE:
        {
        std::pair<coordX, coordY> coordsIniciales;
        
        b2Vec2 fuerzamovimiento(500.0f, 0.0f); 

        break;
        }
    case EQUIPARSE:
        break;
    case PREPARAR:
        break;
    case ATAQUE:
        break;
    }

    b2Vec2 fuerza; 

//INICIO_IZQ, FIN_IZQ, INICIO_DER, FIN_DER, SALTO, PIRUETA, INVAL_DIR
    // switch (accion) {
    // case AccionServer::MOV_IZQ:
    //     this->setDireccion(DERECHA);
    //     cambio.first = -1.0f;
    //     cambio.second = 0.0f;
    //     break;
    // case AccionServer::MOV_DER:
    //     this->setDireccion(IZQUIERDA);
    //     cambio.first = 1.0f;
    //     cambio.second = 0.0f;
    //     break;
    //     //TODO Esto no se si es 100% correcto. Por ahora funca
    // case AccionServer::MOV_SALTO:
    //     cambio.first = 1.0f;
    //     cambio.second = 1.0f;
    //     break;
    // case AccionServer::MOV_PIRUETA:
    //     cambio.first = -1.0f;
    //     cambio.second = 1.0f;
    //     break;
    // case AccionServer::MOV_QUIETO:
    //     cambio.first = 0.0f;
    //     cambio.second = 0.0f;
    //     break;
    // }

    return cambio;
    

}

std::pair<coordX, coordY> Gusano::getCoords() {
    b2Vec2 position = this->cuerpo.GetPosition();
    std::pair<coordX, coordY> representacionPair;
    representacionPair.enX = position.x;
    representacionPair.enY = position.y;

    return representacionPair;
}

void Gusano::setDireccion(DireccionGusano nuevaDireccion) {
    this->direccion = nuevaDireccion;
}


RepresentacionGusano Gusano::getRepresentacion() {
    RepresentacionGusano repre;
    repre.idGusano = this->idGusano;
    repre.vida = this->vida;
    repre.dir = this->direccion;
    repre.posicion = this->getCoords();;

    return repre;
}
