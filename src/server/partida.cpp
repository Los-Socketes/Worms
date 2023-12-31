#include "partida.h"

#include "box2dDefs.h"

#include <map>
#include <random>

#define SLEEPSEGS 1
#define NOW NULL
#define PLANO 0


// Usado para castear un puntero a una reference y hacer
// el codigo mas explicito
#define REFERENCE *

const std::chrono::duration<double> frameDuration(1.0 / 30);

int numeroRandomEnRango(int comienzo, int fin) {
    int resultado;
    resultado = rand() % (fin + 1) + comienzo;

    return resultado;
}

Partida::Partida(Mapa mapa):
    world(b2Vec2(config.gravedad.enX, config.gravedad.enY )){
    this->world.SetContactListener(&this->colisiones);
    this->posJugadorActual = -1;
    this->finPartida = false;
    this->colisiones.finPartida = false;
    this->termino = false;
    this->momento = ESPERANDO;
    this->ultimaProvision = time(NOW);
    this->mapaUsado = mapa;
    this->viento = b2Vec2(config.viento.enX, config.viento.enY);
    this->ultimoCambioViento = time(NOW);
    
    for (auto &&viga : mapa.vigas) {
        this->anadirViga(viga.angulo, viga.tamanio, viga.coordenadas);
    }

    for (int i = 0; i < (int)mapa.posicionGusanos.size(); i++) {
        this->posicionesGusanos.insert({i, mapa.posicionGusanos[i]});
    }
    
    this->cantidad_gusanos_insertados = 0;

    this->anadirOceano(std::pair<coordX, coordY>(0.0f, 0.0f));
}

Gusano *Partida::anadirGusano(std::pair<coordX, coordY> coords) {
    Entidad *nuevaEntidad = new Entidad;
    nuevaEntidad->tipo = TipoEntidad::GUSANO;
    Gusano *nuevoGusano = new Gusano(std::ref(mapaUsado));
    nuevaEntidad->gusano = nuevoGusano;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(coords.enX, coords.enY);
    bodyDef.userData.pointer = reinterpret_cast<uintptr_t> (nuevaEntidad);
    b2Body* body = world.CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;

    dynamicBox.SetAsBox(0.5f, 0.5f);
  
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    fixtureDef.filter.categoryBits = (uint16_t)TipoEntidad::GUSANO;
    fixtureDef.filter.maskBits = (uint16_t)TipoEntidad::VIGA |
        (uint16_t)TipoEntidad::OCEANO |
        (uint16_t)TipoEntidad::PROYECTIL |
        (uint16_t)TipoEntidad::PROVISION;

    body->CreateFixture(&fixtureDef);
    nuevoGusano->setCuerpo(body);

    this->gusanos.push_back(nuevoGusano);
    int idGusano;
    idGusano = this->gusanos.size() - 1;
    nuevoGusano->giveId(idGusano);

    return nuevoGusano;
}

void Partida::anadirProvision(std::pair<coordX, coordY> coordenadas, tipoProvision queProvision, bool esTrampa) {
    std::pair<coordX, coordY> posicionInicial;
    posicionInicial = coordenadas;
        
    Entidad *nuevaEntidad = new Entidad;
    nuevaEntidad->tipo = TipoEntidad::PROVISION;

    b2BodyDef provisionDef;
    provisionDef.type = b2_dynamicBody;
    provisionDef.position.Set(posicionInicial.enX, posicionInicial.enY);
    provisionDef.userData.pointer = reinterpret_cast<uintptr_t> (nuevaEntidad);

    b2PolygonShape provision;
    provision.SetAsBox(0.5f, 0.5f);

    b2FixtureDef provisionFixDef;
    provisionFixDef.shape = &provision;
    provisionFixDef.density = 0.1f;
    provisionFixDef.friction = 0.3f;
    provisionFixDef.filter.categoryBits = (uint16_t)TipoEntidad::PROVISION;
    provisionFixDef.filter.maskBits = (uint16_t)TipoEntidad::VIGA |
        (uint16_t)TipoEntidad::OCEANO |
        (uint16_t)TipoEntidad::PROYECTIL |
        (uint16_t)TipoEntidad::GUSANO;

    b2Body* provisionBody = world.CreateBody(&provisionDef);
    provisionBody->CreateFixture(&provisionFixDef);

    int tipo;
    tipo = numeroRandomEnRango(0,1);

    ArmaProtocolo arma = BANANA_P;
    if (queProvision == VIDA) {
        arma = NADA_P;
    } else {
        std::vector<ArmaProtocolo> armasConMunicion;
        for (int i = 0; i < INVAL_ARMA_P; i++) {
	  Arma arma((ArmaProtocolo)i);
	  if (arma.getMuniciones() == -1)
	      continue;
	  armasConMunicion.push_back((ArmaProtocolo)i);
        }
        int pri = 0;
        //Longitud del array
        int fin = armasConMunicion.size() - 1;

        int armaAUsar = numeroRandomEnRango(pri, fin);

        arma = armasConMunicion.at(armaAUsar);
    }

    int idProvision;
    idProvision = this->cantidadProvisionesGeneradas;
    this->cantidadProvisionesGeneradas += 1;

    Provision *nuevaProvision = new Provision(queProvision, arma, provisionBody, idProvision, esTrampa);
    nuevaEntidad->provision = nuevaProvision;

    this->provisiones.push_back(nuevaProvision);
}

void Partida::anadirViga(radianes angulo, int longitud, std::pair<coordX, coordY> posicionInicial) {
    Entidad *nuevaEntidad = new Entidad;
    nuevaEntidad->tipo = TipoEntidad::VIGA;

    b2BodyDef vigaDef;
    vigaDef.position.Set(posicionInicial.enX, posicionInicial.enY);
    vigaDef.angle = angulo;
    vigaDef.userData.pointer = reinterpret_cast<uintptr_t> (nuevaEntidad);

    //ATTENTION Dividimos a la mitad porque box2d pide la mitad de
    // la longitud
    longitud /= 2;

    b2PolygonShape viga;
    viga.SetAsBox(longitud, ANCHOVIGA / 2);

    b2Body* groundBody = world.CreateBody(&vigaDef);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &viga;
    fixtureDef.density = MASACUERPOESTATICO;

    fixtureDef.filter.categoryBits = (uint16_t)TipoEntidad::VIGA;
    fixtureDef.filter.maskBits = -1;

    groundBody->CreateFixture(&fixtureDef);

    RepresentacionViga vigaEnMapa;
    vigaEnMapa.angulo = angulo;
    vigaEnMapa.longitud = longitud * 2;
    vigaEnMapa.posicionInicial = posicionInicial;
    this->vigasEnMapa.push_back(vigaEnMapa);

}

void Partida::anadirOceano(std::pair<coordX, coordY> posicionInicial) {
    Entidad *nuevaEntidad = new Entidad;
    nuevaEntidad->tipo = TipoEntidad::OCEANO;

    b2BodyDef oceanoDef;
    oceanoDef.position.Set(posicionInicial.enX, posicionInicial.enY);
    oceanoDef.userData.pointer = reinterpret_cast<uintptr_t> (nuevaEntidad);

    b2PolygonShape oceano;
    oceano.SetAsBox(999999, ANCHOVIGA / 2);

    b2Body* oceanoCuerpo = world.CreateBody(&oceanoDef);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &oceano;
    fixtureDef.density = MASACUERPOESTATICO;

    fixtureDef.filter.categoryBits = (uint16_t)TipoEntidad::OCEANO;
    fixtureDef.filter.maskBits = -1;

    oceanoCuerpo->CreateFixture(&fixtureDef);
}


InformacionInicial Partida::obtenerInfoInicial() {
    InformacionInicial infoInicial;
    //Le damos los gusanos al jugador del cliente y acceso a la queue
    //de acciones
    Jugador *jugadorNuevo = new Jugador();
    this->jugadores.push_back(jugadorNuevo);

    //Creo el id del nuevo jugador
    idJugador idNuevoJugador;
    idNuevoJugador = (idJugador) this->jugadores.size();
    infoInicial.jugador = idNuevoJugador;
    jugadorNuevo->setID(idNuevoJugador);

    infoInicial.vigas = this->vigasEnMapa;
    infoInicial.dimensiones = this->mapaUsado.dimensiones;

    return infoInicial;
}

void Partida::anadirCliente(Cliente *clienteNuevo) {
    clienteNuevo->obtenerAccesoAAcciones(&this->acciones);

    //Anadimos al jugador a la partida
    this->clientes.push_back(clienteNuevo);
    //Aviso que se unio un jugador
    this->seUnioJugador.notify_all();
    this->enviarEstadoAJugadores();
}

bool Partida::enviarEstadoAJugadores() {
    std::shared_ptr<EstadoDelJuego> estadoActual(new EstadoDelJuego);

    std::map<idJugador, std::map<id, RepresentacionGusano>> representacionGusanos;
    std::map<idJugador, SituacionJugador> situaciones;

    estadoActual->jugadorDeTurno = 1;
    estadoActual->gusanoDeTurno = 0;
    estadoActual->segundosRestantes = 0;
    for (int jugador = 0; jugador < (int) this->jugadores.size() ; jugador++) {
        Jugador *jugadorActual;
        jugadorActual = this->jugadores.at(jugador);

        if (jugador == this->posJugadorActual) {
	  estadoActual->jugadorDeTurno = jugador;
	  estadoActual->gusanoDeTurno = jugadorActual->getGusanoDeTurno()->getId();
	  estadoActual->segundosRestantes = jugadorActual->getGusanoDeTurno()->getTiempoQueMeQueda();
        }

        std::map<id, RepresentacionGusano> gusanosJugActual;

        gusanosJugActual = jugadorActual->getRepresentacionGusanos();

        representacionGusanos.insert({jugador, gusanosJugActual});

        //Aca configuro las situaciones
        SituacionJugador situacion;
        situacion = jugadorActual->getSituacion();

        situaciones.insert({jugador, situacion});

        
    }
    estadoActual->gusanos = representacionGusanos;
    estadoActual->situacionJugadores = situaciones;


    std::vector<RepresentacionProyectil> proyectilesRepre;
    for (Proyectil *proyectil : this->proyectiles) {
        if (
	  proyectil->armaOrigen == NADA_P ||
	  proyectil->armaOrigen == TELETRANSPORTACION_P ||
	  proyectil->armaOrigen == BATE_P
	  )
	  continue;
        RepresentacionProyectil repre;
        repre.id = proyectil->id;
        repre.proyectil = proyectil->armaOrigen;
        repre.esFragmento = proyectil->esFragmento;

        repre.posicion = deb2VecACoord(proyectil->cuerpo->GetPosition());
        
        b2Vec2 velocidad = proyectil->cuerpo->GetLinearVelocity();
        if (velocidad.y > 0) {
            repre.angulo = std::atan(velocidad.y/velocidad.x);
            repre.angulo = M_PI/2 - repre.angulo;
        } else {
            repre.angulo = std::atan(-velocidad.y/velocidad.x);
            repre.angulo += M_PI/2;
        }
        if (velocidad.x < 0) {
            repre.angulo += M_PI;
        } 
        if (proyectil->armaOrigen == DINAMITA_P) {
            repre.angulo = 0;
        }
        repre.cuentaRegresiva = proyectil->countdown;
        repre.exploto = proyectil->exploto;

        proyectilesRepre.push_back(repre);
    }
    estadoActual->proyectiles = proyectilesRepre;

    estadoActual->momento = this->momento;

    std::vector<RepresentacionProvisiones> representacionProvi;
    for (Provision *provision : this->provisiones) {
        RepresentacionProvisiones repreActual;
        repreActual = provision->getRepresentacin();
        representacionProvi.push_back(repreActual);
    }
    estadoActual->provisiones = representacionProvi;

    estadoActual->viento = this->viento.x;

    estadoActual->ronda = this->rondas;

    bool hayJugadores = false;
    for(Cliente *cliente : this->clientes) {
        if (cliente != nullptr && !cliente->estaMuerto()) {
            cliente->enviarEstadoJuego(estadoActual);
            hayJugadores = true;
        }
    }
    return hayJugadores;
}

void Partida::procesarCheats(Accion cheat, Gusano *gusanoActual,
		         Jugador *jugadorActual) {
    TipoCheat cheatDeseado;
    cheatDeseado = cheat.cheat;

    switch(cheatDeseado) {
    case PROVISION_C:
        {
        std::pair<coordX, coordY> coordenadasGusano;
        coordenadasGusano = gusanoActual->getCoords();
        coordenadasGusano.enY += 3;
        this->anadirProvision(coordenadasGusano, MUNICION, false);
        break;
        }
    case VIDA_C:
        {
        std::pair<coordX, coordY> coordenadasGusano;
        coordenadasGusano = gusanoActual->getCoords();
        coordenadasGusano.enY += 3;
        this->anadirProvision(coordenadasGusano, VIDA, false);
        break;
        }
    case DANIO_C:
        for (Jugador *jugador : this->jugadores) {
	  if (jugador == jugadorActual)
	      continue;
	  std::vector<Gusano *> gusanosEnemigos;
	  gusanosEnemigos = jugador->getGusanos();
	  for (Gusano *gusanoEnemigo : gusanosEnemigos) {
	      gusanoEnemigo->recibirDanoPorCheat();
	  }
        }
        break;
    case ARRANCAR_C:
        //Este cheat se procesa antes, en el loop de conexion
        break;
    case INVAL_CHEAT_C:
        break;
    }

}

Accion Partida::obtenerAccion(Accion accionObtenida, bool obtuvoNueva,
			Gusano* gusanoActual) {
    Accion accionAEjecutar;
    //Si la ultima accion fue de movimiento y no obtuvimos nada
    //nuevo; ejecutamos esa accion de movimiento.
    //AKA: Nos movemos a la Izquierda hasta que nos digan de detenernos
    Accion ultimaAccion = gusanoActual->getUltimaAccion();

    tipoAccion tipoUltimaAccion;
    tipoUltimaAccion = ultimaAccion.accion;
    if (obtuvoNueva == true) {
        accionAEjecutar = accionObtenida;
    }
    //Si entra en estos otros if es porque NO se obtuvo algo nuevo
    else if (tipoUltimaAccion == MOVERSE &&
        (ultimaAccion.dir != SALTO &&
        ultimaAccion.dir != PIRUETA)) {
        accionAEjecutar = ultimaAccion;
    }
    else if (tipoUltimaAccion == ATAQUE) {
        accionAEjecutar = ultimaAccion;
        accionAEjecutar.accion = ATACO;
    } else if (tipoUltimaAccion == ATACO) {
        accionAEjecutar = ultimaAccion;
    } else {
        accionAEjecutar = ultimaAccion;
        accionAEjecutar.accion = ESTAQUIETO;
    }

    return accionAEjecutar;
}

void Partida::generarExplosion(Proyectil *proyectil) {
    if (proyectil->tipo == TipoProyectil::Countdown && proyectil->countdown > 0) {
        return;
    }

    else if (proyectil->tipo == TipoProyectil::Colision && proyectil->colisiono == false) {
        return;
    }
    proyectil->exploto = true;
    proyectil->colisiono = true;
    if ((proyectil->armaOrigen == MORTERO_P || proyectil->armaOrigen == GRANADA_ROJA_P) && 
        !proyectil->esFragmento) {
        Arma armaElegida(proyectil->armaOrigen);
        this->crearFragmentos(proyectil, armaElegida.getFragmentos());
    }

    //Fuente: https://www.iforce2d.net/b2dtut/explosions
    int numRays = 32;
    for (int i = 0; i < numRays; i++) {
        Entidad *nuevaEntidad = new Entidad;
        nuevaEntidad->tipo = TipoEntidad::PROYECTIL;
        nuevaEntidad->proyectil.arma = proyectil->armaOrigen;

        float angle = (i / (float)numRays) * 360 * DEGTORAD;
        b2Vec2 rayDir( sinf(angle), cosf(angle) );

        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        bd.fixedRotation = true; // rotation not necessary
        bd.bullet = true; // prevent tunneling at high speed
        // bd.linearDamping = 10; // drag due to moving through air
        bd.gravityScale = 0; // ignore gravity
        bd.userData.pointer = reinterpret_cast<uintptr_t> (nuevaEntidad);
        b2Vec2 coords = proyectil->cuerpo->GetPosition();
        nuevaEntidad->proyectil.posInicial = coords;
        bd.position = coords;
        // bd.linearVelocity = blastPower * rayDir;
        bd.linearVelocity = 20 * rayDir;
        b2Body* body = this->world.CreateBody( &bd );

        b2CircleShape circleShape;
        circleShape.m_radius = 0.05; // very small

        b2FixtureDef fd;
        fd.shape = &circleShape;
        fd.density = 60 / (float)numRays; // very high - shared across all particles
        fd.friction = 0; // friction not necessary
        fd.restitution = 0.99f; // high restitution to reflect off obstacles
        fd.filter.groupIndex = -1; // particles should not collide with each other
        fd.filter.categoryBits = (uint16_t)TipoEntidad::PROYECTIL;
        fd.filter.maskBits = -1;

        body->CreateFixture( &fd );
        nuevaEntidad->proyectil.proyectil = body;
        nuevaEntidad->proyectil.horaDeCreacion = time(NOW);
        nuevaEntidad->proyectil.tiempoMinimoDeVida = 0.5f;
        
        Proyectil *proyectilNuevo = new Proyectil();
        *proyectilNuevo = *proyectil;
        nuevaEntidad->proyectilReal = proyectilNuevo;

        this->cuerposADestruir.push_back(body);
    }

    if (proyectil->armaOrigen == BANANA_P || proyectil->armaOrigen == DINAMITA_P) {
        b2FixtureDef fixtureNuevo;
        b2CircleShape circleShape;
        circleShape.m_radius = 0.05; // very small
    
        fixtureNuevo.shape = &circleShape;
        fixtureNuevo.density = 3.0f;
        fixtureNuevo.friction = 0.3f;
        fixtureNuevo.restitution = 0.2;

        proyectil->cuerpo->DestroyFixture(proyectil->fixture);

        proyectil->fixture = proyectil->cuerpo->CreateFixture(&fixtureNuevo);
    }
}


void Partida::crearFragmentos(Proyectil* proyectil, int cantFragmentos) {
    for (int i = 0; i < cantFragmentos; i++) {
        Proyectil *nuevoProyectil = this->proyectilConstructor();
        nuevoProyectil->armaOrigen = proyectil->armaOrigen;
        nuevoProyectil->exploto = false;
        nuevoProyectil->colisiono = false;
        nuevoProyectil->enElAire = true;
        nuevoProyectil->esFragmento = true;
        nuevoProyectil->tipo = TipoProyectil::Colision;

        float angle = (i / (float)cantFragmentos) * 360 * DEGTORAD;
        b2Vec2 rayDir( sinf(angle), cosf(angle) );

        b2Vec2 coords = proyectil->cuerpo->GetPosition();
        coords += rayDir; 
        
        nuevoProyectil->cuerpo->SetTransform(coords, true);
        
        b2Vec2 linearVelocity = 2 * rayDir;
        nuevoProyectil->cuerpo->SetLinearVelocity(linearVelocity);
    }
    
}

void Partida::crearProyectiles(Gusano *gusano, Ataque ataque) {
    ArmaProtocolo arma = ataque.arma;
    if (arma == NADA_P || arma == TELETRANSPORTACION_P) {
        return;
    }

    Arma armaFinal(arma);

    int cantProyectiles = 0;
    cantProyectiles = armaFinal.getCantidadProyectiles();

    for (int i = 0 ; i < cantProyectiles; i++) {
        Proyectil *nuevoProyectil = this->proyectilConstructor();
        nuevoProyectil->armaOrigen = arma;
        nuevoProyectil->exploto = false;
        nuevoProyectil->colisiono = false;
        nuevoProyectil->countdown = ataque.tiempoEspera;
        nuevoProyectil->esFragmento = false;

        if (arma == DINAMITA_P || arma == GRANADA_VERDE_P || 
            arma == GRANADA_SANTA_P || arma == BANANA_P || 
            arma == GRANADA_ROJA_P) {
	  nuevoProyectil->tipo = TipoProyectil::Countdown;
        }
        else if (arma == BAZOOKA_P || arma == ATAQUE_AEREO_P || arma == MORTERO_P) {
	  nuevoProyectil->tipo = TipoProyectil::Colision;
        } else {
	  nuevoProyectil->tipo = TipoProyectil::Ningun;
        }

        if (arma == BATE_P) {
            nuevoProyectil->exploto = true;
	  std::pair<b2Vec2, std::pair<inicioCaja, finCaja>> golpeYCaja;
	  golpeYCaja = gusano->ejecutarGolpe();
	  std::pair<inicioCaja, finCaja> coordsGolpe;
	  coordsGolpe = golpeYCaja.second;
	  b2Vec2 golpe;
	  golpe = golpeYCaja.first;

	  ResolvedorQuery query;
	  b2AABB aabb;
	  aabb.lowerBound = coordsGolpe.inicio;
	  aabb.upperBound = coordsGolpe.fin;
	  this->world.QueryAABB( &query, aabb );

	  for (int i = 0; i < (int) query.foundBodies.size(); i++) {
	      b2Body* cuerpoA = query.foundBodies[i];
	      if (cuerpoA->GetType() == b2_staticBody)
		continue;

	      Entidad *entidadA = (Entidad *) cuerpoA->GetUserData().pointer;

	      Entidad *nuevaEntidad = new Entidad;
	      nuevaEntidad->tipo = TipoEntidad::PROYECTIL;
	      nuevaEntidad->proyectil.arma = nuevoProyectil->armaOrigen;
	      b2Vec2 coords = golpeYCaja.second.first;
	      nuevaEntidad->proyectil.posInicial = coords;
	      nuevaEntidad->proyectilReal = nuevoProyectil;

	      entidadA->gusano->recibirDano(golpe, nuevaEntidad);
	  }
        }

        if (arma == BANANA_P) {
	  b2FixtureDef fixtureNuevo;
	  b2CircleShape circleShape;
	  circleShape.m_radius = 0.05; // very small

	  fixtureNuevo.shape = &circleShape;
	  fixtureNuevo.density = 3.0f;
	  fixtureNuevo.friction = 0.3f;
	  fixtureNuevo.restitution = 0.8;

	  nuevoProyectil->cuerpo->DestroyFixture(nuevoProyectil->fixture);

	  nuevoProyectil->fixture = nuevoProyectil->cuerpo->CreateFixture(&fixtureNuevo);
        } else if (arma == DINAMITA_P) {
	  b2FixtureDef fixtureNuevo;
	  b2CircleShape circleShape;
	  circleShape.m_radius = 0.05; // very small

	  fixtureNuevo.shape = &circleShape;
	  fixtureNuevo.density = 3.0f;
	  fixtureNuevo.friction = 0.3f;
	  fixtureNuevo.restitution = 0;

	  nuevoProyectil->cuerpo->DestroyFixture(nuevoProyectil->fixture);

	  nuevoProyectil->fixture = nuevoProyectil->cuerpo->CreateFixture(&fixtureNuevo);
        }

        else if (arma == ATAQUE_AEREO_P) {
	  ataque.posicion.y += 5;
	  ataque.posicion.x += 2;
        }

        if (armaFinal.getCaracteristicas().esAfectadoPorViento == true)
	  ataque.impulsoInicial += viento;

        nuevoProyectil->cuerpo->SetTransform(ataque.posicion, true);
        nuevoProyectil->cuerpo->SetLinearVelocity(ataque.impulsoInicial);

    }
}


Jugador *Partida::siguienteJugador(Jugador *viejoJugador) {
    Jugador *jugadorActual;
    //Si con el siguiente me paso, volve a 0
    if (this->posJugadorActual + 1 >= (int) this->jugadores.size())
        this->posJugadorActual = -1;

    this->posJugadorActual += 1;
    jugadorActual = this->jugadores.at(this->posJugadorActual);

    return jugadorActual;
}


bool Partida::sePuedeCambiarDeJugador(Gusano *gusanoActual, time_t tiempoActual) {
    bool finDelGusano;
    finDelGusano = gusanoActual->hayQueCambiarDeTurno(tiempoActual);
    if (finDelGusano == false)
        return false;
    bool todoExploto = true;
    for (auto &&proyectil : this->proyectiles) {
        if (!proyectil->exploto) {
            todoExploto = false;
        }
        if (proyectil->cuerpo->GetPosition().y < 5) {
            proyectil->exploto = true;
        } 
    }
    if (todoExploto == false) {
        return false;
    }
    
    bool todoEstaQuieto = true;
    for (int i = 0; (i < (int) this->gusanos.size())
		&&
		//Apenas sea false, no quiero seguir iterando
		(todoEstaQuieto == true);
         i++) {
        Gusano *gusano;
        gusano = this->gusanos.at(i);

        bool gusanoEstaQuieto;
        gusanoEstaQuieto = gusano->estaQuieto();

        //Con que uno de estos sea false, ya te hace el valor false
        todoEstaQuieto = gusanoEstaQuieto;
    }
    if (todoEstaQuieto == false)
        return false;


    //Si llegue hasta aca, significa que todas las otras condiciones
    //se cumplieron, entonces sabemos que se puede cambiar de turno
    return true;

}

std::pair<Gusano *, Jugador *> Partida::cambiarDeJugador(Jugador *jugadorTurnoActual, Gusano *gusanoActual, time_t tiempoActual) {
    std::pair<Gusano *, Jugador *> gusanoYJugador;
    Gusano *gusanoDeTurno = gusanoActual;
    Jugador *jugadorDeTurno = jugadorTurnoActual;
    gusanoYJugador.first = gusanoActual;
    gusanoYJugador.second = jugadorDeTurno;

    bool cambioDeTurno;
    cambioDeTurno = this->sePuedeCambiarDeJugador(gusanoActual, tiempoActual);

    //Si no hay cambio de turno, devolvemos el mismo gusano
    if (cambioDeTurno == false)
        return gusanoYJugador;

    //Si el ultimo jugador cambia de turno, significa que tenemos una
    //ronda nueva
    if(jugadorTurnoActual->getID() == (idJugador) this->jugadores.size())
        this->rondas += 1;


    //Antes de nada me fijo si el jugador actual perdio
    jugadorTurnoActual->chequearSiPerdi();

    gusanoActual->setEstado(QUIETO);

    gusanoDeTurno = nullptr;
    for (int i = 0; i <= (int)this->jugadores.size(); i++) {
        jugadorDeTurno = this->siguienteJugador(jugadorTurnoActual);
        gusanoDeTurno = jugadorDeTurno->getGusanoActual();
        if (gusanoDeTurno != nullptr) {
            break;
        }
    }
    if (gusanoDeTurno != nullptr) {
        gusanoActual->giveGun(NADA_P);
        gusanoDeTurno->esMiTurno(tiempoActual);
    }
    gusanoYJugador.first = gusanoDeTurno;
    gusanoYJugador.second = jugadorDeTurno;

    //Este for loop chequea si el proximo gano
    bool elProximoGano = true;
    // for (Jugador *jugador : this->jugadores)
    for (int i = 0; (i < (int) this->jugadores.size())
		&&
		//Apenas sea false, no quiero seguir iterando
		(elProximoGano == true);
         i++) {
        Jugador *jugadorAChequear;
        jugadorAChequear = this->jugadores.at(i);

        if (jugadorAChequear == jugadorDeTurno)
	  continue;
        jugadorAChequear->chequearSiPerdi();
        SituacionJugador situacion;
        situacion = jugadorAChequear->getSituacion();
        bool jugadorPerdio;
        jugadorPerdio = (situacion == PERDISTE);

        //Con que uno de estos sea false, ya te hace el valorFalse
        elProximoGano = elProximoGano && jugadorPerdio;
    }

    if (elProximoGano == true) {
        jugadorDeTurno->avisarQueGane();
        this->termino = true;
        this->momento = TERMINADA;
    }

    return gusanoYJugador;
}

void Partida::generarProvision(time_t horaActual) {
    double diferencia;
    diferencia = difftime(horaActual, this->ultimaProvision);

    if (diferencia < config.esperaProvision)
        return;

    //Actualizo la hora incluso si el coin flip no lo logra.
    //Es para que SIEMPRE haya TIEMPOESPERAPROVISION de tiempo entre
    //intento e intento
    this->ultimaProvision = horaActual;

    int cointFlip = numeroRandomEnRango(0,1);

    if (cointFlip == 0)
        return;


    std::vector<Viga> vigasChatas;
    for (Viga viga : this->mapaUsado.vigas) {
        if (viga.angulo != PLANO)
	  continue;
        vigasChatas.push_back(viga);
    }

    int ultimaViga;
    ultimaViga = vigasChatas.size() - 1;

    int vigaRandom;
    vigaRandom = numeroRandomEnRango(0, ultimaViga);

    Viga vigaElegida;
    vigaElegida = vigasChatas.at(vigaRandom);

    int offestRandom;
    offestRandom = numeroRandomEnRango(0, vigaElegida.tamanio);

    //Restamos la mitad porque la coordenada se toma en el centro (creemos)
    float coordALaIzquierda;
    coordALaIzquierda = vigaElegida.coordenadas.enX - (vigaElegida.tamanio / 2.0f) - 0.5;
    //0.5 es una pequena correccion para que no se caiga

    float posEnX;
    posEnX = coordALaIzquierda + offestRandom;

    float posEnY = mapaUsado.dimensiones.enY;
    std::pair<coordX, coordY> coordenadas (posEnX, posEnY);

    int tipo;
    tipo = numeroRandomEnRango(0,1);

    tipoProvision queProvision;
    if (tipo == 0)
        queProvision = VIDA;
    else
        queProvision = MUNICION;


    bool esTrampa;
    int calculoSiTrampa;
    calculoSiTrampa = numeroRandomEnRango(0,10);
    if (calculoSiTrampa == 0)
        esTrampa = true;
    else
        esTrampa = false;

    this->anadirProvision(coordenadas, queProvision, esTrampa);
}


Proyectil *Partida::proyectilConstructor() {
    b2Vec2 origen(0,0);

    Proyectil * nuevoProyectil = new Proyectil();
    nuevoProyectil->armaOrigen = NADA_P;
    nuevoProyectil->id = this->proyectiles.size();
    nuevoProyectil->tipo = TipoProyectil::Ningun;

    nuevoProyectil->countdown = 0;
    nuevoProyectil->enElAire = false;
    nuevoProyectil->colisiono = false;

    Entidad *nuevaEntidad = new Entidad;
    nuevaEntidad->tipo = TipoEntidad::PROYECTILREAL;
    nuevaEntidad->proyectilReal = nuevoProyectil;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.userData.pointer = reinterpret_cast<uintptr_t> (nuevaEntidad);

    b2Body* body = world.CreateBody(&bodyDef);

    b2FixtureDef fixtureDef;
    b2CircleShape circleShape;
    circleShape.m_radius = 0.05; // very small
  
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 3.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.2;
    fixtureDef.filter.groupIndex = -1;
    fixtureDef.filter.categoryBits = (uint16_t)TipoEntidad::PROYECTILREAL;
    fixtureDef.filter.maskBits = (uint16_t)TipoEntidad::VIGA | (uint16_t)TipoEntidad::OCEANO | (uint16_t)TipoEntidad::GUSANO;

    nuevoProyectil->fixture = body->CreateFixture(&fixtureDef);
    nuevoProyectil->cuerpo = body;
    nuevoProyectil->exploto = false;

    this->proyectiles.push_back(nuevoProyectil);

    return nuevoProyectil;
}

void Partida::cambiarElViento(time_t tiempoActual) {
    double diferencia;
    diferencia = difftime(tiempoActual, this->ultimoCambioViento);

    if (diferencia < config.esperaViento)
        return;

    //Actualizo la hora incluso si el coin flip no lo logra.
    //Es para que SIEMPRE haya TIEMPOEESPERAVIENTO de tiempo entre
    //intento e intento
    this->ultimoCambioViento = tiempoActual;

    int cointFlip = numeroRandomEnRango(0,1);

    if (cointFlip == 0)
        return;

    int maxRandom;
    maxRandom = config.maxViento - config.minViento;

    int nuevoViento = numeroRandomEnRango(0, maxRandom);
    nuevoViento = nuevoViento + config.minViento;
    
    this->viento.x = nuevoViento;

}


bool destruirProyectil(b2Body *proyectil) {
    bool expirado;

    Entidad *entidad = (Entidad *) proyectil->GetUserData().pointer;
    time_t horaDeCreacion;
    horaDeCreacion = entidad->proyectil.horaDeCreacion;
    time_t horaActual;
    horaActual = time(NOW);
    double diferencia;
    diferencia = difftime(horaActual, horaDeCreacion);

    expirado = diferencia > entidad->proyectil.tiempoMinimoDeVida;

    return expirado;
}

void Partida::borrarCuerpos() {
    //Borro todos los cuerpos a destruir
    //Leo la lista al reves para no tener problemas del offset al
    //borrar elementos

    for(int i = this->cuerposADestruir.size() - 1 ; i >= 0 ; i--) {
        b2Body *cuerpoABorrar;
        cuerpoABorrar = this->cuerposADestruir.at(i);
        bool loBorro;
        loBorro = destruirProyectil(cuerpoABorrar);
        // NO HACER delete entidad. Tira invalid delete
        if (loBorro == true) {
	  Entidad *entidadB = (Entidad *) cuerpoABorrar->GetUserData().pointer;
	  this->world.DestroyBody(cuerpoABorrar);
	  this->cuerposADestruir.erase(this->cuerposADestruir.begin() + i);

	  for (Gusano *gusano : this->gusanos) {
	      gusano->golpeado = false;
	  }
	  delete entidadB->proyectilReal;
	  delete entidadB;
        }

    }

    for(int i = this->proyectiles.size() - 1 ; i >= 0 ; i--) {
        Proyectil *proyectil = this->proyectiles[i];
        b2Body *cuerpoABorrar = proyectil->cuerpo;
        // NO HACER delete entidad. Tira invalid delete
        if (proyectil->exploto) {
            Entidad *entidadB = (Entidad *) cuerpoABorrar->GetUserData().pointer;
            this->world.DestroyBody(cuerpoABorrar);
            delete entidadB->proyectilReal;
            this->proyectiles.erase(this->proyectiles.begin() + i);
	  for (Gusano *gusano : this->gusanos) {
	      gusano->golpeado = false;
	  }

	  delete entidadB;
        }

    }

    for(int i = this->provisiones.size() - 1 ; i >= 0 ; i--) {
        Provision *provision = this->provisiones[i];
        b2Body *cuerpoABorrar = provision->cuerpo;
        // NO HACER delete entidad. Tira invalid delete
        if (provision->fueAgarrada == true && provision->esTrampa == true && provision->exploto == false) {
	  Proyectil *dinamita = new Proyectil();

	  dinamita->armaOrigen = GRANADA_VERDE_P;
	  dinamita->tipo = TipoProyectil::Countdown;
	  //GOTCHA No se lo voy a enviar, este id es basura
	  dinamita->id = -2;
	  dinamita->cuerpo = provision->cuerpo;
	  dinamita->exploto = false;
	  dinamita->colisiono = true;
	  dinamita->esFragmento = false;
	  dinamita->countdown = 2;

	  provision->miProyectil = dinamita;
	  Ataque ataqueProvision;
	  ataqueProvision.posicion = provision->cuerpo->GetPosition();
	  ataqueProvision.posicion.y += 1;
	  ataqueProvision.potencia = 100;
	  ataqueProvision.tiempoEspera = 8;
	  ataqueProvision.arma = DINAMITA_P;
	  ataqueProvision.impulsoInicial = b2Vec2(0,0);
	  ataqueProvision.proyectilAsociado = dinamita;

	  this->crearProyectiles(nullptr, ataqueProvision);
	  provision->exploto = true;
        }
        else if (provision->fueAgarrada == true && provision->esTrampa == false) {
            Entidad *entidadB = (Entidad *) cuerpoABorrar->GetUserData().pointer;
            this->world.DestroyBody(cuerpoABorrar);
            delete entidadB->provision;
            delete entidadB;
            this->provisiones.erase(this->provisiones.begin() + i);
        }
        else if (provision->fueAgarrada == true && provision->esTrampa == true && provision->exploto == true) {
            Entidad *entidadB = (Entidad *) cuerpoABorrar->GetUserData().pointer;
            this->world.DestroyBody(cuerpoABorrar);
            delete entidadB->provision->miProyectil;
            delete entidadB->provision;
            delete entidadB;
            this->provisiones.erase(this->provisiones.begin() + i);
	  
        }


    }

}


//Fisher-Yates shuffle:
//https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
//https://github.com/lima-limon-inc/Blackjack-basic-strategy/blob/main/cards.c#L55-L68
void mezclarPosiciones(std::vector<coordsGusanos>& posiciones) {
	srand(time(NULL));   // Initialization, should only be called once.
	int rankNumer, randomPosition;
	coordsGusanos tmp;
    int size = posiciones.size();
	for (rankNumer = size - 1; rankNumer > 0; rankNumer--) {
		randomPosition = rand() % (rankNumer + 1);
		tmp = posiciones[randomPosition];
		posiciones[randomPosition] = posiciones[rankNumer];
		posiciones[rankNumer] = tmp;
	}
}


void Partida::gameLoop() {
    //Esperamos hasta que se una 1 jugador
    std::unique_lock<std::mutex> lck(mtx);
    while (this->clientes.size() < 1)
        this->seUnioJugador.wait(lck);
    
    this->momento = ESPERANDO;

    Accion accionRecibida;
    accionRecibida.idGusano = INVAL_ID;
    accionRecibida.esEmpezar = false;
    while (this->momento != EN_MARCHA && !this->finPartida) {
        this->finPartida = NOT this->enviarEstadoAJugadores();
        if (this->finPartida) {
            this->momento = TERMINADA;
            return;
        }

        std::this_thread::sleep_for(frameDuration);

        bool pudeObtenerla;
        pudeObtenerla = acciones.try_pop(accionRecibida);

        if (pudeObtenerla == true && accionRecibida.accion == CHEAT
	  && accionRecibida.cheat == ARRANCAR_C)
	  this->momento = EN_MARCHA;

        if (this->clientes.size() < MINJUGADORES)
            continue;
        this->momento = POR_INICIAR;

        if (pudeObtenerla == true) {
	  if ((accionRecibida.esEmpezar) ||
	      (accionRecibida.accion == CHEAT && accionRecibida.cheat == ARRANCAR_C)) {
	      this->momento = EN_MARCHA;
	  }
        }
    }

    bool cantJusta = this->mapaUsado.cantGusanos % this->jugadores.size() == 0;
    int cantGusanos = this->mapaUsado.cantGusanos / this->jugadores.size();
    std::vector<coordsGusanos> posiciones = this->mapaUsado.posicionGusanos;
    mezclarPosiciones(posiciones);
    if (cantJusta) {
        for (auto &&jugador : this->jugadores) {
            for (int i = 0; i < cantGusanos; i++, cantidad_gusanos_insertados++) {
                
                Gusano *nuevoGusano = this->anadirGusano(posiciones.at(cantidad_gusanos_insertados));

                jugador->anadirGusano(nuevoGusano);
            }
        }
        
    }  else {
        for (auto &&jugador : this->jugadores) {
            std::vector<Gusano*> gusanosJugador;
            for (int i = 0; i < cantGusanos; i++, cantidad_gusanos_insertados++) {
                
                Gusano *nuevoGusano = this->anadirGusano(posiciones.at(cantidad_gusanos_insertados));

                jugador->anadirGusano(nuevoGusano);
            }
        }

        int indexJugador = 0;
        for (; cantidad_gusanos_insertados < this->mapaUsado.cantGusanos; cantidad_gusanos_insertados++, indexJugador++) {
            Jugador *jugadorActual = this->jugadores[indexJugador];
            Gusano *nuevoGusano = this->anadirGusano(posiciones.at(cantidad_gusanos_insertados));
            jugadorActual->anadirGusano(nuevoGusano);
        }

        for (;indexJugador < (int)this->jugadores.size(); indexJugador++) {
            Jugador *jugadorActual = this->jugadores[indexJugador];
            jugadorActual->darMasVidaAGusanos();
        }
    }
    float timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;


    bool exploto = false;

    Ataque ataqueARealizar;

    time_t tiempoActual;
    tiempoActual = time(NOW);

    Jugador *jugadorActual = nullptr;
    jugadorActual = this->siguienteJugador(NULL);

    Gusano *gusanoActual = nullptr;
    gusanoActual = jugadorActual->getGusanoDeTurno();
    gusanoActual->esMiTurno(tiempoActual);


    while (this->finPartida == false) {
        tiempoActual = time(NOW);

        this->generarProvision(tiempoActual);
        
        this->cambiarElViento(tiempoActual);
        
        std::pair<Gusano *, Jugador *> gusanoYJugador;
        gusanoYJugador = this->cambiarDeJugador(jugadorActual, gusanoActual, tiempoActual);
        // se quedo sin gusanos posibles para jugar
        if (gusanoYJugador.first == nullptr) {
            break;
        }
        gusanoActual = gusanoYJugador.first;
        jugadorActual = gusanoYJugador.second;

        //Fabri was here
        this->finPartida = NOT this->enviarEstadoAJugadores();
        if (this->finPartida == true)
	        break;

        this->borrarCuerpos();

        this->world.Step(timeStep, velocityIterations, positionIterations);

        Accion accionRecibida;
        accionRecibida.idGusano = INVAL_ID;
        bool pudeObtenerla;
        pudeObtenerla = acciones.try_pop(accionRecibida);

        Accion accionAEjecutar;
        accionAEjecutar = this->obtenerAccion(accionRecibida, pudeObtenerla,
				      gusanoActual);

        if (accionAEjecutar.accion == CHEAT)
	  this->procesarCheats(accionAEjecutar, gusanoActual, jugadorActual);
        else
	  ataqueARealizar = gusanoActual->ejecutar(accionAEjecutar);

        for (auto &&proyectil : this->proyectiles) {
            if (proyectil->tipo == TipoProyectil::Countdown) {
	      proyectil->countdown -= 1;
            }
            this->generarExplosion(proyectil);
        }
            
        this->crearProyectiles(gusanoActual, ataqueARealizar);

        std::this_thread::sleep_for(frameDuration);
    }

}

MomentoDePartida Partida::getMomentoDePartida() {
    return this->momento;
}


Partida::~Partida() {
    this->finPartida = true;
    this->colisiones.finPartida = true;

    if (!this->acciones.is_closed()) {
        this->acciones.close();
    }

    for ( b2Body* b = world.GetBodyList(); b != NULL; b = b->GetNext()) {

        Entidad *entidadB = (Entidad *) b->GetUserData().pointer;
        delete entidadB;
        world.DestroyBody(b);
    }

    for (auto &&gusano : this->gusanos) {
        delete gusano;
    }

    for (auto &&jugador : this->jugadores) {
        delete jugador;
    }

    for (auto &&proyectil : this->proyectiles) {
        b2Body *cuerpo;
        cuerpo = proyectil->cuerpo;
        Entidad *entidadB = (Entidad *) cuerpo->GetUserData().pointer;
        delete entidadB;
        delete proyectil;
    }

    for (Provision *provision : this->provisiones) {
        if (provision->esTrampa) {
            delete provision->miProyectil;
        }
        delete provision;
    }
}

void Partida::stop() {
    this->finPartida = true;
    this->colisiones.finPartida = true;
}


bool Partida::estaCompleta(){
    return this->clientes.size() >= this->mapaUsado.posicionGusanos.size();
}
