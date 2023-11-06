#include "aceptador.h"
#include "jugador.h"


Aceptador::Aceptador(const char *puerto)
    : socket(puerto) {};

void Aceptador::aceptarClientes() {
    //TODO: Hacer que en vez de true sea socket cerrado
    while (true) {
        Socket conexionEntrante = this->socket.accept();

        //TODO Anadir un reap dead ACA que busque en una lista de jugadores
        Jugador *jugadorNuevo = new Jugador(std::move(conexionEntrante), this->escenariosDisponibles, this->partidas);
    }

}

void Aceptador::asignar(std::vector<std::string> escenariosDisponibles) {
    this->escenariosDisponibles = escenariosDisponibles;
}



void Aceptador::kill() {
    this->socket.shutdown(SHUT_RDWR);
    this->socket.close();
}
