#include "partida.h"
#include <map>
#include "box2dDefs.h"

#define SLEEPSEGS 1

const std::chrono::duration<double> frameDuration(1.0 / 30);

Partida::Partida(std::string mapa)
    :world(b2Vec2(FUERZAGRAVITARIAX, FUERZAGRAVITARIAY)){
    this->mapa = mapa;
    this->world.SetContactListener(&this->colisiones);


    this->anadirViga(0, LONGITUDVIGAGRANDE, std::pair<coordX,coordY>(0.0f, 10.0f));
}

//Esto tendria que estar en el YAML?
#define CANTGUSANOS 1

// Usado para castear un puntero a una reference y hacer
// el codigo mas explicito
#define REFERENCE *

void ResolvedorColisiones::BeginContact(b2Contact *contact) {
    b2Body* cuerpoA = contact->GetFixtureA()->GetBody();
    b2Body* cuerpoB = contact->GetFixtureB()->GetBody();

    Entidad *entidadA = (Entidad *) cuerpoA->GetUserData().pointer;
    Entidad *entidadB = (Entidad *) cuerpoB->GetUserData().pointer;

    if(entidadA->tipo == TipoEntidad::VIGA) {
        entidadB->gusano->setEstado(QUIETO);
    }

    if(entidadB->tipo == TipoEntidad::VIGA) {
        entidadA->gusano->setEstado(QUIETO);
    }

    if (entidadA->tipo == TipoEntidad::GUSANO
        &&
        entidadB->tipo == TipoEntidad::ARMA) {

        cuerpoA->ApplyLinearImpulseToCenter(b2Vec2(100.0f, 1000.0f), true);
  }
      
  // abort();
}

void ResolvedorColisiones::EndContact(b2Contact *contact) {
    b2Body* cuerpoA = contact->GetFixtureA()->GetBody();
    b2Body* cuerpoB = contact->GetFixtureB()->GetBody();

    Entidad *entidadA = (Entidad *) cuerpoA->GetUserData().pointer;
    Entidad *entidadB = (Entidad *) cuerpoB->GetUserData().pointer;

    if(entidadA->tipo == TipoEntidad::VIGA) {
        entidadB->gusano->setEstado(CAYENDO);
    }

    if(entidadB->tipo == TipoEntidad::VIGA) {
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

    //ATTENTION: Hacemos que el cuerpo sea dinamico
    //ya que los gusanos se van a mover

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(coords.enX, coords.enY);
    bodyDef.userData.pointer = reinterpret_cast<uintptr_t> (nuevaEntidad);
    b2Body* body = world.CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);
    nuevoGusano->setCuerpo(body);

    this->gusanos.push_back(nuevoGusano);

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
    viga.SetAsBox(longitud, ANCHOVIGA);

    b2Body* groundBody = world.CreateBody(&vigaDef);

    groundBody->CreateFixture(&viga, MASACUERPOESTATICO);

}

InformacionInicial Partida::anadirCliente(Cliente *clienteNuevo) {
    std::vector<Gusano*> gusanosParaElNuevoJugador;
    //Todos los gusanos que creamos lo anadimos al jugador y a la partida
    for (int i = 0 ;i < CANTGUSANOS; i++) {
        //TODO Hacer las coordenadas distintas
        std::pair<coordX, coordY> coordsIniciales(20.0f,20.0f);
        // std::pair<coordX, coordY> coordsIniciales(0.0f,0.0f);

        Gusano *nuevoGusano = this->anadirGusano(coordsIniciales);

        gusanosParaElNuevoJugador.push_back(nuevoGusano);
        int idGusano;
        idGusano = this->gusanos.size();
        nuevoGusano->giveId(idGusano);

    }
    //Le damos los gusanos al jugador del cliente y acceso a la queue
    //de acciones
    Jugador *jugadorNuevo = new Jugador(gusanosParaElNuevoJugador);

    InformacionInicial infoInicial;

    //Creo el id del nuevo jugador
    idJugador idNuevoJugador;
    idNuevoJugador = (idJugador) this->jugadores.size();

    clienteNuevo->obtenerAccesoAAcciones(&this->acciones);

    this->jugadores.push_back(jugadorNuevo);

    //Anadimos al jugador a la partida
    this->clientes.push_back(clienteNuevo);
    //Aviso que se unio un jugador
    this->seUnioJugador.notify_all();

    //Leo las vigas
    std::vector<RepresentacionViga> vigasEnMapa;

    //Fuente: https://www.iforce2d.net/b2dtut/bodies
    for ( b2Body* b = this->world.GetBodyList(); b; b = b->GetNext())
    {

        b2BodyType tipoDelCuerpo;
        tipoDelCuerpo = b->GetType();
        /*
        b2_staticBody = 0,
        b2_kinematicBody,
        b2_dynamicBody
        */
        //Ignoramos los que no son estaticos porque los gusanos los
        //sacamos de los jugadores
        if (tipoDelCuerpo != b2_staticBody)
	  continue;

        RepresentacionViga vigaActual;
        vigaActual.angulo = b->GetAngle();
        //TODO Desharcodear
        vigaActual.longitud = 8;
        b2Vec2 posicion = b->GetPosition();
        std::pair<coordX, coordY> posicionProtocolo;
        posicionProtocolo.enX = posicion.x;
        posicionProtocolo.enY = posicion.y;
        vigaActual.posicionInicial = posicionProtocolo;

        vigasEnMapa.push_back(vigaActual);
    }

    //TODO Mover arriba
    infoInicial.jugador = idNuevoJugador;
    infoInicial.vigas = vigasEnMapa;
    return infoInicial;
}

void Partida::enviarEstadoAJugadores() {
    //TODO esto deberia ser un puntero
    std::shared_ptr<EstadoDelJuego> estadoActual(new EstadoDelJuego);

    std::map<idJugador, std::vector<RepresentacionGusano>> representacionPartida;
    for (int jugador = 0; jugador < (int) this->jugadores.size() ; jugador++) {
        Jugador *jugadorActual;
        jugadorActual = this->jugadores.at(jugador);

        std::vector<RepresentacionGusano> gusanosJugActual;

        gusanosJugActual = jugadorActual->getRepresentacionGusanos();

        representacionPartida.insert({jugador, gusanosJugActual});
    }
    estadoActual->gusanos = representacionPartida;
    // TODO: actualizar para que sea el posta
    estadoActual->jugadorDeTurno = 0;
    estadoActual->gusanoDeTurno = 0;


    // estadoActual.vigas = vigasEnMapa;

    for(Cliente *cliente : this->clientes) {
        cliente->enviarEstadoJuego(estadoActual);
    }

}


Accion Partida::obtenerAccion(Accion accionObtenida, bool obtuvoNueva,
			Accion& ultimaAccion) {
       Accion accionAEjecutar;
       //Si la ultima accion fue de movimiento y no obtuvimos nada
       //nuevo; ejecutamos esa accion de movimiento.
       //AKA: Nos movemos a la Izquierda (por ej) hasta que nos digan
       //de detenernos

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
       else {
	  // accionAEjecutar = ultimaAccion;
	  accionAEjecutar.accion = ESTAQUIETO;
       }

       //WARNING Debug
       // switch (accionAEjecutar.accion) {
       // case ESTAQUIETO:
       // 	 std::cout << "Quieto" << "\n";
       // 	 break;
       // case MOVERSE:
       // 	 std::cout << "Moverse" << "\n";
       // 	 break;
       // case EQUIPARSE:
       // 	 std::cout << "Equiparse" << "\n";
       // 	 break;
       // case PREPARAR:
       // 	 std::cout << "Preparar" << "\n";
       // 	 break;
       // case ATAQUE:
       // 	 std::cout << "Ataque" << "\n";
       // 	 break;
       // }

       return accionAEjecutar;
}

void Partida::darArmaA(Gusano *gusano, ArmaDeseada arma) {
    //Si no quiere equiparse nada, no hacemos nada
    if (arma == NADA_P)
        return;
    // Entidad *nuevaEntidad = new Entidad;
    // nuevaEntidad->tipo = TipoEntidad::ARMA;
    // Arma *nuevaArma = new Arma();
    // nuevaEntidad->arma = nuevaArma;

    std::pair<inicioCaja, finCaja> coordsGolpe;
    coordsGolpe = gusano->getAreaGolpe();

    ResolvedorQuery query;
    b2AABB aabb;
    aabb.lowerBound = coordsGolpe.inicio;
    aabb.upperBound = coordsGolpe.fin;
    this->world.QueryAABB( &query, aabb );

    for (int i = 0; i < (int) query.foundBodies.size(); i++) {
        b2Body* cuerpoA = query.foundBodies[i];
        cuerpoA->ApplyLinearImpulseToCenter(b2Vec2(100.0f, 1000.0f), true);
        // printf("Otro: %4.2f %4.2f \n", pos.x, pos.y);
    }
    
      
    // DireccionGusano dondeMira;
    // dondeMira = gusano->getDondeMira();
    // coordX offset = 0;
    // if (dondeMira == DERECHA)
    //     offset = TAMANOGUSANO;
    // else
    //     offset = -TAMANOGUSANO;
    // coords.enX += offset;

    // std::cout << "Le doy el arma\n";

    // b2BodyDef bodyDef;
    // bodyDef.type = b2_dynamicBody;
    // bodyDef.position.Set(coords.enX, coords.enY);
    // bodyDef.userData.pointer = reinterpret_cast<uintptr_t> (nuevaEntidad);
    // b2Body* body = world.CreateBody(&bodyDef);

    // b2PolygonShape dynamicBox;
    // dynamicBox.SetAsBox(1.0f, 1.0f);

    // b2FixtureDef fixtureDef;
    // fixtureDef.shape = &dynamicBox;
    // fixtureDef.density = 1.0f;
    // fixtureDef.friction = 0.3f;

    // body->CreateFixture(&fixtureDef);

    
}

//     //INICIO_IZQ, FIN_IZQ, INICIO_DER, FIN_DER, SALTO, PIRUETA, INVAL_DIR
void Partida::gameLoop() {
    std::unique_lock<std::mutex> lck(mtx);

    //Esperamos hasta que se unan todos los jugadores necesarios
    while (this->clientes.size() < MINJUGADORES)
        this->seUnioJugador.wait(lck);

    // this->anadirViga(0, 60000, std::pair<coordX,coordY>(0.0f, 10.0f));

    float timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    Jugador *jugadorActual;
    jugadorActual = this->jugadores.at(0);
    Gusano *gusanoActual;
    gusanoActual = jugadorActual->getGusanoActual();
    // for (int32 i = 0; i < 600; ++i)
    // {
    //     world.Step(timeStep, velocityIterations, positionIterations);
    //     // b2Vec2 position = body->GetPosition();
    //     // float angle = body->GetAngle();
    //     // printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
    //     std::pair<coordX, coordY> coords;
    //     coords = gusanoActual->getCoords();
    //     printf("%4.2f %4.2f \n", coords.enX, coords.enY);
    //     this->enviarEstadoAJugadores();

    //     std::this_thread::sleep_for(frameDuration);
    // }


    //WARNING: Creamos esto para testear las colisiones
    // b2BodyDef bodyDef;
    // bodyDef.type = b2_dynamicBody;
    // bodyDef.position.Set(0.0f, 110.0f);
    // b2Body* body = world.CreateBody(&bodyDef);

    // b2PolygonShape dynamicBox;
    // dynamicBox.SetAsBox(1.0f, 1.0f);

    // b2FixtureDef fixtureDef;
    // fixtureDef.shape = &dynamicBox;
    // fixtureDef.density = 1.0f;
    // fixtureDef.friction = 0.3f;

    // body->CreateFixture(&fixtureDef);
    
    // abort();

    Accion ultimaAccion;

    while (true) {
        this->world.Step(timeStep, velocityIterations, positionIterations);
        this->enviarEstadoAJugadores();

        Accion accionRecibida;
        bool pudeObtenerla;
        pudeObtenerla = acciones.try_pop(accionRecibida);
        // accionRecibida = acciones.pop();

        Accion accionAEjecutar;
        accionAEjecutar = this->obtenerAccion(accionRecibida, pudeObtenerla,
				      ultimaAccion);

        ArmaDeseada armaQueQuiere;
        armaQueQuiere = gusanoActual->ejecutar(accionAEjecutar);

        this->darArmaA(gusanoActual, armaQueQuiere);



        printf("\n");
        std::pair<coordX, coordY> posicionamiento = gusanoActual->getCoords();
        printf("Gusano: %4.2f %4.2f \n", posicionamiento.enX, posicionamiento.enY);


        // b2Vec2 position = body->GetPosition();
        // printf("Otro: %4.2f %4.2f \n", position.x, position.y);




        // std::pair<cambioX, cambioY> cambioDeseado = gusanoActual->cambio(accionAEjecutar);

        // std::pair<coordX, coordY> coordenadasIniciales = gusanoActual->getCoords();
        // std::pair<coordX, coordY> coordenadasFinales;
        // coordenadasFinales.first = coordenadasIniciales.first + cambioDeseado.first;
        // coordenadasFinales.second = coordenadasIniciales.second + cambioDeseado.second;

        std::this_thread::sleep_for(frameDuration);
    }

}
