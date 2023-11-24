#include "partida.h"
#include <map>
#include "box2dDefs.h"

#define SLEEPSEGS 1
#define NOW NULL
#define NOT !

int esperar = 0;

const std::chrono::duration<double> frameDuration(1.0 / 30);

Partida::Partida(std::string mapa)
    :world(b2Vec2(FUERZAGRAVITARIAX, FUERZAGRAVITARIAY)){
    this->mapa = mapa;
    this->world.SetContactListener(&this->colisiones);
    this->posJugadorActual = -1;
    this->finPartida = false;


    this->anadirViga(0, LONGITUDVIGAGRANDE, std::pair<coordX,coordY>(03.0f, 20.0f));
    this->anadirViga(0, LONGITUDVIGAGRANDE, std::pair<coordX,coordY>(13.0f, 20.0f));
    this->anadirViga(0, LONGITUDVIGAGRANDE, std::pair<coordX,coordY>(23.0f, 20.0f));
    this->anadirViga(0, LONGITUDVIGAGRANDE, std::pair<coordX,coordY>(33.0f, 20.0f));
    this->anadirViga(0, LONGITUDVIGAGRANDE, std::pair<coordX,coordY>(43.0f, 20.0f));
}

//Esto tendria que estar en el YAML?
#define CANTGUSANOS 2

// Usado para castear un puntero a una reference y hacer
// el codigo mas explicito
#define REFERENCE *

void ResolvedorColisiones::BeginContact(b2Contact *contact) {
    b2Body* cuerpoA = contact->GetFixtureA()->GetBody();
    b2Body* cuerpoB = contact->GetFixtureB()->GetBody();

    Entidad *entidadA = (Entidad *) cuerpoA->GetUserData().pointer;
    Entidad *entidadB = (Entidad *) cuerpoB->GetUserData().pointer;

    if (entidadB->tipo == TipoEntidad::PROYECTIL
        &&
        entidadA->tipo == TipoEntidad::GUSANO) {
        b2Vec2 dir = cuerpoB->GetLinearVelocity();
        // cuerpoA->ApplyLinearImpulseToCenter(dir, true);
        entidadA->gusano->recibirDano(dir, entidadB->proyectil.arma);
        printf("A\n");
        // abort();
    }

    if (entidadA->tipo == TipoEntidad::PROYECTIL
        &&
        entidadB->tipo == TipoEntidad::GUSANO) {
        b2Vec2 dir = cuerpoA->GetLinearVelocity();
        entidadB->gusano->recibirDano(dir, entidadA->proyectil.arma);
        printf("B\n");
        // abort();
    }

    else if(entidadA->tipo == TipoEntidad::VIGA
        &&
        entidadB->tipo == TipoEntidad::GUSANO) {
        entidadB->gusano->setEstado(QUIETO);
    }

    else if(entidadB->tipo == TipoEntidad::VIGA 
        &&
        entidadA->tipo == TipoEntidad::GUSANO) {
        entidadA->gusano->setEstado(QUIETO);
    }

    else if (entidadA->tipo == TipoEntidad::GUSANO
        &&
        entidadB->tipo == TipoEntidad::ARMA) {

        cuerpoA->ApplyLinearImpulseToCenter(b2Vec2(100.0f, 1000.0f), true);
  }
      
  // abort();
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
        entidadB->gusano->setEstado(CAYENDO);
    }

    if(entidadB->tipo == TipoEntidad::VIGA
       &&
       entidadA->tipo == TipoEntidad::GUSANO) {
        entidadA->gusano->setEstado(CAYENDO);
    }
    std::cout << "FIN CONTACTO\n";
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

    groundBody->CreateFixture(&viga, MASACUERPOESTATICO);

}


InformacionInicial Partida::obtenerInfoInicial() {
    InformacionInicial infoInicial;

    std::vector<Gusano*> gusanosParaElNuevoJugador;
    //Todos los gusanos que creamos lo anadimos al jugador y a la partida
    for (int i = 0 ;i < CANTGUSANOS; i++) {
        //TODO Hacer las coordenadas distintas

        std::pair<coordX, coordY> coordsIniciales(15.0f,23.0f);
      
        Gusano *nuevoGusano = this->anadirGusano(coordsIniciales);

        gusanosParaElNuevoJugador.push_back(nuevoGusano);

    }
    //Le damos los gusanos al jugador del cliente y acceso a la queue
    //de acciones
    Jugador *jugadorNuevo = new Jugador(gusanosParaElNuevoJugador);
    this->jugadores.push_back(jugadorNuevo);

    //Creo el id del nuevo jugador
    idJugador idNuevoJugador;
    idNuevoJugador = (idJugador) this->jugadores.size();
    infoInicial.jugador = idNuevoJugador;

    //Leo las vigas
    std::vector<RepresentacionViga> vigasEnMapa;
    for ( b2Body* b = this->world.GetBodyList(); b; b = b->GetNext())
    {

        b2BodyType tipoDelCuerpo;
        tipoDelCuerpo = b->GetType();
        //Ignoramos los que no son estaticos porque los gusanos los
        //sacamos de los jugadores
        if (tipoDelCuerpo != b2_staticBody)
	  continue;

        RepresentacionViga vigaActual;
        vigaActual.angulo = b->GetAngle();
        // Juampi: creo que el problema de que no estaban alineadas las vigas era que le 
        // estabas pasando el 8 hardcodeado para graficar y la longitud de la viga en box2d es 
        // LONGITUDVIGAGRANDE que es 14, pero no estoy seguro.
        // Me parece que 14 es mucho y se van a superponer, así que lo volví a 6.
        // El centro de la viga al parecer no es la esquina inferior izquierda, es el centro de 
        // la caja puede ser? Probe usando ese centro en cliente y se veian bien.
        // En teoría con esto así debería andar si se le pone un valor más chico en el def.
        vigaActual.longitud = LONGITUDVIGAGRANDE;
        b2Vec2 posicion = b->GetPosition();
        std::pair<coordX, coordY> posicionProtocolo;
        posicionProtocolo.enX = posicion.x;
        posicionProtocolo.enY = posicion.y;
        vigaActual.posicionInicial = posicionProtocolo;

        vigasEnMapa.push_back(vigaActual);
    }

    infoInicial.vigas = vigasEnMapa;


    return infoInicial;
}

void Partida::anadirCliente(Cliente *clienteNuevo) {
    //TODO: Lock
    clienteNuevo->obtenerAccesoAAcciones(&this->acciones);

    //Anadimos al jugador a la partida
    this->clientes.push_back(clienteNuevo);

    //Aviso que se unio un jugador
    this->seUnioJugador.notify_all();
}

bool Partida::enviarEstadoAJugadores() {
    std::shared_ptr<EstadoDelJuego> estadoActual(new EstadoDelJuego);

    std::map<idJugador, std::map<id, RepresentacionGusano>> representacionGusanos;
    for (int jugador = 0; jugador < (int) this->jugadores.size() ; jugador++) {
        Jugador *jugadorActual;
        jugadorActual = this->jugadores.at(jugador);

        if (jugador == this->posJugadorActual) {
	  estadoActual->jugadorDeTurno = jugador;
	  estadoActual->gusanoDeTurno = jugadorActual->getGusanoDeTurno()->getId();
	  estadoActual->segundosRestantes = jugadorActual->getGusanoDeTurno()->getTiempoQueMeQueda();
	  // std::cout << "TIEMPO: " << estadoActual->segundosRestantes << "\n";
        }

        std::map<id, RepresentacionGusano> gusanosJugActual;

        gusanosJugActual = jugadorActual->getRepresentacionGusanos();

        representacionGusanos.insert({jugador, gusanosJugActual});
    }
    estadoActual->gusanos = representacionGusanos;


    std::vector<RepresentacionProyectil> proyectilesRepre;
    for (Proyectil *proyectil : this->proyectiles) {
        //TODO: Hack momentaneo
        if (proyectil->armaOrigen == NADA_P
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
        repre.posicion = deb2VecACoord(proyectil->posicion);
        repre.angulo = 0.0f;
        repre.cuentaRegresiva = proyectil->countdown;
        repre.exploto = proyectil->exploto;

        proyectilesRepre.push_back(repre);
    }
    estadoActual->proyectiles = proyectilesRepre;

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
			Accion& ultimaAccion) {
    Accion accionAEjecutar;
    //Si la ultima accion fue de movimiento y no obtuvimos nada
    //nuevo; ejecutamos esa accion de movimiento.
    //AKA: Nos movemos a la Izquierda hasta que nos digan de detenernos

    tipoAccion tipoUltimaAccion;
    tipoUltimaAccion = ultimaAccion.accion;
    if (obtuvoNueva == true) {
        accionAEjecutar = accionObtenida;
        ultimaAccion = accionAEjecutar;
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
    }
    else {
        accionAEjecutar = ultimaAccion;
        accionAEjecutar.accion = ESTAQUIETO;
    }


    return accionAEjecutar;
}

void Partida::crearProjectil(Gusano *gusano, Ataque ataque, Proyectil* proyectil) {
    ArmaDeseada arma;
    arma = ataque.arma;
    int countdown = proyectil->countdown;
    //Si no quiere equiparse nada, no hacemos nada
    if (arma == NADA_P)
        return;

    else if (arma == BATE_P) {
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
	  entidadA->gusano->recibirDano(golpe, BATE_P);

	  // printf("PEGO\n");
        }
    }

    else if (arma == DINAMITA_P) {
        if (countdown > 0)
	  return;
        //WARNING HAY QUE PONER QUE NO EXPLOTO MAS ADELANTE
        proyectil->exploto = true;

        printf("KATAPUM\n");
        int numRays = 32;
        for (int i = 0; i < numRays; i++) {
	  std::cout << i << "\n";
	  Entidad *nuevaEntidad = new Entidad;
	  nuevaEntidad->tipo = TipoEntidad::PROYECTIL;
      nuevaEntidad->proyectil.arma = DINAMITA_P;

	  float angle = (i / (float)numRays) * 360 * DEGTORAD;
	  b2Vec2 rayDir( sinf(angle), cosf(angle) );

	  b2BodyDef bd;
	  bd.type = b2_dynamicBody;
	  bd.fixedRotation = true; // rotation not necessary
	  bd.bullet = true; // prevent tunneling at high speed
	  // bd.linearDamping = 10; // drag due to moving through air
	  bd.gravityScale = 0; // ignore gravity
	  bd.userData.pointer = reinterpret_cast<uintptr_t> (nuevaEntidad);
	  b2Vec2 coords = ataque.posicion;
	  bd.position = coords;
	  // bd.linearVelocity = blastPower * rayDir;
	  bd.linearVelocity = 200 * rayDir;
	  b2Body* body = this->world.CreateBody( &bd );

	  b2CircleShape circleShape;
	  circleShape.m_radius = 0.05; // very small

	  b2FixtureDef fd;
	  fd.shape = &circleShape;
	  fd.density = 60 / (float)numRays; // very high - shared across all particles
	  fd.friction = 0; // friction not necessary
	  fd.restitution = 0.99f; // high restitution to reflect off obstacles
	  fd.filter.groupIndex = -1; // particles should not collide with each other
	  body->CreateFixture( &fd );
	  nuevaEntidad->proyectil.proyectil = body;
        }
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

std::pair<Gusano *, Jugador *> Partida::cambiarDeJugador(Jugador *jugadorTurnoActual, Gusano *gusanoActual, time_t tiempoActual) {
    std::pair<Gusano *, Jugador *> gusanoYJugador;
    Gusano *gusanoDeTurno = gusanoActual;
    Jugador *jugadorDeTurno = jugadorTurnoActual;
    gusanoYJugador.first = gusanoActual;
    gusanoYJugador.second = jugadorDeTurno;

    bool cambioDeTurno;
    cambioDeTurno = gusanoActual->hayQueCambiarDeTurno(tiempoActual);
    //Si no hay cambio de turno, devolvemos el mismo gusano
    if (cambioDeTurno == false)
        return gusanoYJugador;

    jugadorDeTurno = this->siguienteJugador(jugadorTurnoActual);
    gusanoDeTurno = jugadorDeTurno->getGusanoActual();
    gusanoDeTurno->esMiTurno(tiempoActual);
    gusanoYJugador.first = gusanoDeTurno;
    gusanoYJugador.second = jugadorDeTurno;

    return gusanoYJugador;
}

void Partida::gameLoop() {
    std::unique_lock<std::mutex> lck(mtx);

    //Esperamos hasta que se unan todos los jugadores necesarios
    while (this->clientes.size() < MINJUGADORES)
        this->seUnioJugador.wait(lck);

    float timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;


    Accion ultimaAccion;
    ultimaAccion.idGusano = INVAL_ID;
    // WARNING valor basura para que no rompa valgrind
    ultimaAccion.accion = ESTAQUIETO;
    bool exploto = false;

    b2Vec2 origen(0,0);
    Ataque ataqueARealizar;
    Proyectil * nuevoProyectil = new Proyectil();
    nuevoProyectil->armaOrigen = NADA_P;
    nuevoProyectil->posicion = origen;
    nuevoProyectil->id = 0;
    nuevoProyectil->countdown = 0;
    nuevoProyectil->cuerpo = nullptr;
    nuevoProyectil->exploto = false;
    this->proyectiles.push_back(nuevoProyectil);
    ataqueARealizar.proyectilAsociado = nuevoProyectil;

    time_t tiempoActual;
    tiempoActual = time(NOW);

    Jugador *jugadorActual;
    jugadorActual = this->siguienteJugador(NULL);

    Gusano *gusanoActual;
    gusanoActual = jugadorActual->getGusanoDeTurno();
    gusanoActual->esMiTurno(tiempoActual);
    while (this->finPartida == false) {
        tiempoActual = time(NOW);

        std::pair<Gusano *, Jugador *> gusanoYJugador;
        gusanoYJugador = this->cambiarDeJugador(jugadorActual, gusanoActual, tiempoActual);
        gusanoActual = gusanoYJugador.first;
        jugadorActual = gusanoYJugador.second;

        this->world.Step(timeStep, velocityIterations, positionIterations);

        //Fabri was here
        this->finPartida = NOT this->enviarEstadoAJugadores();
        if (this->finPartida == true)
	  break;

        Accion accionRecibida;
        accionRecibida.idGusano = INVAL_ID;
        bool pudeObtenerla;
        pudeObtenerla = acciones.try_pop(accionRecibida);

        Accion accionAEjecutar;
        accionAEjecutar = this->obtenerAccion(accionRecibida, pudeObtenerla,
				      ultimaAccion);

        ataqueARealizar = gusanoActual->ejecutar(accionAEjecutar);

        if (nuevoProyectil->countdown == 0) {
            nuevoProyectil->armaOrigen = ataqueARealizar.arma;
            nuevoProyectil->countdown = ataqueARealizar.tiempoEspera;
            nuevoProyectil->posicion = ataqueARealizar.posicion;
            nuevoProyectil->exploto = false;
        }
        
        else {
            nuevoProyectil->countdown -= 1;
            ataqueARealizar.arma = ultimaAccion.armaAEquipar;
            ataqueARealizar.posicion = nuevoProyectil->posicion;
        }


        this->crearProjectil(gusanoActual, ataqueARealizar, nuevoProyectil);

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
