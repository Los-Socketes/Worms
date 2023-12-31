#include "server.h"
#include "aceptador.h"

Server::Server(const char *puerto)
    :aceptador(puerto) {
    this->aceptador.start();
};

void Server::esperarQueSeCierre() {
    std::string input;
    while (std::cin >> input) {
        if (input == "q") {
            this->aceptador.kill();
            break;
        }
    };
    this->aceptador.join();
}
