#include "partida.h"
#include "defs.h"
#include "gusano.h"
#include "jugador.h"
#include "protocolo.h"
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <utility>
#include <chrono>

#define fuerzaGravitariaX 0.0f
#define fuerzaGravitariaY -10.0f 
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

        //Anadimos los gusanos del cliente a la partida
        // this->gusanos.push_back(nuevoGusano);

        //TODO No soy fan de que tenga que guardar las coordenadas
        //en dos lados distintos. Es lo que hay (?.
        // this->coordsGusanos.insert({coordsIniciales,nuevoGusano});
        this->gusanos.push_back(nuevoGusano);
    }
    //Le damos los gusanos al jugador del cliente y acceso a la queue
    //de acciones
    Jugador *jugadorNuevo = new Jugador(gusanosParaElNuevoJugador);
    this->jugadores.push_back(jugadorNuevo);

    //Anadimos al jugador a la partida
    this->clientes.push_back(clienteNuevo);
    //Aviso que se unio un jugador
    this->seUnioJugador.notify_all();

    idJugador idNuevoJugador;
    idNuevoJugador = (idJugador) this->jugadores.size();
    return idNuevoJugador;
}


// std::pair<coordX, coordY> Partida::gravedad(
// 				    std::pair<cambioX, cambioY> cambioDeseado,
// 				    std::pair<coordX, coordY> posInicial
// 				    ) {
//     return cambioDeseado;
// }

void Partida::enviarEstadoAJugadores() {
    EstadoDelJuego estadoActual;
    // for (auto const& [posicion, gusano] : this->coordsGusanos) {
    //     if (gusano == nullptr)
    // 	  continue;
    //     estadoActual.posicion = posicion;

    //     DireccionGusano direccionPresente;
    //     direccionPresente = gusano->getDireccion(); 
    //     estadoActual.dir = direccionPresente;
    // }
    std::map<idJugador, std::vector<RepresentacionGusano>> representacionPartida;
    for (int jugador = 0; jugador < (int) this->jugadores.size() ; jugador++) {
        Jugador *jugadorActual;
        jugadorActual = this->jugadores.at(jugador);

        std::vector<RepresentacionGusano> gusanosJugActual;

        gusanosJugActual = jugadorActual->getRepresentacionGusanos();

        representacionPartida.insert({jugador, gusanosJugActual});
        
        // std::pair<coordX, coordY> coords;
        // coords = gusanoActual->getCoords();

        // estadoActual.posicion = coords;

        // DireccionGusano direccionPresente;
        // direccionPresente = gusanoActual->getDireccion(); 
        // estadoActual.dir = direccionPresente;
    }



    // TODO: cambiar a clientes
    // for(Jugador *jugador : this->jugadores) {
    //     jugador->enviarEstadoJuego(estadoActual);
    // }
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
       }
       //Si entra en estos otros if es porque NO se obtuvo algo nuevo
       else if (tipoUltimaAccion == MOVERSE) {
	  accionAEjecutar = ultimaAccion;
       }
       else {
	  accionAEjecutar = ultimaAccion;
	  accionAEjecutar.accion = NOMOVERSE;
       }

       return accionAEjecutar;
}

//     //INICIO_IZQ, FIN_IZQ, INICIO_DER, FIN_DER, SALTO, PIRUETA, INVAL_DIR
void Partida::gameLoop() {
    std::unique_lock<std::mutex> lck(mtx);

    //Esperamos hasta que se unan todos los jugadores necesarios
    while (this->clientes.size() < MINJUGADORES)
        this->seUnioJugador.wait(lck);

    /*
      Creamos un cuerpo rigidoo
    */

    //Esto crea un cuerpo, el cual despues se lo vamos a pasar al
    //mundo. Los cuerpos por default, son estaticos
    //WARNING:*Los cuerpos estaticos no chocan con otros cuerpos y son
    //inmovibles*
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);

    //Hacemos que el world cree un cuerpo basado en nuestra definicion
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);

    groundBody->CreateFixture(&groundBox, 0.0f);


    /*
      Creamos un cuerpo dinamico
    */
    // b2BodyDef bodyDef;
    // //ATTENTION: Hacemos que el cuerpo sea dinamico
    // bodyDef.type = b2_dynamicBody;
    // bodyDef.position.Set(0.0f, 4.0f);
    // b2Body* body = world.CreateBody(&bodyDef);

    // b2PolygonShape dynamicBox;
    // dynamicBox.SetAsBox(1.0f, 1.0f);

    // b2FixtureDef fixtureDef;
    // fixtureDef.shape = &dynamicBox;
    // fixtureDef.density = 1.0f;
    // fixtureDef.friction = 0.3f;

    // body->CreateFixture(&fixtureDef);



    float timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    // Cliente *clienteActual;
    // clienteActual = this->clientes.at(0);
    Jugador *jugadorActual;
    jugadorActual = this->jugadores.at(0);
    Gusano *gusanoActual;
    gusanoActual = jugadorActual->getGusanoActual();
    for (int32 i = 0; i < 600; ++i)
    {
        world.Step(timeStep, velocityIterations, positionIterations);
        // b2Vec2 position = body->GetPosition();
        // float angle = body->GetAngle();
        // printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
        std::pair<coordX, coordY> coords;
        coords = gusanoActual->getCoords();
        printf("%4.2f %4.2f \n", coords.enX, coords.enY);
        this->enviarEstadoAJugadores();

        std::this_thread::sleep_for(frameDuration);
    }


    
    abort();

    // int posJugadorActual = 0;

    //TODO Como cambiar de jugador
    // Cliente *clienteActual;
    // clienteActual = this->clientes.at(posJugadorActual);
    // Jugador *jugadorActual;
    // jugadorActual = clienteActual->getJugador();
    // Gusano *gusanoActual;
    // gusanoActual = jugadorActual->getGusanoActual();

    Accion ultimaAccion;

    while (true) {
        this->enviarEstadoAJugadores();

        Accion accionRecibida;
        bool pudeObtenerla;
        pudeObtenerla = acciones.try_pop(accionRecibida);

        Accion accionAEjecutar;
        accionAEjecutar = this->obtenerAccion(accionRecibida, pudeObtenerla,
				      ultimaAccion);

        std::pair<cambioX, cambioY> cambioDeseado = gusanoActual->cambio(accionAEjecutar);

        std::pair<coordX, coordY> coordenadasIniciales = gusanoActual->getCoords();
        std::pair<coordX, coordY> coordenadasFinales;
        coordenadasFinales.first = coordenadasIniciales.first + cambioDeseado.first;
        coordenadasFinales.second = coordenadasIniciales.second + cambioDeseado.second;

        // gusanoActual->setCoords(coordenadasFinales);
        // this->coordsGusanos[coordenadasFinales] = gusanoActual;

        //TODO Borrar
        //Tengo que poner el if, porque sino se pisaria el puntero en
        //el caso donde ambas posiciones sean iguales aka el gusano
        //no se movio
        // if (coordenadasIniciales != coordenadasFinales)
        // 	  this->coordsGusanos[coordenadasIniciales] = nullptr;
      
        std::this_thread::sleep_for(frameDuration);
    }

}
