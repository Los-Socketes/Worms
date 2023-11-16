#include "gusano.h"
#include <iostream>
#include "box2dDefs.h"

Gusano::Gusano(b2Body &cuerpo)
    : cuerpo(cuerpo)
      {
    this->direccion = DERECHA;
    this->vida = 100;
    this->armaEquipada = NADA_P;
    this->estado = CAYENDO;
}
void Gusano::giveId(int idGusano) {
    this->idGusano = idGusano;
}


void Gusano::realizarMovimiento(Direccion direccionDeseada) {
    b2Vec2 direccion;
    switch (direccionDeseada) {
    case INICIO_DER:
        std::cout << "Inicio der" << "\n";
        direccion.x = VELOCIDADMOVIMIENTO;
        direccion.y = 0.0f;
        this->estado = CAMINANDO;
        this->setDireccion(DERECHA);
        this->cuerpo.SetLinearVelocity(direccion);
        break;
    case INICIO_IZQ:
        std::cout << "Inicio izq" << "\n";
        direccion.x = -VELOCIDADMOVIMIENTO;
        direccion.y = 0.0f;
        this->estado = CAMINANDO;
        this->setDireccion(IZQUIERDA);
        this->cuerpo.SetLinearVelocity(direccion);
        break;
    case FIN_DER:
        std::cout << "Fin der" << "\n";
        direccion.x = 0.0f;
        direccion.y = 0.0f;
        this->estado = QUIETO;
        this->setDireccion(DERECHA);
        this->cuerpo.SetLinearVelocity(direccion);
        break;
    case FIN_IZQ:
        std::cout << "Fin izq" << "\n";
        direccion.x = 0.0f;
        direccion.y = 0.0f;
        this->estado = QUIETO;
        this->setDireccion(IZQUIERDA);
        this->cuerpo.SetLinearVelocity(direccion);
        break;
    case SALTO:
        std::cout << "Salto" << "\n";
        direccion.x = 0.0f;
        direccion.y = 0.0f;
        this->cuerpo.ApplyLinearImpulseToCenter(b2Vec2(0.0f, 105.0f), false);
        break;
    case PIRUETA:
        std::cout << "PIRUETA" << "\n";
        break;
    case INVAL_DIR:
        std::cout << "Invalid dir" << "\n";
        abort();
        break;
    }
}

ArmaDeseada Gusano::ejecutar(Accion accion) {
    ArmaDeseada armaQueQuiero;

    tipoAccion accionDeseada;
    accionDeseada = accion.accion;
    switch (accionDeseada) {
    case ESTAQUIETO:
        armaQueQuiero = NADA_P;
        break;
    case MOVERSE:
        {
        // std::pair<coordX, coordY> coordsIniciales;
        // this->cuerpo.SetLinearVelocity;
        
        // b2Vec2 fuerzamovimiento(100.0f, 0.0f);
        // b2Vec2 fuerzamovimiento;
        Direccion direccionDeseada;
        direccionDeseada = accion.dir;
        // fuerzamovimiento =
        // WARNING: Aca se ejecuta el movimiento
        realizarMovimiento(direccionDeseada);

        // this->cuerpo.SetLinearVelocity(std::ref(fuerzamovimiento));

        armaQueQuiero = NADA_P;
        break;
        }
    case EQUIPARSE:
        std::cout << "EQUIPO EL ARMA\n";
        ArmaProtocolo armaElegida;
        armaElegida = accion.armaAEquipar;

        this->armaEquipada = armaElegida;
        armaQueQuiero = NADA_P;
        break;
    case PREPARAR:
        armaQueQuiero = NADA_P;
        break;
    case ATAQUE:
        armaQueQuiero = this->armaEquipada;
        this->estado = DISPARANDO;
        std::cout << "ATACO\n";
        break;
    }


    return armaQueQuiero;
    

}


void Gusano::giveGun(ArmaProtocolo arma) {
    this->armaEquipada = arma;
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
    repre.estado = this->estado;
    repre.posicion = this->getCoords();
    repre.armaEquipada = this->armaEquipada;

    return repre;
}
