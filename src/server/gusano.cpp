#include "gusano.h"
#include <iostream>
#include "box2dDefs.h"

Gusano::Gusano(b2Body &cuerpo)
    : cuerpo(cuerpo)
      {
    this->direccion = DERECHA;
    this->vida = 100;
    this->armaEquipada = NADA_P;
}
void Gusano::giveId(int idGusano) {
    this->idGusano = idGusano;
}


b2Vec2 Gusano::getVectorDeDireccion(Direccion direccionDeseada) {
    b2Vec2 direccion;
    switch (direccionDeseada) {
    case INICIO_DER:
        direccion.x = VELOCIDADMOVIMIENTO;
        direccion.y = 0.0f;
        this->setDireccion(DERECHA);
        break;
    case INICIO_IZQ:
        direccion.x = -VELOCIDADMOVIMIENTO;
        direccion.y = 0.0f;
        this->setDireccion(IZQUIERDA);
        break;
    case FIN_DER:
        break;
    case FIN_IZQ:
        break;
    case SALTO:
        break;
    case PIRUETA:
        break;
    case INVAL_DIR:
        break;
    }
    return direccion;
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
    tipoAccion accionDeseada;
    accionDeseada = accion.accion;
    switch (accionDeseada) {
    case ESTAQUIETO:
        break;
    case MOVERSE:
        {
        // std::pair<coordX, coordY> coordsIniciales;
        // this->cuerpo.SetLinearVelocity;
        
        // b2Vec2 fuerzamovimiento(100.0f, 0.0f);
        b2Vec2 fuerzamovimiento;
        Direccion direccionDeseada;
        direccionDeseada = accion.dir;
        fuerzamovimiento = getVectorDeDireccion(direccionDeseada);

        this->cuerpo.SetLinearVelocity(std::ref(fuerzamovimiento));

        break;
        }
    case EQUIPARSE:
        ArmaProtocolo armaDeseada;
        armaDeseada = accion.armaAEquipar;
        this->armaEquipada = armaDeseada;
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
