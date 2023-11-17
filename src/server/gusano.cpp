#include "gusano.h"
#include <iostream>
#include "box2dDefs.h"

Gusano::Gusano() : armaSeleccionada(NADA_P) 
      {
    this->direccion = DERECHA;
    this->vida = 100;
    this->armaEquipada = NADA_P;
    this->estado = CAYENDO;
    this->armaSeleccionada.setAngulo(0); 
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
    //Esta funcion crea la hitbox donde que el gusano va a usar para
    //pegar. Es una caja con coordenadas inferior izquierda y sup derecha
    std::pair<coordX, coordY> coords;
    coords = this->getCoords();

    DireccionGusano dondeMira;
    dondeMira = this->direccion;

    coordX offset = 0;
    if (dondeMira == DERECHA)
        offset = 1;
    else
        offset = -1;
    coords.enX += offset;
    coords.enY -= offset;

    std::pair<inicioCaja, finCaja> vecs;
    b2Vec2 vectorCoordInicio = deCoordAb2Vec(coords);

    //reutilizo la variables coords para la segunda coordenada (sup der)
    //WARNING: ESTO ES UNA BANDA, ES SOLO PARA QUE ANDE
    coords.enX += offset * 5;
    coords.enY += offset * 5;

    b2Vec2 vectorCoordFin = deCoordAb2Vec(coords);
    //WARNING Valores hardcodeados hasta ver cual se ve mejor
    // vectorCoordFin.x += vectorCoordInicio.x + offset;
    // vectorCoordFin.y += vectorCoordInicio.y + 2 * offset;

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
        if (this->direccion == DERECHA)
	  direccion.x = 100000.0f;
        else
	  direccion.x = -100000.0f;
        direccion.y = 1000000.0f;
        this->cuerpo->ApplyForceToCenter(direccion, false);
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

void Gusano::recibirDano() {
    //TODO switch dependiendo del arma de this
    this->vida -= 20;
    this->cuerpo->ApplyLinearImpulseToCenter(b2Vec2(100.0f, 1000.0f), true);
}

void Gusano::preparar(Accion& accion) {
    Configuracion configDeseado;
    configDeseado = accion.configARealizar;
    switch (configDeseado.caracteristica) {
    case ANGULO:
        {
        float anguloActual = this->armaSeleccionada.getAngulo();
        anguloActual += configDeseado.angulo;
        std::cout << "Cambio: " << configDeseado.angulo << "\n";
        std::cout << "Angulo nuevo: " << anguloActual << "\n";
        this->armaSeleccionada.setAngulo(anguloActual);
        }
        break;
    case POTENCIA:
        break;
    case CUENTA_REGRESIVA:
        break;
    case COORDENADAS:
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
        {
        std::cout << "EQUIPO EL ARMA\n";
        ArmaProtocolo armaElegida;
        armaElegida = accion.armaAEquipar;

        this->armaEquipada = armaElegida;
        Arma armaSeleccionada(armaElegida);
        this->armaSeleccionada = armaSeleccionada;
        armaQueQuiero = NADA_P;
        break;
        }
    case PREPARAR:
        std::cout << "PREPARAR\n";
        this->preparar(accion);
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
    RepresentacionArma arma = this->armaSeleccionada.getRepresentacion();
    // std::cout << "Angulo a enviar: " << arma.anguloRad << "\n";
    arma.municiones = 100000;
    // arma.arma = this->armaEquipada;

    repre.armaEquipada = arma;

    return repre;
}
