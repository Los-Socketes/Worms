#include "partida.h"
#include "defs.h"
#include "gusano.h"
#include "jugador.h"
#include "protocolo.h"
#include <cstdlib>
#include <unistd.h>
#include <utility>

#define SLEEPSEGS 1

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

    int posJugadorActual = 0;

    //TODO Como cambiar de jugador
    Jugador *jugadorActual;
    jugadorActual = jugadores.at(posJugadorActual);
    Gusano *gusanoActual;
    gusanoActual = jugadorActual->getGusanoActual();

    Accion ultimaAccion = Accion::MOV_QUIETO;

    while (true) {
        sleep(SLEEPSEGS);
        this->enviarEstadoAJugadores();

        Direccion accionRecibida;
        bool pudeObtenerla;
        pudeObtenerla = acciones.try_pop(accionRecibida);

        // if (accionRecibida == INICIO_DER || accionRecibida == INICIO_IZQ)
        // 	  gusanoActual->ponerEnMovimiento();

        // if (accionRecibida == FIN_DER || accionRecibida == FIN_IZQ)
        // 	  gusanoActual->detener();

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
        //Tengo que poner el if, porque sino se pisaria el puntero en
        //el caso donde ambas posiciones sean iguales aka el gusano
        //no se movio
        if (coordenadasIniciales != coordenadasFinales)
	  this->coordsGusanos[coordenadasIniciales] = nullptr;
    }

}
