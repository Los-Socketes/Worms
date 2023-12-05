#include "gusano.h"
#include <iostream>
#include "box2dDefs.h"
#include <math.h>       /* sin */
#include <time.h>

Gusano::Gusano(Mapa& mapa):
    mapa(mapa){
    this->direccion = DERECHA;
    this->vida = 100;
    this->armaEquipada = NADA_P;
    this->estado = CAYENDO;
    this->tiempoQueMeQueda = TIEMPOCAMBIOTURNO;
    this->ultimaAccion.idGusano = INVAL_ID;
    // WARNING valor basura para que no rompa valgrind
    this->ultimaAccion.accion = ESTAQUIETO;
    this->golpeado = false;

    // forma que encontre de iterar un enum
    for (int i = 0; i < INVAL_ARMA_P; i++) {
        Arma armaActual((ArmaProtocolo)i);
        this->armas.push_back(armaActual);
    } 

    this->armaSeleccionada = &this->armas[NADA_P];
    this->armaSeleccionada->setAngulo(0); 
}

void Gusano::setCuerpo(b2Body* nuevoCuerpo) {
    this->cuerpo = nuevoCuerpo;
}


void Gusano::setEstado(EstadoGusano nuevoEstado) {
    //Si esta muerto, no cambia mas
    if (this->estado == MUERTO ||
        this->estado == AHOGADO)
        return;


    this->estado = nuevoEstado;

    //Si se esta cayendo queremos cancelar todo tipo de velocidad y
    //que caiga directo. TODO: Esto lo hace "re duro". Podriamos hacer
    //que se conserve ALGO de la velocidad
    if (this->estado == CAYENDO || this->estado == QUIETO) {
        b2Vec2 direccion;
        direccion.x = 0.0f;
        direccion.y = 0.0f;
        this->cuerpo->SetLinearVelocity(direccion);
    } else if (this->estado == MUERTO ||
	     this->estado == AHOGADO) {
        this->turno.recibioDano = true;
        this->vida = 0;
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
    double cuantoTiempoLlevo = difftime(tiempoActual, this->turno.cuandoArranco);
    if (this->turno.usoSuArma && cuantoTiempoLlevo < TIEMPOCAMBIOTURNO - TIEMPOCHANGUI) {
        this->setEstado(QUIETO);
        this->turno.cuandoArranco -= (TIEMPOCAMBIOTURNO - cuantoTiempoLlevo - 3);
    }

    bool cambiaDeTurno;

    //Esto, en teoria, da la rta en segundos
    bool noTengoMasTiempo;
    cuantoTiempoLlevo = difftime(tiempoActual, this->turno.cuandoArranco);
    noTengoMasTiempo = (cuantoTiempoLlevo >= TIEMPOCAMBIOTURNO);

    //Hay que cambiar de turno con que se cumpla alguna de estas condiciones
    cambiaDeTurno = (noTengoMasTiempo ||
        this->turno.recibioDano == true);

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
    float potencia = this->armaSeleccionada->getDanio().epicentro;
    float hipotenusa = 0.9*potencia;
    float angulo = this->armaSeleccionada->getAngulo();

    //SOHCAHTOA
    float adyacente;
    adyacente = cos(angulo) * hipotenusa;
    if (this->direccion == IZQUIERDA)
        adyacente *= -1;

    float opuesto;
    opuesto = sin(angulo) * hipotenusa*2;
    if (opuesto < 1) {
        opuesto = 1;
    }

    b2Vec2 golpeDeseado(adyacente, opuesto);
    golpeYCaja.first = golpeDeseado;

    //CAJA
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

    std::pair<inicioCaja, finCaja> vecs;
    b2Vec2 vectorCoordInicio = deCoordAb2Vec(coords);


    b2Vec2 vectorCoordFin = deCoordAb2Vec(coords);

    vecs.inicio = vectorCoordInicio;
    vecs.fin = vectorCoordFin;
    golpeYCaja.second = vecs;

    return golpeYCaja;
}



void Gusano::realizarMovimiento(Direccion direccionDeseada) {
    if (this->estado == CAYENDO ||
        this->estado == SALTANDO ||
        this->estado == HACE_PIRUETA)
        return;

    // b2Vec2 direccion;
    std::pair<coordX, coordY> direccion;
    switch (direccionDeseada) {
    case INICIO_DER:
        direccion.enX = config.velocidadMovimiento;
        direccion.enY = 0.0f;
        this->setEstado(CAMINANDO);
        this->setDireccion(DERECHA);
        this->cuerpo->SetLinearVelocity(deCoordAb2Vec(direccion));
        break;
    case INICIO_IZQ:
        direccion.enX = -config.velocidadMovimiento;
        direccion.enY = 0.0f;
        this->setEstado(CAMINANDO);
        this->setDireccion(IZQUIERDA);
        this->cuerpo->SetLinearVelocity(deCoordAb2Vec(direccion));
        break;
    case FIN_DER:
        direccion.enX = 0.0f;
        direccion.enY = 0.0f;
        this->setEstado(QUIETO);
        this->setDireccion(DERECHA);
        this->cuerpo->SetLinearVelocity(deCoordAb2Vec(direccion));
        break;
    case FIN_IZQ:
        direccion.enX = 0.0f;
        direccion.enY = 0.0f;
        this->setEstado(QUIETO);
        this->setDireccion(IZQUIERDA);
        this->cuerpo->SetLinearVelocity(deCoordAb2Vec(direccion));
        break;
    case SALTO:
        {
        if (this->estado == SALTANDO) {
            break;
        }
        direccion.enX = (this->direccion == DERECHA) ? config.potenciaSalto : -config.potenciaSalto;
        direccion.enY = config.potenciaSalto*3;
        b2Vec2 saltoHorizontal = deCoordAb2Vec(direccion);
        this->cuerpo->ApplyLinearImpulseToCenter(saltoHorizontal,true);
        this->setEstado(SALTANDO);
        break;
        }
    case PIRUETA:
        {
        if (this->estado == HACE_PIRUETA) {
            break;
        }
        direccion.enX = (this->direccion == DERECHA) ? -config.potenciaSalto : config.potenciaSalto;
        direccion.enY = config.potenciaSalto*3.5;
        b2Vec2 saltoHorizontal = deCoordAb2Vec(direccion);
        this->cuerpo->ApplyLinearImpulseToCenter(saltoHorizontal,true);
        this->setEstado(HACE_PIRUETA);
        break;
        }
    case INVAL_DIR:
        abort();
        break;
    }
}

void Gusano::recibirDano(b2Vec2 golpe, Entidad *entidad) {
    if (this->golpeado == true)
        return;
    this->golpeado = true;
    b2Vec2 velocidadActual = this->cuerpo->GetLinearVelocity();
    ArmaProtocolo tipoArma;
    tipoArma = entidad->proyectil.arma;
    if (this->estado == MUERTO || false) {
        // ((tipoArma != GRANADA_VERDE_P) // && (velocidadActual.x != 0 && velocidadActual.y != 0)
        //  )) {
        return;
    }

    std::cout << "RECIBO DANIO\n";

    float distanciaGusanoBomba = b2Distance(entidad->proyectil.posInicial, this->cuerpo->GetPosition());

    //TODO cambiar a que no tenga que crear el arma para obtener el danio
    Arma armaUsada(tipoArma);
    u_int danio;
    int radio;

    if (entidad->proyectilReal->esFragmento) {
        danio = armaUsada.getDanioFragmento().epicentro;
        radio = armaUsada.getDanioFragmento().radio;
    } else {
        danio = armaUsada.getDanio().epicentro;
        radio = armaUsada.getDanio().radio;
    }

    if (tipoArma == BATE_P) {
        radio = 2;
    }

    float danioReal;

    float porcentaje = 1.0f / (1.0f + std::pow(distanciaGusanoBomba / radio,2));

    // // Calculate the graduated damage
    danioReal = (distanciaGusanoBomba > radio) ? 0 : danio * porcentaje;
    if (tipoArma == BATE_P && distanciaGusanoBomba <= radio) {
        radio = 2;
        danioReal = danio;
    }

    if ((u_int)danioReal == 0) {
        return;
    }
    if (this->vida < (u_int)danioReal) {
        this->vida = 0;
        this->setEstado(MUERTO);

    } else {
        this->vida -= danioReal;
    }
    this->turno.recibioDano = true;

    if (tipoArma == BATE_P) {
        this->cuerpo->ApplyLinearImpulseToCenter(golpe, true);
    } else {
        if (golpe.y > 0 && golpe.y < 10) {
            golpe.y = 10;
        }
        golpe.x *= porcentaje;
        golpe.y *= porcentaje;
        float fuerzaX = golpe.x > 0 ? golpe.x : -golpe.x;
        float fuerzaY = golpe.y > 0 ? golpe.y : -golpe.y;
        float angulo = std::atan(fuerzaY/fuerzaX);
        float hipotenusa = 5;
        float adyacente = cos(angulo) * hipotenusa;
        if (golpe.x < 0)
            adyacente *= -1;
        float opuesto;
        opuesto = sin(angulo) * hipotenusa*2;
        if (golpe.y < 0) {
            opuesto *= -1;
        }


        golpe.x = adyacente;
        golpe.y = opuesto;
        this->cuerpo->ApplyLinearImpulseToCenter(golpe, true);
    }
    this->setEstado(HERIDO);
}

void Gusano::preparar(Accion& accion) {
    Configuracion configDeseado;
    configDeseado = accion.configARealizar;
    switch (configDeseado.caracteristica) {
    case ANGULO:
        {
        float anguloActual = this->armaSeleccionada->getAngulo();
        anguloActual += configDeseado.angulo;
        if (!this->armaSeleccionada->getCaracteristicas().tieneMira || anguloActual > M_PI/2 || anguloActual < -M_PI/2) {
            break;
        } 
        this->armaSeleccionada->setAngulo(anguloActual);
        break;
        }
    case POTENCIA:
        {
        float potenciaActual = this->armaSeleccionada->getPotencia();
        potenciaActual += configDeseado.potencia;
        if (configDeseado.potencia == 0) {
            potenciaActual = 0;
        }else if (!this->armaSeleccionada->getCaracteristicas().tienePotenciaVariable || potenciaActual > 100) {
            potenciaActual = 100;
        } 
        this->armaSeleccionada->setPotencia(potenciaActual);
        break;
        }
    case CUENTA_REGRESIVA:
        if (!this->armaSeleccionada->getCaracteristicas().tieneCuentaRegresiva) {
            break;
        }
        this->armaSeleccionada->setCuentaRegresiva(configDeseado.cuentaRegresiva);
        break;
    case COORDENADAS:
        this->armaSeleccionada->setCoordenadasTeletransporte(configDeseado.coordenadas);
        break;
    }
}



Ataque Gusano::ejecutar(Accion accion) {
    Ataque ataqueARealizar;
    if (this->getTiempoQueMeQueda() <= 0) {
        if (this->estado != CAYENDO && this->estado != SALTANDO
        && this->estado != HACE_PIRUETA) 
            this->setEstado(QUIETO);
        return ataqueARealizar;
    }

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
        // this->estado = QUIETO;
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
        ArmaProtocolo armaElegida;
        armaElegida = accion.armaAEquipar;

        this->armaEquipada = armaElegida;
        this->armaSeleccionada = &this->armas[armaElegida];
        armaQueQuiero = NADA_P;
        tiempoEspera = 0;
        posicion = (deCoordAb2Vec(this->getCoords()));

        ataqueARealizar.posicion = posicion;
        ataqueARealizar.tiempoEspera = tiempoEspera;
        ataqueARealizar.arma = armaQueQuiero;

        break;
        }
    case PREPARAR:
        this->preparar(accion);

        armaQueQuiero = NADA_P;

        tiempoEspera = 0;
        posicion = (deCoordAb2Vec(this->getCoords()));

        ataqueARealizar.arma = NADA_P;
        ataqueARealizar.posicion = posicion;
        ataqueARealizar.tiempoEspera = tiempoEspera;
        ataqueARealizar.arma = armaQueQuiero;
        ataqueARealizar.potencia = this->armaSeleccionada->getPotencia();
        break;
    case ATAQUE:
        {
        b2Vec2 impulso(0,0);
        tiempoEspera = 0;
        armaQueQuiero = this->armaEquipada;
        posicion = (deCoordAb2Vec(this->getCoords()));

        // si no tiene municiones -> no ataco
        int municionesDisponibles = this->armaSeleccionada->getMuniciones();
        if (municionesDisponibles == 0) {
            ataqueARealizar.arma = NADA_P;
            ataqueARealizar.posicion = posicion;
            ataqueARealizar.tiempoEspera = tiempoEspera;
            ataqueARealizar.arma = armaQueQuiero;
            break;
        }

        // tengo munciones disponibles
        if (armaEquipada == TELETRANSPORTACION_P) {
	        this->teletransportarse();
        }

        if (armaEquipada == ATAQUE_AEREO_P) {
	  std::pair<coordX, coordY> ataquePos;
	  ataquePos = this->armaSeleccionada->getCoordenadasTeletransporte();
	  ataquePos.enY = this->mapa.dimensiones.enY - 20;
	  posicion = deCoordAb2Vec(ataquePos);
	  ataqueARealizar.impulsoInicial = b2Vec2(config.gravedad.enX, config.gravedad.enY);
        }

        ataqueARealizar.posicion = posicion;

        if(armaEquipada == DINAMITA_P ||
	 armaEquipada == GRANADA_VERDE_P ||
	 armaEquipada == GRANADA_SANTA_P ||
	 armaEquipada == BANANA_P ||
	 armaEquipada == GRANADA_ROJA_P
	 ) {
	        tiempoEspera = this->armaSeleccionada->getCuentaRegresiva() * 30;
        } 
        else {
	        tiempoEspera = 0;
        }

        
        b2Vec2 adelante;
        adelante = this->cuerpo->GetPosition(); 

        adelante.x += 0;
        adelante.y += 1;

        if (this->armaSeleccionada->getCaracteristicas().tienePotenciaVariable == true) {
      float angulo = this->armaSeleccionada->getAngulo();
	  float throwPower = this->armaSeleccionada->getPotencia();
	  float minPower = 5.0f;
	  float maxPower = 100.0f;
	  float minSpeed = 5.0f;
	  float maxSpeed = 20.0f;

	  float throwSpeed = minSpeed + (maxSpeed - minSpeed) * ((throwPower - minPower) / (maxPower - minPower));

	  float adyacente = cos(angulo) * throwSpeed;
	  float opuesto = sin(angulo) * throwSpeed;

	  if (this->direccion == IZQUIERDA)
	      adyacente *= -1;

      impulso.x = adyacente;
      impulso.y = opuesto;
        ataqueARealizar.posicion = adelante;
        }

        
        ataqueARealizar.tiempoEspera = tiempoEspera;
        ataqueARealizar.arma = armaQueQuiero;
        ataqueARealizar.impulsoInicial = impulso;
        this->estado = DISPARANDO;
        this->turno.usoSuArma = true;
        this->armaSeleccionada->usar();
        break;
        }
    case ATACO: {
        armaQueQuiero = NADA_P;
        tiempoEspera = 0;
        posicion = (deCoordAb2Vec(this->getCoords()));

        ataqueARealizar.posicion = posicion;
        ataqueARealizar.tiempoEspera = tiempoEspera;
        ataqueARealizar.arma = armaQueQuiero;
        this->estado = DISPARANDO;
        break;
    }
    case INVAL_ACCION:
        break;

    case CHEAT:
        break;
    }

    //Hago que el gusano se acuerde que fue lo ulitmo que realizo
    // si es ataco hay que "mentirle"
    if (this->ultimaAccion.accion == ATACO) {
        this->ultimaAccion.accion = ESTAQUIETO;
    } else {
        this->ultimaAccion = accion;

    }
    return ataqueARealizar;
    

}

void Gusano::teletransportarse() {
    std::pair<coordX, coordY> destino = this->armaSeleccionada->getCoordenadasTeletransporte();

    b2Vec2 vectorDestino = deCoordAb2Vec(destino);

    this->cuerpo->SetTransform(vectorDestino, true);

    this->armaSeleccionada->usar();
    
    //LE aplico alguito de velociadad para qeu se actualice y caiga
    b2Vec2 direccion;
    direccion.x = 0.0f;
    direccion.y = 1.0f;
    this->cuerpo->SetLinearVelocity(direccion);
}

void Gusano::giveGun(ArmaProtocolo arma) {
    this->armaEquipada = arma;
    this->armaSeleccionada = &this->armas[arma];
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
    RepresentacionArma arma = this->armaSeleccionada->getRepresentacion();
    // arma.municiones = 100000;
    // arma.arma = this->armaEquipada;

    repre.armaEquipada = arma;

    return repre;
}

//Puede ser que haya mas de un estado que represente que el gusano se
//murio
bool Gusano::estaMuerto() {
    bool meMori;
    meMori = (this->estado == MUERTO);

    bool meAhoge;
    meAhoge = (this->estado == AHOGADO);

    bool estoyMuerto;
    estoyMuerto = (meAhoge ||
	         meMori);

    return estoyMuerto;
}

bool Gusano::estaQuieto() {
    bool estaQuieto;
    estaQuieto = (this->estado == QUIETO);
    bool estoyMuerto;
    estoyMuerto = this->estaMuerto();

    bool estoyQuieto;
    estoyQuieto = (estaQuieto ||
	       estoyMuerto);

    return estoyQuieto;
}

void Gusano::recibirDanioCaida(b2Vec2 velocidad) {
    int distanciaQueCae = -std::pow(velocidad.y, 2) / (2.0f*config.gravedad.enY);
    
    if (distanciaQueCae < config.metrosSinDanio) {
        this->setEstado(QUIETO);
        return;
    }

    this->turno.recibioDano = true;


    float danio = std::min(distanciaQueCae*2, 25);
    if (this->vida <= (u_int)danio) {
        this->vida = 0;
        this->setEstado(MUERTO);
        return;
    }
    this->vida -= danio;
    this->setEstado(QUIETO);
}

void Gusano::anadirVida(int vidaExtra) {
    this->vida += vidaExtra;
}

void Gusano::anadirMunicion(int cantiMunicion, ArmaProtocolo queArma) {
    Arma &armaQueRecibe = this->armas.at(queArma);
    armaQueRecibe.anadirMuniciones(cantiMunicion);
}


void Gusano::recibirDanoPorCheat() {
    if (this->vida / 2 <= 0 ) {
        this->vida = 0;
        this->setEstado(MUERTO);
    }
    else {
        this->vida /= 2;
    }
}
