#include "gusano.h"
#include <iostream>
#include "box2dDefs.h"
#include <math.h>       /* sin */
#include <time.h>

#define TIEMPOCAMBIOTURNO 20

Gusano::Gusano() : armaSeleccionada(NADA_P) 
      {
    this->direccion = DERECHA;
    this->vida = 100;
    this->armaEquipada = NADA_P;
    this->estado = CAYENDO;
    this->armaSeleccionada.setAngulo(0); 
    this->tiempoQueMeQueda = TIEMPOCAMBIOTURNO;
    this->ultimaAccion.idGusano = INVAL_ID;
    // WARNING valor basura para que no rompa valgrind
    this->ultimaAccion.accion = ESTAQUIETO;
}

void Gusano::setCuerpo(b2Body* nuevoCuerpo) {
    this->cuerpo = nuevoCuerpo;
}


void Gusano::setEstado(EstadoGusano nuevoEstado) {
    //Si esta muerto, no cambia mas
    if (this->estado == MUERTO)
        return;


    this->estado = nuevoEstado;

    //Si se esta cayendo queremos cancelar todo tipo de velocidad y
    //que caiga directo. TODO: Esto lo hace "re duro". Podriamos hacer
    //que se conserve ALGO de la velocidad
    if (this->estado == CAYENDO) {
        b2Vec2 direccion;
        direccion.x = 0.0f;
        direccion.y = 0.0f;
        this->cuerpo->SetLinearVelocity(direccion);
    } else if (this->estado == MUERTO
	     ||
	     this->estado == AHOGADO) {
    this->turno.recibioDano = true;
        this->vida = 0;
        //Desabilito al cuerpo para que, una vez muerto, no colisione
        //ras
        // this->cuerpo->SetEnabled(false);
    }
}

void Gusano::giveId(int idGusano) {
    this->idGusano = idGusano;
}

void Gusano::esMiTurno(time_t arrancoAhora) {
    this->turno.cuandoArranco = arrancoAhora;
    this->turno.recibioDano = false;
    this->turno.usoSuArma = false;
}

bool Gusano::hayQueCambiarDeTurno(time_t tiempoActual) {
    bool cambiaDeTurno;

    //Esto, en teoria, da la rta en segundos
    bool noTengoMasTiempo;
    double cuantoTiempoLlevo;
    cuantoTiempoLlevo = difftime(tiempoActual, this->turno.cuandoArranco);
    noTengoMasTiempo = (cuantoTiempoLlevo > TIEMPOCAMBIOTURNO);

    //Hay que cambiar de turno con que se cumpla alguna de estas condiciones
    cambiaDeTurno = (noTengoMasTiempo ||
        this->turno.recibioDano == true
        // ||
        // this->turno.usoSuArma == true
		 );

    if (cambiaDeTurno == true) {
        std::cout << cuantoTiempoLlevo << "\n";
        std::cout << this->idGusano << "\n";
        std::cout << "CAMBIO DE TURNO!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
    }


    this->tiempoQueMeQueda = TIEMPOCAMBIOTURNO - cuantoTiempoLlevo;

    return cambiaDeTurno;
}

int Gusano::getTiempoQueMeQueda() {
    return this->tiempoQueMeQueda;
}

int Gusano::getId() {
    int idGusano;
    idGusano = this->idGusano;
    return idGusano;
}

Accion Gusano::getUltimaAccion() {
    return this->ultimaAccion;
}

std::pair<b2Vec2, std::pair<inicioCaja, finCaja>> Gusano::ejecutarGolpe() {
    //Esta funcion crea la hitbox donde que el gusano va a usar para
    //pegar. Es una caja con coordenadas inferior izquierda y sup derecha
    std::pair<
        b2Vec2,
        std::pair<inicioCaja, finCaja>
        > golpeYCaja;

    //Angulo
    //Usando trigonometria  vamos a obtener los vectores dado el angulo
    //WARNING: HARDCODEO La potencia
    /*
           /| 
          / |
         /  |
        /   | Opuesto (y)
       /    |
      /_____|
      Adyacente(x)
     */
    float potencia = 9;
    float hipotenusa = 9 * potencia;
    float angulo = this->armaSeleccionada.getAngulo();

    //SOHCAHTOA
    float adyacente;
    adyacente = cos(angulo) * hipotenusa;
    if (this->direccion == IZQUIERDA)
        adyacente *= -1;
    std::cout << adyacente << "\n";

    float opuesto;
    opuesto = sin(angulo) * hipotenusa;

    b2Vec2 golpeDeseado(adyacente, opuesto);
    golpeYCaja.first = golpeDeseado;



    //CAJA
    std::pair<coordX, coordY> coords;
    coords = this->getCoords();

    std::cout << "ANGULO" << this->armaSeleccionada.getAngulo() << "\n";

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
    coords.enX += offset * 1;
    coords.enY += offset * 1;

    b2Vec2 vectorCoordFin = deCoordAb2Vec(coords);
    //WARNING Valores hardcodeados hasta ver cual se ve mejor
    // vectorCoordFin.x += vectorCoordInicio.x + offset;
    // vectorCoordFin.y += vectorCoordInicio.y + 2 * offset;

    vecs.inicio = vectorCoordInicio;
    vecs.fin = vectorCoordFin;
    golpeYCaja.second = vecs;

    return golpeYCaja;
}



void Gusano::realizarMovimiento(Direccion direccionDeseada) {
    if (this->estado == CAYENDO)
        return;

    // b2Vec2 direccion;
    std::pair<coordX, coordY> direccion;
    switch (direccionDeseada) {
    case INICIO_DER:
        std::cout << "Inicio der" << "\n";
        direccion.enX = VELOCIDADMOVIMIENTO;
        direccion.enY = 0.0f;
        this->setEstado(CAMINANDO);
        this->setDireccion(DERECHA);
        this->cuerpo->SetLinearVelocity(deCoordAb2Vec(direccion));
        break;
    case INICIO_IZQ:
        std::cout << "Inicio izq" << "\n";
        direccion.enX = -VELOCIDADMOVIMIENTO;
        direccion.enY = 0.0f;
        this->setEstado(CAMINANDO);
        this->setDireccion(IZQUIERDA);
        this->cuerpo->SetLinearVelocity(deCoordAb2Vec(direccion));
        break;
    case FIN_DER:
        std::cout << "Fin der" << "\n";
        direccion.enX = 0.0f;
        direccion.enY = 0.0f;
        this->setEstado(QUIETO);
        this->setDireccion(DERECHA);
        this->cuerpo->SetLinearVelocity(deCoordAb2Vec(direccion));
        break;
    case FIN_IZQ:
        std::cout << "Fin izq" << "\n";
        direccion.enX = 0.0f;
        direccion.enY = 0.0f;
        this->setEstado(QUIETO);
        this->setDireccion(IZQUIERDA);
        this->cuerpo->SetLinearVelocity(deCoordAb2Vec(direccion));
        break;
    case SALTO:
        std::cout << "Salto" << "\n";
        if (this->direccion == DERECHA)
	  direccion.enX = 100000.0f;
        else
	  direccion.enX = -100000.0f;
        direccion.enY = 1000000.0f;
        this->cuerpo->ApplyLinearImpulseToCenter(deCoordAb2Vec(direccion), true);
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

void Gusano::recibirDano(b2Vec2 golpe, ArmaProtocolo tipoArma) {
    //TODO cambiar a que no tenga que crear el arma para obtener el danio
    Arma armaUsada(tipoArma);
    u_int danio = armaUsada.getDanio().epicentro;
    // this->vida -= 20;
    if (this->vida < danio) {
        this->vida = 0;
        this->setEstado(MUERTO);

        //Desabilito al cuerpo para que, una vez muerto, no colisione
        //ras
        //this->cuerpo->SetEnabled(false);
    } else {
        this->vida -= danio;
    }
    this->turno.recibioDano = true;
    this->cuerpo->ApplyLinearImpulseToCenter(golpe, true);
    std::cout << "Vida nueva: " << this->vida << "\n";
    // this->cuerpo->ApplyLinearImpulseToCenter(golpe, true);
    // this->cuerpo->ApplyLinearImpulseToCenter(golpe, true);
    // this->cuerpo->ApplyLinearImpulseToCenter(golpe, true);
}

void Gusano::preparar(Accion& accion) {
    Configuracion configDeseado;
    configDeseado = accion.configARealizar;
    switch (configDeseado.caracteristica) {
    case ANGULO:
        {
        float anguloActual = this->armaSeleccionada.getAngulo();
        anguloActual += configDeseado.angulo;
        if (!this->armaSeleccionada.getCaracteristicas().tieneMira || anguloActual > M_PI/2 || anguloActual < -M_PI/2) {
            break;
        } 
        std::cout << "Cambio: " << configDeseado.angulo << "\n";
        std::cout << "Angulo nuevo: " << anguloActual << "\n";
        this->armaSeleccionada.setAngulo(anguloActual);
        break;
        }
    case POTENCIA:
        {
        float potenciaActual = this->armaSeleccionada.getPotencia();
        potenciaActual += configDeseado.potencia;
        if (configDeseado.potencia == 0) {
            potenciaActual = 0;
        }else if (!this->armaSeleccionada.getCaracteristicas().tienePotenciaVariable || potenciaActual > 100) {
            potenciaActual = 100;
        } 
        std::cout << "Cambio: " << configDeseado.potencia << "\n";
        std::cout << "Potencia nueva: " << potenciaActual << "\n";
        this->armaSeleccionada.setPotencia(potenciaActual);
        break;
        }
    case CUENTA_REGRESIVA:
        if (!this->armaSeleccionada.getCaracteristicas().tieneCuentaRegresiva) {
            break;
        }
        this->armaSeleccionada.setCuentaRegresiva(configDeseado.cuentaRegresiva);
        break;
    case COORDENADAS:
        this->armaSeleccionada.setCoordenadasTeletransporte(configDeseado.coordenadas);
        break;
    }
}

Ataque Gusano::ejecutar(Accion accion) {
    Ataque ataqueARealizar;
    b2Vec2 posicion; //Posicion donde se va a realizar el ataque
    int tiempoEspera; //Delay
    ArmaDeseada armaQueQuiero;

    tipoAccion accionDeseada;
    accionDeseada = accion.accion;
    switch (accionDeseada) {
    case ESTAQUIETO:
        armaQueQuiero = NADA_P;
        tiempoEspera = 0;
        posicion = (deCoordAb2Vec(this->getCoords()));

        ataqueARealizar.posicion = posicion;
        ataqueARealizar.tiempoEspera = tiempoEspera;
        ataqueARealizar.arma = armaQueQuiero;
        break;
    case MOVERSE:
        {
        Direccion direccionDeseada;
        direccionDeseada = accion.dir;
        // WARNING: Aca se ejecuta el movimiento
        realizarMovimiento(direccionDeseada);

        armaQueQuiero = NADA_P;
        tiempoEspera = 0;
        posicion = (deCoordAb2Vec(this->getCoords()));

        ataqueARealizar.posicion = posicion;
        ataqueARealizar.tiempoEspera = tiempoEspera;
        ataqueARealizar.arma = armaQueQuiero;
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
        tiempoEspera = 0;
        posicion = (deCoordAb2Vec(this->getCoords()));

        ataqueARealizar.posicion = posicion;
        ataqueARealizar.tiempoEspera = tiempoEspera;
        ataqueARealizar.arma = armaQueQuiero;

        break;
        }
    case PREPARAR:
        std::cout << "PREPARAR\n";
        this->preparar(accion);
        armaQueQuiero = NADA_P;
        tiempoEspera = 0;
        posicion = (deCoordAb2Vec(this->getCoords()));

        ataqueARealizar.arma = NADA_P;
        ataqueARealizar.posicion = posicion;
        ataqueARealizar.tiempoEspera = tiempoEspera;
        ataqueARealizar.arma = armaQueQuiero;
        break;
    case ATAQUE:
        tiempoEspera = 0;
        if (armaEquipada == TELETRANSPORTACION_P) {
	        this->teletransportarse();
        }  

        armaQueQuiero = this->armaEquipada;
        posicion = (deCoordAb2Vec(this->getCoords()));

        ataqueARealizar.posicion = posicion;
        // tiempoEspera = 99;
        if(armaEquipada == DINAMITA_P) {
	        tiempoEspera = this->armaSeleccionada.getCuentaRegresiva() * 30;
        } else {
	        tiempoEspera = 0;
        }

        ataqueARealizar.tiempoEspera = tiempoEspera;
        ataqueARealizar.arma = armaQueQuiero;
        this->estado = DISPARANDO;
        this->turno.usoSuArma = true;
        std::cout << "ATACO\n";
        break;
    case INVAL_ACCION:
        std::cout << "INVALID ACCION\n";
        break;
    }


    //Hago que el gusano se acuerde que fue lo ulitmo que realizo;
    this->ultimaAccion = accion;

    return ataqueARealizar;
    

}

void Gusano::teletransportarse() {
    std::pair<coordX, coordY> destino = this->armaSeleccionada.getCoordenadasTeletransporte();

    std::cout << destino.first << " " << destino.second << "\n";
    b2Vec2 vectorDestino = deCoordAb2Vec(destino);


    // std::pair<coordX, coordY> origen = this->getCoords();
    // b2Vec2 inicio = deCoordAb2Vec(origen);

    // b2Vec2 quiero = vectorDestino - inicio;
    // b2Vec2 quiero(5.0f,53.0f);


    this->cuerpo->SetTransform(vectorDestino, true);
}

void Gusano::giveGun(ArmaProtocolo arma) {
    this->armaEquipada = arma;
    Arma armaNueva(arma);
    this->armaSeleccionada = armaNueva;
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

EstadoGusano Gusano::getEstado() {
    return this->estado;
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
