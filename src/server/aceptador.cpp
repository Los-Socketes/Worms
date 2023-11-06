#include "aceptador.h"
#include "jugador.h"


Aceptador::Aceptador(const char *puerto)
    : socket(puerto) {};

void Aceptador::aceptarClientes() {
    //TODO: Hacer que en vez de true sea socket cerrado
    while (true) {
        Socket conexionEntrante = this->socket.accept();

        //TODO Anadir un reap dead ACA que busque en una lista de jugadores
        Cliente* cliente = new Cliente(std::move(conexionEntrante), this->escenariosDisponibles, std::ref(this->partidas));
        // Jugador *jugadorNuevo = new Jugador(std::move(conexionEntrante), this->escenariosDisponibles, this->partidas);

        // Primero el reap_dead para que la lista tenga un elemento menos
        // reap_dead();
        this->listaClientes.push_back(cliente);
    }

}

void Aceptador::asignar(std::vector<std::string> escenariosDisponibles) {
    this->escenariosDisponibles = escenariosDisponibles;
}

// void Aceptador::reap_dead() {
//     this->listaClientes.remove_if([](Jugador* c) {
//         if (!c->get_is_alive()) {
//             delete c;
//             return true;
//         }
//         return false;
//     });
// }

void Aceptador::kill() {
    this->socket.shutdown(SHUT_RDWR);
    this->socket.close();
}
