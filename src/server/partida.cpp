#include "partida.h"
#include "defs.h"
#include "gusano.h"
#include "jugador.h"
#include "protocolo.h"
#include <cstdlib>
#include <unistd.h>
#include <utility>
#include <chrono>

#include <box2d/box2d.h>

#define SLEEPSEGS 1

const std::chrono::duration<double> frameDuration(1.0 / 30);

Partida::Partida(std::string mapa) {
    this->mapa = mapa;
}

//Esto tendria que estar en el YAML?
#define CANTGUSANOS 1

/*
  struct zonaAfectada {
        std::pair<posX,posY> coordenada;
        int lejania;
        int danoBase;
  }

 */

void Partida::anadirJugador(Jugador *jugadorNuevo) {
    std::vector<Gusano*> gusanosParaElCliente;
    //Todos los gusanos que creamos lo anadimos al jugador y a la partida
    for (int i = 0 ;i < CANTGUSANOS; i++) {
        //TODO Hacer las coordenadas distintas
        std::pair<int, int> coordsIniciales(0,0);

        Gusano *nuevoGusano = new Gusano(coordsIniciales);
        gusanosParaElCliente.push_back(nuevoGusano);

        //Anadimos los gusanos del cliente a la partida
        // this->gusanos.push_back(nuevoGusano);

        //TODO No soy fan de que tenga que guardar las coordenadas
        //en dos lados distintos. Es lo que hay (?.
        this->coordsGusanos.insert({coordsIniciales,nuevoGusano});
    }
    //Le damos los gusanos al cliente y acceso a la queue de acciones
    jugadorNuevo->obtenerGusanosIniciales(gusanosParaElCliente);
    jugadorNuevo->obtenerAccesoAAcciones(&this->acciones);

    //Anadimos al jugador a la partida
    this->jugadores.push_back(jugadorNuevo);
    //Aviso que se unio un jugador
    this->seUnioJugador.notify_all();
}


std::pair<int, int> Partida::gravedad(std::pair<int, int> cambioDeseado,
		        std::pair<int, int> posInicial
		         ){
    return cambioDeseado;
}

// void Partida::gameLoop() {
//     int posJugadorActual = 0;

//     while (this->jugadores.size() < 1) {
//         sleep(4);
//     }

//     while (true) {
//         sleep(SLEEPSEGS);

//         Jugador* jugadorActual = jugadores.at(posJugadorActual);

//         //Non blocking pop (try pop)
//         bool pudeObtenerla;
//         Direccion accionAEjecutar;
//         pudeObtenerla = jugadorActual->obtenerAccion(accionAEjecutar);

//         //Si no obtuvimos nada, vamos a la siguiente iteracion.
//         //Sigue siendo el turno del mismo jugador y de el mismo gusano
//         if (pudeObtenerla == false)
// 	  continue;

//         /*
//          CUIDADO: EFECTOS SECUNDARIOS:
//          ESTA FUNCION VA A DEVOLVER UN GUSANO DISTINTO CADA VEZ QUE
//          LA LLAMES.
//          No soy nada fan de esto. Pero creo que es la forma mas
//          elegante de hacer que los jugadores manejen a sus gusanos.
//          Entonces, a medida que van muriendo, el jugador se encarga
//          de matarlo.
//         */
//         Gusano *gusanoActual = jugadorActual->getGusanoActual();
//         std::pair<int, int> cambio;
//         cambio = gusanoActual->cambio(accionAEjecutar);

//         std::pair<int, int> coordenadasIniciales;
//         coordenadasIniciales = gusanoActual->getCoords();

//         std::pair<int, int> coordenadasFinales;
//         coordenadasFinales  = this->gravedad(cambio, coordenadasIniciales);
//         //TODO: Cambiar a algo mas generico cuando tengamos las armas
//         // int jugadorActual.getAccion();

//         //TODO: Implementar "calcular cambios"
//         //std::list<std::pair<posX,posY> areasAfectadas = this.calcularCambios(Accion);

//         //TODO: Implementar "actualizar"
//         //this.actualizar();
//         this->coordsGusanos[coordenadasFinales] = gusanoActual;
//         this->coordsGusanos[coordenadasIniciales] = nullptr;

//         //actualizarGameState();
//         //TODO: En el game state incluir si es el turno O pedir un
//         //mensaje al protocolo


//         posJugadorActual += 1;

//     }
// };
void Partida::enviarEstadoAJugadores() {
    EstadoDelJuego estadoActual;
    for (auto const& [posicion, gusano] : this->coordsGusanos) {
        if (gusano == nullptr)
	  continue;
        estadoActual.posicion = posicion;

        DireccionGusano direccionPresente;
        direccionPresente = gusano->getDireccion(); 
        estadoActual.dir = direccionPresente;
    }

    for(Jugador *jugador : this->jugadores) {
        jugador->enviarEstadoJuego(estadoActual);
    }
}

// std::pair<int,int> Partida::calcularMovimiento(Gusano *gusano, Direccion accion, bool estaMoviendose) {
//     std::pair<int,int> cambioARealizar;
//     //INICIO_IZQ, FIN_IZQ, INICIO_DER, FIN_DER, SALTO, PIRUETA, INVAL_DIR
//     switch (accion) {
//     case SALTO:
//         break;
//     case PIRUETA:
//         break;
//     case INVAL_DIR:
//         abort();
//         break;
//     }
    
    
// }

Accion Partida::obtenerAccion(Direccion accionObtenida, bool obtuvoNueva,
			Accion& ultimaAccion) {
    Accion accionAEjecutar;
    if (obtuvoNueva == false) {
        // accionAEjecutar = Accion::MOV_QUIETO;
        accionAEjecutar = ultimaAccion;
        return accionAEjecutar;
    }

    switch (accionObtenida) {
//     //INICIO_IZQ, FIN_IZQ, INICIO_DER, FIN_DER, SALTO, PIRUETA, INVAL_DIR
    case INICIO_IZQ:
        accionAEjecutar = Accion::MOV_IZQ;
        break;
    case FIN_IZQ:
        accionAEjecutar = Accion::MOV_QUIETO;
        break;
    case INICIO_DER:
        accionAEjecutar = Accion::MOV_DER;
        break;
    case FIN_DER:
        accionAEjecutar = Accion::MOV_QUIETO;
        break;
    case SALTO:
        accionAEjecutar = Accion::MOV_SALTO;
        break;
    case PIRUETA:
        accionAEjecutar = Accion::MOV_PIRUETA;
        break;
    case INVAL_DIR:
        abort();
        break;
    }

    ultimaAccion = accionAEjecutar;
    return accionAEjecutar;
}

//     //INICIO_IZQ, FIN_IZQ, INICIO_DER, FIN_DER, SALTO, PIRUETA, INVAL_DIR
void Partida::gameLoop() {
    std::unique_lock<std::mutex> lck(mtx);

    //Esperamos hasta que se unan todos los jugadores necesarios
    while (this->jugadores.size() < MINJUGADORES)
        this->seUnioJugador.wait(lck);

    /*
      Creamos un cuerpo rigidoo
    */
    //Definimos el vector gravitatorio usado por el world
    b2Vec2 gravity(0.0f, -10.0f);

    //Creamos el mundo con gravedad creada antes
    b2World world(gravity);

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
    b2BodyDef bodyDef;
    //ATTENTION: Hacemos que el cuerpo sea dinamico
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 4.0f);
    b2Body* body = world.CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);



    float timeStep = 1.0f / 60.0f;

    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    for (int32 i = 0; i < 60; ++i)
    {
        world.Step(timeStep, velocityIterations, positionIterations);
        b2Vec2 position = body->GetPosition();
        float angle = body->GetAngle();
        printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
    }


    
    abort();

    int posJugadorActual = 0;

    //TODO Como cambiar de jugador
    Jugador *jugadorActual;
    jugadorActual = jugadores.at(posJugadorActual);
    Gusano *gusanoActual;
    gusanoActual = jugadorActual->getGusanoActual();

    Accion ultimaAccion = Accion::MOV_QUIETO;

    while (true) {
        this->enviarEstadoAJugadores();

        Direccion accionRecibida;
        bool pudeObtenerla;
        pudeObtenerla = acciones.try_pop(accionRecibida);

        Accion accionAEjecutar;
        accionAEjecutar = this->obtenerAccion(accionRecibida, pudeObtenerla,
				      ultimaAccion);

        std::pair<int, int> cambioDeseado = gusanoActual->cambio(accionAEjecutar);

        std::pair<int, int> coordenadasIniciales = gusanoActual->getCoords();
        std::pair<int, int> coordenadasFinales;
        coordenadasFinales.first = coordenadasIniciales.first + cambioDeseado.first;
        coordenadasFinales.second = coordenadasIniciales.second + cambioDeseado.second;

        gusanoActual->setCoords(coordenadasFinales);
        this->coordsGusanos[coordenadasFinales] = gusanoActual;

        //TODO Borrar
        //Tengo que poner el if, porque sino se pisaria el puntero en
        //el caso donde ambas posiciones sean iguales aka el gusano
        //no se movio
        if (coordenadasIniciales != coordenadasFinales)
	  this->coordsGusanos[coordenadasIniciales] = nullptr;
      
        std::this_thread::sleep_for(frameDuration);
    }

}
