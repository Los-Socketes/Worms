#include "aceptador.h"
#include "jugador.h"


Aceptador::Aceptador(const char *puerto, MonitorPartida& partidas)
                     // std::vector<std::string> escenariosDisponibles,
                     // MonitorPartida &partidas)
    // : socket(puerto), escenariosDisponibles(escenariosDisponibles),
    //   partidas(partidas)
    : socket(puerto),
      partidas(partidas)
{
};

void Aceptador::aceptarClientes() {
    //TODO: Hacer que en vez de true sea socket cerrado
    while (true) {
        Socket conexionEntrante = this->socket.accept();

        Jugador *jugadorNuevo = new Jugador(std::move(conexionEntrante), this->escenariosDisponibles, this->partidas);
    }

}

void Aceptador::asignar(std::vector<std::string> escenariosDisponibles) {
    this->escenariosDisponibles = escenariosDisponibles;
}
