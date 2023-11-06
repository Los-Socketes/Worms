#include "server.h"
#include "aceptador.h"

Server::Server(const char *puerto)
    :aceptador(puerto, this->partidas) {
    this->aceptador.asignar(escenariosDisponibles);
    this->aceptador.start();
};

// TODO: que funcione este metodo
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

//TODO Pide partidas en vez de mapas
