#include "server.h"
#include "aceptador.h"

Server::Server(const char *puerto)
    // :aceptador(puerto, this->escenariosDisponibles, this->partidas)
    :aceptador(puerto, this->partidas)
{
    // this->aceptador = Aceptador(puerto, this->escenariosDisponibles, this->partidas);
    this->aceptador.asignar(escenariosDisponibles);
    this->aceptador.aceptarClientes();
};

void Server::esperarQueSeCierre() {
    std::string input;
    while (std::cin >> input) {
        if (input == "q") {
	  //TODO Crear este metodo
            // this->aceptador.kill();
            break;
        }
    };
    this->aceptador.join();
}
