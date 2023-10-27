#include "partida.h"
#include "jugador.h"
#include "protocolo.h"
#include <unistd.h>

#define SLEEPSEGS 3000

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
        Gusano *nuevoGusano = new Gusano();
        gusanosParaElCliente.push_back(nuevoGusano);

        //Anadimos los gusanos del cliente a la partida
        this->gusanos.append(nuevoGusano);
    }
    //Le damos los gusanos al cliente
    jugadorNuevo->obtenerGusanosIniciales(gusanosParaElCliente);

    //Anadimos al jugador a la partida
    this->jugadores.append(jugadorNuevo);
}


void Partida::gameLoop() {
    int posJugadorActual = 0;

    while (true) {
        sleep(SLEEPSEGS);

        Jugador* jugadorActual = jugadores.at(posJugadorActual);

        //Non blocking pop (try pop)
        bool pudeObtenerla;
        Direccion accionAEjecutar;
        pudeObtenerla = jugadorActual->obtenerAccion(accionAEjecutar);

        //Si no obtuvimos nada, vamos a la siguiente iteracion.
        //Sigue siendo el turno del mismo jugador y de el mismo gusano
        if (pudeObtenerla == false)
	  continue;

        // Gusano& gusanoActual = jugadorActual.getGusanoActual();

        //TODO: Cambiar a algo mas generico cuando tengamos las armas
        // int jugadorActual.getAccion();

        //TODO: Implementar "calcular cambios"
        //std::list<std::pair<posX,posY> areasAfectadas = this.calcularCambios(Accion);

        //TODO: Implementar "actualizar"
        //this.actualizar();


        posJugadorActual += 1;

    }
};
