#include "partida.h"
#include "jugador.h"

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
    std::vector<Gusano*> gusanosDelCliente;
    //Todos los gusanos que creamos lo anadimos al jugador y a la partida
    for (int i = 0 ;i < CANTGUSANOS; i++) {
        Gusano *nuevoGusano = new Gusano();
        gusanosDelCliente.push_back(nuevoGusano);

        this->gusanos.push_back(nuevoGusano);
    }

    this->jugadores.push_back(jugadorNuevo);
}


void Partida::gameLoop() {
    int posJugadorActual = 0;

    while (true) {
        Jugador* jugadorActual = jugadores.at(posJugadorActual);
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
