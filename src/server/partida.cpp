#include "partida.h"
#include "defs.h"
#include "jugador.h"
#include "protocolo.h"
#include <unistd.h>

#define SLEEPSEGS 3

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
inline void Partida::enviarEstadoAJugadores() {
    EstadoDelJuego estadoActual;
    for (auto const& [posicion, gusano] : this->coordsGusanos) {
	  estadoActual.posicion = posicion;

	  DireccionGusano direccionPresente;
	  direccionPresente = gusano->getDireccion(); 
	  estadoActual.dir = direccionPresente;
        }

    for(Jugador *jugador : this->jugadores) {
        jugador->enviarEstadoJuego(estadoActual);
    }
}

void Partida::gameLoop() {
    std::unique_lock<std::mutex> lck(mtx);

    //Esperamos hasta que se unan todos los jugadores necesarios
    while (this->jugadores.size() < MINJUGADORES)
        this->seUnioJugador.wait(lck);

    while (true) {
        sleep(SLEEPSEGS);
        this->enviarEstadoAJugadores();
        Direccion accionRecibida;
        bool pudeObtenerla;
        pudeObtenerla = acciones.try_pop(accionRecibida);

        
    }

}
