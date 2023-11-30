#include "partida.h"
#include <map>
#include "box2dDefs.h"

#define SLEEPSEGS 1
#define NOW NULL

int esperar = 0;

const std::chrono::duration<double> frameDuration(1.0 / 30);

Partida::Partida(std::string mapa)
    :world(b2Vec2(FUERZAGRAVITARIAX, FUERZAGRAVITARIAY)){
    this->mapa = mapa;
    this->world.SetContactListener(&this->colisiones);
    this->posJugadorActual = -1;
    this->finPartida = false;
    this->colisiones.finPartida = false;
    this->dimensiones = std::pair<coordX, coordY>(MAXALTURA,40);
    this->termino = false;
    this->momento = ESPERANDO;


    this->anadirViga(0, LONGITUDVIGAGRANDE, std::pair<coordX,coordY>(5.0f, 17.0f));
    this->anadirViga(M_PI/2, LONGITUDVIGAGRANDE, std::pair<coordX,coordY>(30.0f, 17.0f));
    this->anadirViga(M_PI/2, LONGITUDVIGAGRANDE, std::pair<coordX,coordY>(30.0f, 23.0f));
    this->anadirViga(0, LONGITUDVIGAGRANDE, std::pair<coordX,coordY>(27.0f, 17.0f));
    
    this->anadirViga(0, LONGITUDVIGAGRANDE, std::pair<coordX,coordY>(5.0f, 10.0f));
    this->anadirViga(0, LONGITUDVIGAGRANDE, std::pair<coordX,coordY>(12.0f, 10.0f));
    this->anadirViga(0, LONGITUDVIGAGRANDE, std::pair<coordX,coordY>(25.0f, 10.0f));
    this->anadirViga(0, LONGITUDVIGAGRANDE, std::pair<coordX,coordY>(50.0f, 10.0f));
    this->anadirViga(0, LONGITUDVIGAGRANDE, std::pair<coordX,coordY>(60.0f, 10.0f));
    this->anadirViga(0, LONGITUDVIGAGRANDE, std::pair<coordX,coordY>(70.0f, 10.0f));
    
    this->anadirViga(0, LONGITUDVIGACHICA, std::pair<coordX,coordY>(10.0f, 15.0f));
    this->anadirViga(0, LONGITUDVIGACHICA, std::pair<coordX,coordY>(65.0f, 15.0f));

    // this->anadirViga(M_PI/8, LONGITUDVIGAGRANDE, std::pair<coordX,coordY>(30.0f, 17.0f));
    this->anadirViga(-M_PI/8, LONGITUDVIGAGRANDE, std::pair<coordX,coordY>(45.0f, 17.0f));
    
    this->anadirViga(0, LONGITUDVIGAGRANDE, std::pair<coordX,coordY>(37.5f, 18.0f));

    this->anadirViga(0, LONGITUDVIGACHICA, std::pair<coordX,coordY>(27.0f, 13.0f));
    this->anadirViga(0, LONGITUDVIGACHICA, std::pair<coordX,coordY>(48.0f, 13.0f));

    this->posicionesGusanos.insert({0, std::pair<coordX, coordY>(5.0f, 19.0f)});
    this->posicionesGusanos.insert({1, std::pair<coordX, coordY>(28.5f, 19.0f)});
    this->posicionesGusanos.insert({2, std::pair<coordX, coordY>(28.5f, 19.0f)});
    this->posicionesGusanos.insert({3, std::pair<coordX, coordY>(50.0f, 19.0f)});
    this->posicionesGusanos.insert({4, std::pair<coordX, coordY>(60.0f, 19.0f)});
    this->posicionesGusanos.insert({5, std::pair<coordX, coordY>(70.0f, 19.0f)});

    this->cantidad_gusanos_insertados = 0;

    this->anadirOceano(std::pair<coordX, coordY>(0.0f, 0.0f));
}

//Esto tendria que estar en el YAML? Posiblemente
#define CANTGUSANOS 3

// Usado para castear un puntero a una reference y hacer
// el codigo mas explicito
#define REFERENCE *

void ResolvedorColisiones::BeginContact(b2Contact *contact) {
    // std::cout << "CONTACTO\n";
    
    b2Body* cuerpoA = contact->GetFixtureA()->GetBody();
    b2Body* cuerpoB = contact->GetFixtureB()->GetBody();

    Entidad *entidadA = (Entidad *) cuerpoA->GetUserData().pointer;
    Entidad *entidadB = (Entidad *) cuerpoB->GetUserData().pointer;

    if (entidadB->tipo == TipoEntidad::PROYECTIL
        &&
        entidadA->tipo == TipoEntidad::GUSANO) {
        b2Vec2 dir = cuerpoB->GetLinearVelocity();
        // cuerpoA->ApplyLinearImpulseToCenter(dir, true);
        // printf("A\n");
        entidadA->gusano->recibirDano(dir, entidadB);
        // abort();
    }

    else if (entidadA->tipo == TipoEntidad::PROYECTIL
        &&
        entidadB->tipo == TipoEntidad::GUSANO) {
        b2Vec2 dir = cuerpoA->GetLinearVelocity();
        // std::cout << "SORETE SORETE " << entidadA->proyectil.posInicial.x << " " << entidadA->proyectil.posInicial.y << "\n";
        // printf("B\n");
        entidadB->gusano->recibirDano(dir, entidadA);
        // abort();
    }

    else if(entidadA->tipo == TipoEntidad::VIGA
        &&
        entidadB->tipo == TipoEntidad::GUSANO) {
        std::cout << "Viga: " << cuerpoA->GetPosition().y << "\n";
        entidadB->gusano->recibirDanioCaida(cuerpoB->GetLinearVelocity());
        // entidadB->gusano->setEstado(QUIETO);
    }

    else if(entidadB->tipo == TipoEntidad::VIGA 
        &&
        entidadA->tipo == TipoEntidad::GUSANO) {
        // float tiempoParaMetrosMax = std::sqrt(METROS_SIN_DANIO*2/(-FUERZAGRAVITARIAY)); 
        // float velocidadMax = -METROS_SIN_DANIO*2/tiempoParaMetrosMax;
        // if (cuerpoB->GetLinearVelocity().y < velocidadMax) {
        //     std::cout << "DANIO POR CAIDA " << velocidadMax << "\n";
        // }
        std::cout << "Viga: " << cuerpoB->GetPosition().y << "\n";
        entidadA->gusano->recibirDanioCaida(cuerpoA->GetLinearVelocity());
        // entidadA->gusano->setEstado(QUIETO);
    }

    else if (entidadA->tipo == TipoEntidad::GUSANO
        &&
        entidadB->tipo == TipoEntidad::ARMA) {

        cuerpoA->ApplyLinearImpulseToCenter(b2Vec2(100.0f, 1000.0f), true);
  }
      
    else if (entidadA->tipo == TipoEntidad::OCEANO
        &&
        entidadB->tipo == TipoEntidad::GUSANO) {
        // b2Vec2 dir = cuerpoA->GetLinearVelocity();
        entidadB->gusano->setEstado(AHOGADO);
        printf("AHOGADO\n");
        // abort();
    }



    //TODO Cambiar estos dos a que no sea solo con viga, sino cualquiera
    else if (entidadA->tipo == TipoEntidad::PROYECTILREAL
	   &&
	   entidadB->tipo == TipoEntidad::VIGA) {
        printf("PROYECTIL REAL A\n");
        entidadA->proyectilReal->enElAire = false;
        entidadA->proyectilReal->colisiono = true;
    }

    else if (entidadB->tipo == TipoEntidad::PROYECTILREAL
	   &&
	   entidadA->tipo == TipoEntidad::VIGA) {
        printf("PROYECTIL REAL B\n");
        entidadB->proyectilReal->enElAire = false;
        entidadB->proyectilReal->colisiono = true;
    }
}

void ResolvedorColisiones::EndContact(b2Contact *contact) {
    //Esto nos evita reads invalidos en caso de que el programa haya
    //terminado
    if (this->finPartida == true)
        return;

    b2Body* cuerpoA = contact->GetFixtureA()->GetBody();
    b2Body* cuerpoB = contact->GetFixtureB()->GetBody();

    Entidad *entidadA = (Entidad *) cuerpoA->GetUserData().pointer;
    Entidad *entidadB = (Entidad *) cuerpoB->GetUserData().pointer;

    if (entidadB->tipo == TipoEntidad::PROYECTIL
        &&
        entidadA->tipo == TipoEntidad::GUSANO) {
        printf("A\n");
        // abort();
    }

    if (entidadA->tipo == TipoEntidad::PROYECTIL
        &&
        entidadB->tipo == TipoEntidad::GUSANO) {
        printf("B\n");
        // abort();
    }

    if(entidadA->tipo == TipoEntidad::VIGA
       &&
       entidadB->tipo == TipoEntidad::GUSANO) {
        if (entidadB->gusano->getEstado() != SALTANDO &&
            entidadB->gusano->getEstado() != HACE_PIRUETA &&
            entidadB->gusano->getEstado() != HERIDO) {
            entidadB->gusano->setEstado(CAYENDO);
        }
    }

    if(entidadB->tipo == TipoEntidad::VIGA
       &&
       entidadA->tipo == TipoEntidad::GUSANO) {
        if (entidadA->gusano->getEstado() != SALTANDO &&
            entidadA->gusano->getEstado() != HACE_PIRUETA &&
            entidadA->gusano->getEstado() != HERIDO) {
            entidadA->gusano->setEstado(CAYENDO);
        }
    }
    // std::cout << "FIN CONTACTO\n";
}


bool ResolvedorQuery::ReportFixture(b2Fixture* fixture) {
    foundBodies.push_back( fixture->GetBody() ); 
    return true;//keep going to find all fixtures in the query area
}



Gusano *Partida::anadirGusano(std::pair<coordX, coordY> coords) {
    Entidad *nuevaEntidad = new Entidad;
    nuevaEntidad->tipo = TipoEntidad::GUSANO;
    Gusano *nuevoGusano = new Gusano();
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
    fixtureDef.filter.maskBits = (uint16_t)TipoEntidad::VIGA | (uint16_t)TipoEntidad::OCEANO | (uint16_t)TipoEntidad::PROYECTIL;

    body->CreateFixture(&fixtureDef);
    nuevoGusano->setCuerpo(body);

    this->gusanos.push_back(nuevoGusano);
    int idGusano;
    idGusano = this->gusanos.size() - 1;
    nuevoGusano->giveId(idGusano);

    return nuevoGusano;
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
    // Juampi: puede que ancho viga tambien tenga que ser dividido por 2?
    viga.SetAsBox(longitud, ANCHOVIGA / 2);
    //viga.SetAsBox(longitud, ANCHOVIGA);

    b2Body* groundBody = world.CreateBody(&vigaDef);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &viga;
    fixtureDef.density = MASACUERPOESTATICO;

    fixtureDef.filter.categoryBits = (uint16_t)TipoEntidad::VIGA;
    fixtureDef.filter.maskBits = 1;

    groundBody->CreateFixture(&fixtureDef);
    // groundBody->CreateFixture(&viga, MASACUERPOESTATICO);

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
    fixtureDef.filter.maskBits = 1;

    oceanoCuerpo->CreateFixture(&fixtureDef);
    // oceanoCuerpo->CreateFixture(&oceano, MASACUERPOESTATICO);

}


InformacionInicial Partida::obtenerInfoInicial() {
    InformacionInicial infoInicial;

    std::vector<Gusano*> gusanosParaElNuevoJugador;
    //Todos los gusanos que creamos lo anadimos al jugador y a la partida
    for (int i = 0; i < CANTGUSANOS; i++) {
              
        Gusano *nuevoGusano = this->anadirGusano(posicionesGusanos.at((i + cantidad_gusanos_insertados) % posicionesGusanos.size()));

        gusanosParaElNuevoJugador.push_back(nuevoGusano);
        cantidad_gusanos_insertados += 1;

    }
    //Le damos los gusanos al jugador del cliente y acceso a la queue
    //de acciones
    Jugador *jugadorNuevo = new Jugador(gusanosParaElNuevoJugador);
    this->jugadores.push_back(jugadorNuevo);

    //Creo el id del nuevo jugador
    idJugador idNuevoJugador;
    idNuevoJugador = (idJugador) this->jugadores.size();
    infoInicial.jugador = idNuevoJugador;

    infoInicial.vigas = this->vigasEnMapa;
    infoInicial.dimensiones = this->dimensiones;


    return infoInicial;
}

void Partida::anadirCliente(Cliente *clienteNuevo) {
    std::cout << "Nuevo cliente\n";
    clienteNuevo->obtenerAccesoAAcciones(&this->acciones);

    //Anadimos al jugador a la partida
    this->clientes.push_back(clienteNuevo);

    this->enviarEstadoAJugadores();

    //Aviso que se unio un jugador
    this->seUnioJugador.notify_all();
}

bool Partida::enviarEstadoAJugadores() {
    std::shared_ptr<EstadoDelJuego> estadoActual(new EstadoDelJuego);

    std::map<idJugador, std::map<id, RepresentacionGusano>> representacionGusanos;
    std::map<idJugador, SituacionJugador> situaciones;

    for (int jugador = 0; jugador < (int) this->jugadores.size() ; jugador++) {
        Jugador *jugadorActual;
        jugadorActual = this->jugadores.at(jugador);

        if (jugador == this->posJugadorActual) {
	  estadoActual->jugadorDeTurno = jugador;
	  estadoActual->gusanoDeTurno = jugadorActual->getGusanoDeTurno()->getId();
	  estadoActual->segundosRestantes = jugadorActual->getGusanoDeTurno()->getTiempoQueMeQueda();
	//   std::cout << "TIEMPO: " << estadoActual->segundosRestantes << "\n";
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
        //TODO: Hack momentaneo
        if (
	  proyectil->armaOrigen == NADA_P
	  ||
	  proyectil->armaOrigen == TELETRANSPORTACION_P
	  ||
	  proyectil->armaOrigen == BATE_P
	  )
	  continue;
        RepresentacionProyectil repre;
        repre.id = proyectil->id;
        repre.proyectil = proyectil->armaOrigen;
        repre.esFragmento = false;

        // std::cout << "COORDS:" << proyectil->cuerpo->GetPosition().x << proyectil->cuerpo->GetPosition().y << "\n";
        repre.posicion = deb2VecACoord(proyectil->cuerpo->GetPosition());
        
        b2Vec2 velocidad = proyectil->cuerpo->GetLinearVelocity();
        if (velocidad.y > 0) {
            repre.angulo = std::atan(velocidad.y/velocidad.x);
            repre.angulo = M_PI/2 - repre.angulo;
        } else {
            repre.angulo = std::atan(-velocidad.y/velocidad.x);
            repre.angulo += M_PI/2;
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

    bool hayJugadores = false;
    for(Cliente *cliente : this->clientes) {
        if (cliente != nullptr && !cliente->estaMuerto()) {
            cliente->enviarEstadoJuego(estadoActual);
            hayJugadores = true;
        }
    }
    return hayJugadores;
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
        // ultimaAccion = accionAEjecutar;
    }
    //Si entra en estos otros if es porque NO se obtuvo algo nuevo
    else if (tipoUltimaAccion == MOVERSE &&
        (ultimaAccion.dir != SALTO &&
        ultimaAccion.dir != PIRUETA)
    ) {
        accionAEjecutar = ultimaAccion;
    }
    else if (tipoUltimaAccion == ATAQUE) {
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

    // printf("KATAPUM\n");
    //Fuente: https://www.iforce2d.net/b2dtut/explosions
    int numRays = 32;
    for (int i = 0; i < numRays; i++) {
        // std::cout << i << "\n";
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
        bd.linearVelocity = 60 * rayDir;
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
    fd.filter.maskBits = (uint16_t)TipoEntidad::VIGA | (uint16_t)TipoEntidad::OCEANO | (uint16_t)TipoEntidad::GUSANO;
        body->CreateFixture( &fd );
        nuevaEntidad->proyectil.proyectil = body;
        nuevaEntidad->proyectil.horaDeCreacion = time(NOW);
        nuevaEntidad->proyectil.tiempoMinimoDeVida = 0.5f;

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
    // printf("FIN KATAPUM\n");
}


// proyectil = nuevoProyectil
void Partida::crearProyectiles(Gusano *gusano, Ataque ataque) {
    for (int i = 0; i<2; i++) {
    ArmaProtocolo arma = ataque.arma;
    if (arma == NADA_P) {
        return;
    }
    Proyectil *nuevoProyectil = this->proyectilConstructor();
    

    nuevoProyectil->armaOrigen = arma;
    nuevoProyectil->exploto = false;
    nuevoProyectil->colisiono = false;
    nuevoProyectil->countdown = ataque.tiempoEspera;

    if (arma == DINAMITA_P || arma == GRANADA_VERDE_P || arma == GRANADA_SANTA_P || arma == BANANA_P) {
        nuevoProyectil->tipo = TipoProyectil::Countdown;
    }
    else if (arma == BAZOOKA_P || arma == ATAQUE_AEREO_P) {
        nuevoProyectil->tipo = TipoProyectil::Colision;
    } else {
        nuevoProyectil->tipo = TipoProyectil::Ningun;
    }

    if (arma == BATE_P) {
        std::cout << "ESTOY ACA\n";
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
    
     

      entidadA->gusano->recibirDano(golpe, nuevaEntidad);
	//   entidadA->gusano->recibirDano(golpe, entidadA);

	  // printf("PEGO\n");
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
        std::cout << "POSITION: " << ataque.posicion.x << " " << ataque.posicion.y << "\n";


    }

    ataque.posicion.y += 3 * i;
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
    bool todoExploto;
    for (auto &&proyectil : this->proyectiles) {
        if (!proyectil->exploto) {
            return false;
        }
    }
    
    // todoExploto = (proyectil->countdown <= 0);
    // if (todoExploto == false)
    //     return false;

    bool todoEstaQuieto = true;
    for (int i = 0; (i < (int) this->clientes.size())
		&&
		//Apenas sea false, no quiero seguir iterando
		(todoEstaQuieto == true);
         i++) {
        Gusano *gusano;
        gusano = this->gusanos.at(i);

        bool gusanoEstaQuieto;
        gusanoEstaQuieto = gusano->estaQuieto();

        //Con que uno de estos sea false, ya te hace el valor false
        todoEstaQuieto = todoEstaQuieto && gusanoEstaQuieto;
    }
    if (todoEstaQuieto == false)
        return false;




    //Si llegue hasta aca, significa que todas las otras condiciones
    //se cumplieron, entonces se que se puede cambiar de turno
    return true;

}

std::pair<Gusano *, Jugador *> Partida::cambiarDeJugador(Jugador *jugadorTurnoActual, Gusano *gusanoActual, time_t tiempoActual) {
    std::pair<Gusano *, Jugador *> gusanoYJugador;
    Gusano *gusanoDeTurno = gusanoActual;
    Jugador *jugadorDeTurno = jugadorTurnoActual;
    gusanoYJugador.first = gusanoActual;
    gusanoYJugador.second = jugadorDeTurno;

    // std::cout << "Exploto: " << std::boolalpha << todoExploto << "\n";

    // std::cout << "Todo quieto: " << std::boolalpha << todoEstaQuieto << "\n";

    

    // std::cout << "Fin gusano actual" << std::boolalpha << finDelGusano << "\n";

    bool cambioDeTurno;
    // cambioDeTurno = (todoExploto &&
    // 		 todoEstaQuieto &&
    // 		 finDelGusano);

    cambioDeTurno = this->sePuedeCambiarDeJugador(gusanoActual, tiempoActual);

    //Si no hay cambio de turno, devolvemos el mismo gusano
    if (cambioDeTurno == false)
        return gusanoYJugador;

    // std::cout << "PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP\n";

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

        SituacionJugador situacion;
        situacion = jugadorAChequear->getSituacion();
        bool jugadorPerdio;
        jugadorPerdio = (situacion == PERDISTE);

        //Con que uno de estos sea false, ya te hace el valorFalse
        elProximoGano = elProximoGano && jugadorPerdio;
    }

    if (elProximoGano == true) {
        std::cout << "TENEMOS UN GANADOR" << "\n";
        jugadorDeTurno->avisarQueGane();
        this->termino = true;
        this->momento = TERMINADA;
    }

    return gusanoYJugador;
}


Proyectil *Partida::proyectilConstructor() {
    b2Vec2 origen(0,0);

    Proyectil * nuevoProyectil = new Proyectil();
    nuevoProyectil->armaOrigen = NADA_P;
    // nuevoProyectil->posicion = origen;
    nuevoProyectil->id = 0;
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
    // fixtureDef.filter.categoryBits = (uint16_t)TipoEntidad::PROYECTILREAL;
    // fixtureDef.filter.maskBits = (uint16_t)TipoEntidad::VIGA | (uint16_t)TipoEntidad::OCEANO | (uint16_t)TipoEntidad::GUSANO;
    // fixtureDef.filter.categoryBits = (uint16_t)TipoEntidad::PROYECTIL;
    // fixtureDef.filter.maskBits = (uint16_t)TipoEntidad::OCEANO | (uint16_t)TipoEntidad::GUSANO;

    nuevoProyectil->fixture = body->CreateFixture(&fixtureDef);

    nuevoProyectil->cuerpo = body;


    nuevoProyectil->exploto = false;
    this->proyectiles.push_back(nuevoProyectil);

    return nuevoProyectil;
}



bool destruirProyectil(b2Body *proyectil) {
    Entidad *entidad = (Entidad *) proyectil->GetUserData().pointer;
    time_t horaDeCreacion;
    horaDeCreacion = entidad->proyectil.horaDeCreacion;
    time_t horaActual;
    horaActual = time(NOW);
    double diferencia;
    diferencia = difftime(horaActual, horaDeCreacion);

    bool expirado;
    expirado = diferencia > entidad->proyectil.tiempoMinimoDeVida;

    return expirado;
}

void Partida::borrarCuerpos() {
    //Borro todos los cuerpos a destruir
    //Leo la lista al reves para no tener problemas del offset al
    //borrar elementos

    // std::cout << "AHORA CUERPOS\n";
    for(int i = this->cuerposADestruir.size() - 1 ; i >= 0 ; i--) {
        b2Body *cuerpoABorrar;
        cuerpoABorrar = this->cuerposADestruir.at(i);
        bool loBorro;
        loBorro = destruirProyectil(cuerpoABorrar);
        // NO HACER delete entidad. Tira invalid delete
        if (loBorro == true) {
	  // std::cout << "Delete\n";
	  Entidad *entidadB = (Entidad *) cuerpoABorrar->GetUserData().pointer;
	//   delete entidadB;
	  this->world.DestroyBody(cuerpoABorrar);
	  this->cuerposADestruir.erase(this->cuerposADestruir.begin() + i);
        }

    }

    // std::cout << "AHORA PROYECTILES\n";
    for(int i = this->proyectiles.size() - 1 ; i >= 0 ; i--) {
        Proyectil *proyectil = this->proyectiles[i];
        b2Body *cuerpoABorrar = proyectil->cuerpo;
        // NO HACER delete entidad. Tira invalid delete
        if (proyectil->exploto && (proyectil->countdown < 0 || proyectil->colisiono)) {
            // std::cout << "Delete\n";
            Entidad *entidadB = (Entidad *) cuerpoABorrar->GetUserData().pointer;
            // delete entidadB;
            this->world.DestroyBody(cuerpoABorrar);
            this->proyectiles.erase(this->proyectiles.begin() + i);
        }

    }

    for (Gusano *gusano : this->gusanos) {
        gusano->golpeado = false;
    }
}

void Partida::gameLoop() {
    std::unique_lock<std::mutex> lck(mtx);

    //Esperamos hasta que se unan todos los jugadores necesarios
    while (this->clientes.size() < MINJUGADORES)
        this->seUnioJugador.wait(lck);
    
    this->momento = POR_INICIAR;

    while (this->finPartida == false) {
        //FIXME Esto estaria bueno, pero tira excepcion :(
        // if (this->clientes.size() >= MINJUGADORES) {
        // 	  this->momento = POR_INICIAR;
        // }

        this->finPartida = NOT this->enviarEstadoAJugadores();
        if (this->finPartida) {
            break;
        }
        Accion accionRecibida;
        accionRecibida.idGusano = INVAL_ID;
        bool pudeObtenerla;
        pudeObtenerla = acciones.try_pop(accionRecibida);

        if (accionRecibida.esEmpezar && pudeObtenerla) {
            this->momento = EN_MARCHA;
            break;
        }
        std::this_thread::sleep_for(frameDuration);
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
        delete proyectil;
    }

    
}

void Partida::stop() {
    this->finPartida = true;
    this->colisiones.finPartida = true;
}
