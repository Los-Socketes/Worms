#include "partida.h"

#include "box2dDefs.h"

#include <map>
#include <random>

#define SLEEPSEGS 1
#define NOW NULL

const std::chrono::duration<double> frameDuration(1.0 / 30);

Partida::Partida(std::string mapa)
    :world(b2Vec2(FUERZAGRAVITARIAX, FUERZAGRAVITARIAY)){
    this->mapa = mapa;
    this->world.SetContactListener(&this->colisiones);
    this->posJugadorActual = -1;
    this->finPartida = false;
    this->colisiones.finPartida = false;
    this->dimensiones = std::pair<coordX, coordY>(MAXALTURA, MAXANCHO);
    this->termino = false;
    this->momento = ESPERANDO;
    this->ultimaProvision = time(NOW);

    // TODO: que reciba el mapa por parametro
    Mapas mapas;
    this->mapaUsado = mapas.mapas[0];
    for (auto &&viga : this->mapaUsado.vigas) {
        this->anadirViga(viga.angulo, viga.tamanio, viga.coordenadas);
    }

    for (int i = 0; i < (int)this->mapaUsado.posicionGusanos.size(); i++) {
        this->posicionesGusanos.insert({i, this->mapaUsado.posicionGusanos[i]});
    }
    
    this->cantidad_gusanos_insertados = 0;

    this->anadirOceano(std::pair<coordX, coordY>(0.0f, 0.0f));
}

//Esto tendria que estar en el YAML? Posiblemente
#define CANTGUSANOS 3

// Usado para castear un puntero a una reference y hacer
// el codigo mas explicito
#define REFERENCE *

int numeroRandomEnRango(int comienzo, int fin) {
    int resultado;
    //Fuente: https://stackoverflow.com/a/13445752/13683575
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(comienzo, fin);
    resultado = dist6(rng);

    return resultado;
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
    fixtureDef.filter.maskBits = (uint16_t)TipoEntidad::VIGA | (uint16_t)TipoEntidad::OCEANO | (uint16_t)TipoEntidad::PROYECTIL | (uint16_t)TipoEntidad::PROVISION;

    body->CreateFixture(&fixtureDef);
    nuevoGusano->setCuerpo(body);

    this->gusanos.push_back(nuevoGusano);
    int idGusano;
    idGusano = this->gusanos.size() - 1;
    nuevoGusano->giveId(idGusano);

    return nuevoGusano;
}

void Partida::anadirProvision() {
    std::pair<coordX, coordY> posicionInicial;
    posicionInicial.enY = MAXALTURA;
    // posicionInicial.enX = 10;

    bool encontreViga = false;
    while (encontreViga == false) {
        //Fuente: https://stackoverflow.com/a/13445752/13683575
        /* Numero aleatorio entre 0 y MAXancho */
        posicionInicial.enX = numeroRandomEnRango(0, MAXANCHO);

        b2Vec2 inicio(posicionInicial.enX, posicionInicial.enY);
        b2Vec2 fin(posicionInicial.enX, 0);

        ResolvedorQuery query;
        b2AABB aabb;
        aabb.lowerBound = fin;
        aabb.upperBound = inicio;
        this->world.QueryAABB( &query, aabb );
        for (int i = 0; i < (int) query.foundBodies.size(); i++) {
	  b2Body* cuerpoA = query.foundBodies[i];
	  
	  Entidad *entidadA = (Entidad *) cuerpoA->GetUserData().pointer;

	  if (entidadA->tipo == TipoEntidad::VIGA) {
	      encontreViga = true;
	      break;
	  }
        }
    }

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
    provisionFixDef.filter.maskBits = -1;

    b2Body* provisionBody = world.CreateBody(&provisionDef);
    provisionBody->CreateFixture(&provisionFixDef);

    int tipo;
    tipo = numeroRandomEnRango(0,1);

    tipoProvision queProvision = VIDA;
    ArmaProtocolo arma = NADA_P;
    if (tipo == 0) {
        queProvision = VIDA;
        arma = NADA_P;
    } else {
        queProvision = MUNICION;
        int pri = NADA_P + 1;
        int fin = INVAL_ARMA_P - 1;
        arma = (ArmaProtocolo) numeroRandomEnRango(pri, fin);
    }

    int idProvision;
    idProvision = this->cantidadProvisionesGeneradas;
    this->cantidadProvisionesGeneradas += 1;

    bool esTrampa;
    int calculoSiTrampa;
    calculoSiTrampa = numeroRandomEnRango(0,1);
    if (calculoSiTrampa == 0)
        esTrampa = true;
    else
        esTrampa = false;

    Provision *nuevaProvision = new Provision(queProvision, arma, provisionBody, idProvision, esTrampa);
    nuevaEntidad->provision = nuevaProvision;

    this->provisiones.push_back(nuevaProvision);
    this->cuerposADestruir.push_back(provisionBody);

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
    fixtureDef.filter.maskBits = -1;
    // fixtureDef.filter.maskBits = 1;

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
    fixtureDef.filter.maskBits = -1;

    oceanoCuerpo->CreateFixture(&fixtureDef);

}


InformacionInicial Partida::obtenerInfoInicial() {
    InformacionInicial infoInicial;

    // std::vector<Gusano*> gusanosParaElNuevoJugador;
    //Todos los gusanos que creamos lo anadimos al jugador y a la partida
    // for (int i = 0; i < CANTGUSANOS; i++) {
              
    //     Gusano *nuevoGusano = this->anadirGusano(posicionesGusanos.at((i + cantidad_gusanos_insertados) % posicionesGusanos.size()));

    //     gusanosParaElNuevoJugador.push_back(nuevoGusano);
    //     cantidad_gusanos_insertados += 1;

    // }
    //Le damos los gusanos al jugador del cliente y acceso a la queue
    //de acciones
    Jugador *jugadorNuevo = new Jugador();
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
        repre.esFragmento = proyectil->esFragmento;

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
        if (velocidad.x < 0) {
            repre.angulo += M_PI;
            std::cout << "ENTRE con: " << repre.angulo << "\n";
        } 
        if (proyectil->armaOrigen == DINAMITA_P) {
            repre.angulo = 0;
        }
        std::cout << "ANGULO ENVIADO: " << repre.angulo << "\n";
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
    if ((proyectil->armaOrigen == MORTERO_P || proyectil->armaOrigen == GRANADA_ROJA_P) && 
        !proyectil->esFragmento) {
        Arma armaElegida(proyectil->armaOrigen);
        this->crearFragmentos(proyectil, armaElegida.getFragmentos());
    }

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
        
        nuevaEntidad->proyectilReal = proyectil;

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
    int cantProyectiles = 0;
    if (arma == ATAQUE_AEREO_P)
        cantProyectiles = 5;
    else
        cantProyectiles = 1;
        
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
        nuevaEntidad->proyectilReal = nuevoProyectil;


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
	  ataque.posicion.y += 5;
	  ataque.posicion.x += 2;
	  std::cout << "POSITION: " << ataque.posicion.x << " " << ataque.posicion.y << "\n";


        }

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

void Partida::generarProvision(time_t horaActual) {
    double diferencia;
    diferencia = difftime(horaActual, this->ultimaProvision);

    if (diferencia < TIEMPOESPERAPROVISION)
        return;

    //Actualizo la hora incluso si el coin flip no lo logra.
    //Es para que SIEMPRE haya TIEMPOESPERAPROVISION de tiempo entre
    //intento e intento
    this->ultimaProvision = horaActual;

    int cointFlip = numeroRandomEnRango(0,1);

    if (cointFlip == 0)
        return;

    std::cout << "GENERAR PROVISION \n";

    this->anadirProvision();
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
    bool expirado;

    Entidad *entidad = (Entidad *) proyectil->GetUserData().pointer;
    if (entidad->tipo == TipoEntidad::PROVISION) {
        expirado = (entidad->provision->fueAgarrada == true);
    } else {
        time_t horaDeCreacion;
        horaDeCreacion = entidad->proyectil.horaDeCreacion;
        time_t horaActual;
        horaActual = time(NOW);
        double diferencia;
        diferencia = difftime(horaActual, horaDeCreacion);

        expirado = diferencia > entidad->proyectil.tiempoMinimoDeVida;
    }

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
        if (proyectil->exploto) {
            // std::cout << "Delete\n";
            Entidad *entidadB = (Entidad *) cuerpoABorrar->GetUserData().pointer;
            this->world.DestroyBody(cuerpoABorrar);
            delete entidadB->proyectilReal;
            this->proyectiles.erase(this->proyectiles.begin() + i);
        }

    }

    for(int i = this->provisiones.size() - 1 ; i >= 0 ; i--) {
        Provision *provision = this->provisiones[i];
        b2Body *cuerpoABorrar = provision->cuerpo;
        // NO HACER delete entidad. Tira invalid delete
        if (provision->fueAgarrada == true) {
            // std::cout << "Delete\n";
            Entidad *entidadB = (Entidad *) cuerpoABorrar->GetUserData().pointer;
            this->world.DestroyBody(cuerpoABorrar);
            delete entidadB->provision;
            this->provisiones.erase(this->provisiones.begin() + i);
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

    Accion accionRecibida;
    accionRecibida.idGusano = INVAL_ID;
    accionRecibida.esEmpezar = false;
    while (this->finPartida == false) {
        //FIXME Esto estaria bueno, pero tira excepcion :(
        // if (this->clientes.size() >= MINJUGADORES) {
        // 	  this->momento = POR_INICIAR;
        // }

        this->finPartida = NOT this->enviarEstadoAJugadores();
        if (this->finPartida) {
            break;
        }

        bool pudeObtenerla;
        pudeObtenerla = acciones.try_pop(accionRecibida);

        if (accionRecibida.esEmpezar && pudeObtenerla) {
            this->momento = EN_MARCHA;
            break;
        }
        std::this_thread::sleep_for(frameDuration);
    }

    bool cantJusta = this->mapaUsado.cantGusanos % this->jugadores.size() == 0;
    int cantGusanos = this->mapaUsado.cantGusanos / this->clientes.size();
    if (cantJusta) {
        for (auto &&jugador : this->jugadores) {
            // std::vector<Gusano*> gusanosJugador;
            for (int i = 0; i < cantGusanos; i++, cantidad_gusanos_insertados++) {
                
                Gusano *nuevoGusano = this->anadirGusano(posicionesGusanos.at(cantidad_gusanos_insertados));

                jugador->anadirGusano(nuevoGusano);
                // cantidad_gusanos_insertados += 1;

            }
            // jugador->setGusanos(gusanosJugador);
        }
        
    }  else {
        for (auto &&jugador : this->jugadores) {
            std::vector<Gusano*> gusanosJugador;
            for (int i = 0; i < cantGusanos; i++, cantidad_gusanos_insertados++) {
                
                Gusano *nuevoGusano = this->anadirGusano(posicionesGusanos.at(cantidad_gusanos_insertados));

                jugador->anadirGusano(nuevoGusano);
                // cantidad_gusanos_insertados += 1;

            }
            // jugador->setGusanos(gusanosJugador);
        }

        int indexJugador = 0;
        for (; cantidad_gusanos_insertados < this->mapaUsado.cantGusanos; cantidad_gusanos_insertados++, indexJugador++) {
            Jugador *jugadorActual = this->jugadores[indexJugador];
            Gusano *nuevoGusano = this->anadirGusano(posicionesGusanos.at(cantidad_gusanos_insertados));
            jugadorActual->anadirGusano(nuevoGusano);
        }

        for (;indexJugador < (int)this->jugadores.size(); indexJugador++) {
            Jugador *jugadorActual = this->jugadores[indexJugador];
            jugadorActual->darMasVidaAGusanos();
        }
    }
    int cant = 6/4;
    std::cout << "CANTIDAD: " << cant << "\n";
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
