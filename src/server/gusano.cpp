#include "gusano.h"
#include <iostream>
#include "box2dDefs.h"

Gusano::Gusano() : armaSeleccionada(NADA_P) 
      {
    this->direccion = DERECHA;
    this->vida = 100;
    this->armaEquipada = NADA_P;
    this->estado = CAYENDO;
}

void Gusano::setCuerpo(b2Body* nuevoCuerpo) {
    this->cuerpo = nuevoCuerpo;
}

void Gusano::setEstado(EstadoGusano nuevoEstado) {
    this->estado = nuevoEstado;

    //Si se esta cayendo queremos cancelar todo tipo de velocidad y
    //que caiga directo. TODO: Esto lo hace "re duro". Podriamos hacer
    //que se conserve ALGO de la velocidad
    if (this->estado == CAYENDO) {
        b2Vec2 direccion;
        direccion.x = 0.0f;
        direccion.y = 0.0f;
        this->cuerpo->SetLinearVelocity(direccion);
    }
}

void Gusano::giveId(int idGusano) {
    this->idGusano = idGusano;
}

std::pair<inicioCaja, finCaja> Gusano::getAreaGolpe() {
    std::pair<coordX, coordY> coords;
    coords = this->getCoords();

    DireccionGusano dondeMira;
    dondeMira = this->direccion;

    coordX offset = 0;
    if (dondeMira == DERECHA)
        offset = TAMANOGUSANO;
    else
        offset = -TAMANOGUSANO;
    coords.enX += offset;

    std::pair<inicioCaja, finCaja> vecs;
    b2Vec2 vectorCoordInicio = deCoordAb2Vec(coords);
    b2Vec2 vectorCoordFin;
    vectorCoordFin.x += vectorCoordInicio.x + offset;
    vectorCoordFin.y += vectorCoordInicio.y + offset;

    vecs.inicio = vectorCoordInicio;
    vecs.fin = vectorCoordFin;

    return vecs;
}



void Gusano::realizarMovimiento(Direccion direccionDeseada) {
    if (this->estado == CAYENDO)
        return;

    b2Vec2 direccion;
    switch (direccionDeseada) {
    case INICIO_DER:
        std::cout << "Inicio der" << "\n";
        direccion.x = VELOCIDADMOVIMIENTO;
        direccion.y = 0.0f;
        this->setEstado(CAMINANDO);
        this->setDireccion(DERECHA);
        this->cuerpo->SetLinearVelocity(direccion);
        break;
    case INICIO_IZQ:
        std::cout << "Inicio izq" << "\n";
        direccion.x = -VELOCIDADMOVIMIENTO;
        direccion.y = 0.0f;
        this->setEstado(CAMINANDO);
        this->setDireccion(IZQUIERDA);
        this->cuerpo->SetLinearVelocity(direccion);
        break;
    case FIN_DER:
        std::cout << "Fin der" << "\n";
        direccion.x = 0.0f;
        direccion.y = 0.0f;
        this->setEstado(QUIETO);
        this->setDireccion(DERECHA);
        this->cuerpo->SetLinearVelocity(direccion);
        break;
    case FIN_IZQ:
        std::cout << "Fin izq" << "\n";
        direccion.x = 0.0f;
        direccion.y = 0.0f;
        this->setEstado(QUIETO);
        this->setDireccion(IZQUIERDA);
        this->cuerpo->SetLinearVelocity(direccion);
        break;
    case SALTO:
        std::cout << "Salto" << "\n";
        direccion.x = 0.0f;
        direccion.y = 0.0f;
        this->cuerpo->ApplyLinearImpulseToCenter(b2Vec2(0.0f, 50.0f), false);
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
    b2Vec2 position = this->cuerpo->GetPosition();
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
    //TODO Ahora est hardcodeado. Hacer algo generico.
    //Esto solo aplica al bate
    RepresentacionArma arma;
    if (this->armaEquipada == NADA_P) {
        arma = this->armaSeleccionada.getRepresentacion();
    } else {
        arma.tieneMira = false;
        arma.tienePotenciaVariable = false;
        arma.tieneCuentaRegresiva = false;
        arma.municiones = 100000;
        arma.fragmentos = 0;
        RepresentacionDanio danio;
        danio.epicentro = 9;
        danio.radio = 0;
        arma.danio = danio;
        arma.danioFragmento = danio;
        arma.anguloRad = 1;
        arma.potencia = 0;
        arma.cuentaRegresiva = 0;
        arma.arma = BATE_P;
    }

    repre.armaEquipada = arma;

    return repre;
}
