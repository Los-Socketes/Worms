#include "gusano.h"
#include <iostream>
#include "box2dDefs.h"
#include <math.h>       /* sin */
#include <time.h>


Gusano::Gusano() {
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
    double cuantoTiempoLlevo = difftime(tiempoActual, this->turno.cuandoArranco);
    if (this->turno.usoSuArma && cuantoTiempoLlevo < TIEMPOCAMBIOTURNO - TIEMPOCHANGUI) {
	  
        std::cout << "KILL YOU\n";
        this->setEstado(QUIETO);
        this->turno.cuandoArranco -= (TIEMPOCAMBIOTURNO - cuantoTiempoLlevo - 3);
    }

    bool cambiaDeTurno;

    //Esto, en teoria, da la rta en segundos
    bool noTengoMasTiempo;
    // double cuantoTiempoLlevo;
    cuantoTiempoLlevo = difftime(tiempoActual, this->turno.cuandoArranco);
    noTengoMasTiempo = (cuantoTiempoLlevo > TIEMPOCAMBIOTURNO);

    //Hay que cambiar de turno con que se cumpla alguna de estas condiciones
    cambiaDeTurno = (noTengoMasTiempo ||
        this->turno.recibioDano == true
        // ||
        // this->turno.usoSuArma == true
		 );

    // if (cambiaDeTurno == true) {
    //     std::cout << cuantoTiempoLlevo << "\n";
    //     std::cout << this->idGusano << "\n";
    //     std::cout << "CAMBIO DE TURNO!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
    // }


    this->tiempoQueMeQueda = TIEMPOCAMBIOTURNO - cuantoTiempoLlevo;
    // std::cout << "tiempo restante: " << this->tiempoQueMeQueda << "\n";
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
    std::cout << adyacente << "\n";

    float opuesto;
    opuesto = sin(angulo) * hipotenusa*2;

    b2Vec2 golpeDeseado(adyacente, opuesto);
    golpeYCaja.first = golpeDeseado;

    std::cout << "GOLPE: " << golpeDeseado.x << ", " << golpeDeseado.y << "\n";

    //CAJA
    std::pair<coordX, coordY> coords;
    coords = this->getCoords();

    std::cout << "ANGULO" << this->armaSeleccionada->getAngulo() << "\n";

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
        {
        if (this->estado == SALTANDO) {
            break;
        }
        std::cout << "Salto" << "\n";
        direccion.enX = (this->direccion == DERECHA) ? POTENCIASALTO : -POTENCIASALTO;
        direccion.enY = POTENCIASALTO*2;
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
        std::cout << "PIRUETA" << "\n";
        direccion.enX = (this->direccion == DERECHA) ? -POTENCIASALTO : POTENCIASALTO;
        direccion.enY = POTENCIASALTO*2;
        b2Vec2 saltoHorizontal = deCoordAb2Vec(direccion);
        this->cuerpo->ApplyLinearImpulseToCenter(saltoHorizontal,true);
        this->setEstado(HACE_PIRUETA);
        break;
        }
    case INVAL_DIR:
        std::cout << "Invalid dir" << "\n";
        abort();
        break;
    }
}

void Gusano::recibirDano(b2Vec2 golpe, Entidad *entidad) {
    if (this->golpeado == true)
        return;
    ArmaProtocolo tipoArma;
    tipoArma = entidad->proyectil.arma;

    float distanciaGusanoBomba = b2Distance(entidad->proyectil.posInicial, this->cuerpo->GetPosition());

    //TODO cambiar a que no tenga que crear el arma para obtener el danio
    Arma armaUsada(tipoArma);
    u_int danio = armaUsada.getDanio().epicentro;
    // distancia 0 = danio full
    // distancia 4 = dano 0
    //TODO Hacer que cada arma tenga esta formula
    // la formula es -12.5 * distancia + 50 = danio

    int radio = armaUsada.getDanio().radio;
    if (tipoArma == BATE_P) {
        radio = 2;
    }

    float danioReal;
    std::cout << "DISTANCIA: " << distanciaGusanoBomba << "\n";

    float porcentaje = 1.0f / (1.0f + std::pow(distanciaGusanoBomba / radio,2));

    // // Calculate the graduated damage
    danioReal = (distanciaGusanoBomba > radio) ? 0 : danio * porcentaje;
    if (tipoArma == BATE_P && distanciaGusanoBomba <= radio) {
        radio = 2;
        danioReal = danio;
    }

    std::cout << this->cuerpo->GetLinearVelocity().x << this->cuerpo->GetLinearVelocity().y << "\n";
    // std::cout << "Danio: " << danio << "\n";
    std::cout << "Danio: " << danioReal << "\n";
    std::cout << "distancia: " << porcentaje << "\n";
    std::cout << "distancia: " << distanciaGusanoBomba << "\n";
    if (this->vida < (u_int) danioReal) {
        this->vida = 0;
        this->setEstado(MUERTO);

    } else {
        printf("RECIBI DANO\n");
        this->vida -= danioReal;
    }
    this->turno.recibioDano = true;


    // golpe.x *= distancia;
    // golpe.y *= distancia;
    // b2Vec2 golpeton(0.0f, 0.0f);
    
    // this->cuerpo->ApplyLinearImpulseToCenter(golpeton, true);
    std::cout << "Vida nueva: " << this->vida << "\n";
    if (tipoArma == BATE_P) {
        this->cuerpo->SetLinearVelocity(golpe);
    }
    this->setEstado(HERIDO);
    // this->cuerpo->ApplyLinearImpulseToCenter(golpe, true);
    // this->cuerpo->ApplyLinearImpulseToCenter(golpe, true);


    this->golpeado = true;
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
        // std::cout << "Cambio: " << configDeseado.angulo << "\n";
        // std::cout << "Angulo nuevo: " << anguloActual << "\n";
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
        // std::cout << "Cambio: " << configDeseado.potencia << "\n";
        // std::cout << "Potencia nueva: " << potenciaActual << "\n";
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

Ataque Gusano::ejecutar(Accion accion, Proyectil *proyectil) {
    Ataque ataqueARealizar;
    if (this->getTiempoQueMeQueda() <= 0) {
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
        // Arma armaSeleccionada(armaElegida);
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
        std::cout << "PREPARAR\n";

        this->preparar(accion);

        // armaQueQuiero = NADA_P;
        armaQueQuiero = NADA_P;

        tiempoEspera = 0;
        posicion = (deCoordAb2Vec(this->getCoords()));

        ataqueARealizar.arma = NADA_P;
        ataqueARealizar.posicion = posicion;
        ataqueARealizar.tiempoEspera = tiempoEspera;
        ataqueARealizar.arma = armaQueQuiero;
        ataqueARealizar.potencia = this->armaSeleccionada->getPotencia();
        // std::cout << "AYUDA AYUDA" << accion.configARealizar.potencia << "\n";
        // if(armaEquipada == DINAMITA_P || armaEquipada == GRANADA_VERDE_P
	 
        // 	 ) {
        // 	        proyectil->tipo = TipoProyectil::Countdown;
        // } else if (armaEquipada == BAZOOKA_P) {
        // 	  proyectil->tipo = TipoProyectil::Colision;
        // }
        break;
    case ATAQUE:
        {
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

        ataqueARealizar.posicion = posicion;
        // tiempoEspera = 99;
        if(armaEquipada == DINAMITA_P ||
	 armaEquipada == GRANADA_VERDE_P ||
	 armaEquipada == GRANADA_SANTA_P ||
     armaEquipada == BANANA_P 
	 ) {
	        proyectil->tipo = TipoProyectil::Countdown;
	        tiempoEspera = this->armaSeleccionada->getCuentaRegresiva() * 30;
	        // tiempoEspera = 100;
	        std::cout << "Tiempo: " << tiempoEspera << "\n";
        } else if (armaEquipada == BAZOOKA_P) {
	  proyectil->tipo = TipoProyectil::Colision;
        }
        else {
            proyectil->tipo = TipoProyectil::Ningun;
	        tiempoEspera = 0;
        }

        // std::cout << "MI POS: " << proyectil->cuerpo->GetPosition().x
        // 	        << " " << proyectil->cuerpo->GetPosition().y << "\n";

        b2Vec2 adelante;
        adelante = this->cuerpo->GetPosition(); 

        adelante.x += 0;
        adelante.y += 1;
        proyectil->cuerpo->SetTransform(adelante , true);
        // std::cout << "MI POS: " << proyectil->cuerpo->GetPosition().x
        // 	        << " " << proyectil->cuerpo->GetPosition().y << "\n";
        if (this->armaEquipada == BANANA_P) {
            b2FixtureDef fixtureNuevo;
            b2CircleShape circleShape;
            circleShape.m_radius = 0.05; // very small
        
            fixtureNuevo.shape = &circleShape;
            fixtureNuevo.density = 3.0f;
            fixtureNuevo.friction = 0.3f;
            fixtureNuevo.restitution = 0.8;

            proyectil->cuerpo->DestroyFixture(proyectil->fixture);

            proyectil->fixture = proyectil->cuerpo->CreateFixture(&fixtureNuevo);
        }
        if (this->armaEquipada == DINAMITA_P) {
            b2FixtureDef fixtureNuevo;
            b2CircleShape circleShape;
            circleShape.m_radius = 0.05; // very small
        
            fixtureNuevo.shape = &circleShape;
            fixtureNuevo.density = 3.0f;
            fixtureNuevo.friction = 0.3f;
            fixtureNuevo.restitution = 0;

            proyectil->cuerpo->DestroyFixture(proyectil->fixture);

            proyectil->fixture = proyectil->cuerpo->CreateFixture(&fixtureNuevo);
        }

        if (this->armaSeleccionada->getCaracteristicas().tienePotenciaVariable == true) {
      float angulo = this->armaSeleccionada->getAngulo();
	  float throwPower = this->armaSeleccionada->getPotencia();
	  float minPower = 5.0f;
	  float maxPower = 100.0f;
	  float minSpeed = 5.0f;  // Adjust this value for the minimum throw speed
	  float maxSpeed = 20.0f;  // Adjust this value for the maximum throw speed

	  // Map throwPower to the throw speed range using linear interpolation
	  float throwSpeed = minSpeed + (maxSpeed - minSpeed) * ((throwPower - minPower) / (maxPower - minPower));

	  std::cout << "ANGULO: " << angulo << "\n"; 
	  float adyacente = cos(angulo) * throwSpeed;
	  float opuesto = sin(angulo) * throwSpeed;

	  if (this->direccion == IZQUIERDA)
	      adyacente *= -1;
	  std::cout << "POTENCIA POTENCIA" << this->armaSeleccionada->getPotencia() << "\n"; 

	  b2Vec2 golpeDeseado(adyacente, opuesto);
	  proyectil->cuerpo->SetLinearVelocity(golpeDeseado);
        }

        // std::cout << "MI POS: " << proyectil->cuerpo->GetPosition().x
        // 	        << " " << proyectil->cuerpo->GetPosition().y << "\n";

        ataqueARealizar.tiempoEspera = tiempoEspera;
        ataqueARealizar.arma = armaQueQuiero;
        this->estado = DISPARANDO;
        this->turno.usoSuArma = true;
        std::cout << ataqueARealizar.arma << "ATACO\n";
        this->armaSeleccionada->usar();
        break;
        }
    case INVAL_ACCION:
        std::cout << "INVALID ACCION\n";
        break;
    }


    //Hago que el gusano se acuerde que fue lo ulitmo que realizo;
    this->ultimaAccion = accion;

    return ataqueARealizar;
    

}

void Gusano::teletransportarse() {
    std::pair<coordX, coordY> destino = this->armaSeleccionada->getCoordenadasTeletransporte();

    std::cout << destino.first << " " << destino.second << "\n";
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
    // std::cout << "Angulo a enviar: " << arma.anguloRad << "\n";
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
    float tiempoParaMetrosMax = std::sqrt(METROS_SIN_DANIO*2/(-FUERZAGRAVITARIAY)); 
    float velocidadMax = -METROS_SIN_DANIO*2/tiempoParaMetrosMax;
    if (velocidad.y >= velocidadMax) {
        this->setEstado(QUIETO);
        return;
    }


    printf("RECIBI DANO DE CAIDA\n");

    int distanciaQueCae = -std::pow(velocidad.y, 2) / (2.0f*FUERZAGRAVITARIAY);

    std::cout << "DISTANCIA QUE CAE: " << distanciaQueCae << "\n";
    std::cout << "VELOCIDAD QUE CAE: " << velocidad.y << "\n";
    int vidaMax = 100;
    float danio = distanciaQueCae * vida / (METROS_SIN_DANIO*3);
    if (this->vida <= (u_int) danio) {
        this->vida = 0;
        this->setEstado(MUERTO);
        return;
    }
    this->vida -= danio;
    this->setEstado(QUIETO);
}
