#include "partida.h"
#include <map>
#include "box2dDefs.h"

#define SLEEPSEGS 1

const std::chrono::duration<double> frameDuration(1.0 / 30);

Partida::Partida(std::string mapa)
    :world(b2Vec2(fuerzaGravitariaX, fuerzaGravitariaY)){
    this->mapa = mapa;
}

//Esto tendria que estar en el YAML?
#define CANTGUSANOS 1

// Usado para castear un puntero a una reference y hacer
// el codigo mas explicito
#define REFERENCE *

Gusano *Partida::anadirGusano(std::pair<coordX, coordY> coords) {
    b2BodyDef bodyDef;
    //ATTENTION: Hacemos que el cuerpo sea dinamico
    //ya que los gusanos se van a mover
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(coords.enX, coords.enY);
    b2Body* body = world.CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);

    Gusano *nuevoGusano = new Gusano(REFERENCE body);

    return nuevoGusano;
}

void Partida::anadirViga(radianes angulo, int longitud, std::pair<coordX, coordY> posicionInicial) {
    b2BodyDef vigaDef;
    vigaDef.position.Set(posicionInicial.enX, posicionInicial.enY);
    vigaDef.angle = angulo;

    //ATTENTION Dividimos a la mitad porque box2d pide la mitad de
    // la longitud
    longitud /= 2;

    b2PolygonShape viga;
    viga.SetAsBox(longitud, anchoViga);

    b2Body* groundBody = world.CreateBody(&vigaDef);

    groundBody->CreateFixture(&viga, masaCuerpoEstatico);

}

idJugador Partida::anadirCliente(Cliente *clienteNuevo) {
    std::vector<Gusano*> gusanosParaElNuevoJugador;
    //Todos los gusanos que creamos lo anadimos al jugador y a la partida
    for (int i = 0 ;i < CANTGUSANOS; i++) {
        //TODO Hacer las coordenadas distintas
        std::pair<coordX, coordY> coordsIniciales(0.0f,100.0f);
        // std::pair<coordX, coordY> coordsIniciales(0.0f,0.0f);

        Gusano *nuevoGusano = this->anadirGusano(coordsIniciales);

        gusanosParaElNuevoJugador.push_back(nuevoGusano);
        int idGusano;
        idGusano = this->gusanos.size();
        nuevoGusano->giveId(idGusano);

        this->gusanos.push_back(nuevoGusano);
    }
    //Le damos los gusanos al jugador del cliente y acceso a la queue
    //de acciones
    Jugador *jugadorNuevo = new Jugador(gusanosParaElNuevoJugador);

    idJugador idNuevoJugador;
    idNuevoJugador = (idJugador) this->jugadores.size();

    clienteNuevo->obtenerAccesoAAcciones(&this->acciones);

    this->jugadores.push_back(jugadorNuevo);

    //Anadimos al jugador a la partida
    this->clientes.push_back(clienteNuevo);
    //Aviso que se unio un jugador
    this->seUnioJugador.notify_all();

    //TODO Mover arriba
    return idNuevoJugador;
}

void Partida::enviarEstadoAJugadores() {
    EstadoDelJuego estadoActual;

    std::map<idJugador, std::vector<RepresentacionGusano>> representacionPartida;
    for (int jugador = 0; jugador < (int) this->jugadores.size() ; jugador++) {
        Jugador *jugadorActual;
        jugadorActual = this->jugadores.at(jugador);

        std::vector<RepresentacionGusano> gusanosJugActual;

        gusanosJugActual = jugadorActual->getRepresentacionGusanos();

        representacionPartida.insert({jugador, gusanosJugActual});
    }
    estadoActual.gusanos = representacionPartida;

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
    }

    estadoActual.vigas = vigasEnMapa;

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

//     //INICIO_IZQ, FIN_IZQ, INICIO_DER, FIN_DER, SALTO, PIRUETA, INVAL_DIR
void Partida::gameLoop() {
    std::unique_lock<std::mutex> lck(mtx);

    //Esperamos hasta que se unan todos los jugadores necesarios
    while (this->clientes.size() < MINJUGADORES)
        this->seUnioJugador.wait(lck);

    this->anadirViga(0, 6, std::pair<coordX,coordY>(0.0f, 10.0f));

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








        // std::pair<cambioX, cambioY> cambioDeseado = gusanoActual->cambio(accionAEjecutar);

        // std::pair<coordX, coordY> coordenadasIniciales = gusanoActual->getCoords();
        // std::pair<coordX, coordY> coordenadasFinales;
        // coordenadasFinales.first = coordenadasIniciales.first + cambioDeseado.first;
        // coordenadasFinales.second = coordenadasIniciales.second + cambioDeseado.second;

        std::this_thread::sleep_for(frameDuration);
    }

}
