#include "server.h"
#include "cliente.h"
#include <vector>

Server::Server(const char *puerto)
    :aceptador(puerto) {
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
