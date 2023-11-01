#include "aceptador.h"
#include "jugador.h"


Aceptador::Aceptador(const char *puerto)
    :socket(puerto) {
};

void Aceptador::aceptarClientes() {
    //TODO: Hacer que en vez de true sea socket cerrado
    while (true) {
        Socket conexionEntrante = this->socket.accept();

        Jugador *jugadorNuevo = new Jugador(std::move(conexionEntrante));
    }

}
