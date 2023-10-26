#include "server.h"
#include "cliente.h"
#include <vector>

Server::Server(const char *puerto)
    :aceptador(puerto) {
    // :recibidor(puerto) {
};

std::vector<RepresentacionPartida> Server::getRepresentacionPartidas() {
    std::vector<RepresentacionPartida> ids;
    id pos = 0;
    for (id pos = 0 ; pos < this->partidas.size() ; pos ++) {
        // El dia que tengamos que enviar mas datos, simplemente
        // es anadir mas valores al struct
        RepresentacionPartida nuevaRepre;
        nuevaRepre.ID = pos;

        ids.push_back(nuevaRepre);
    }

    return ids;
}

void Server::recibirCliente() {
    //TODO: Hacer que en vez de true sea socket cerrado
    while (true) {
        Socket conexionEntrante = this->aceptador.accept();

        // Le pasamos toda la informacion actualmente presente al cliente
        // El cliente va a elejir en base a ESA informacion
        std::vector<RepresentacionPartida> partidasDisponibles = this->getRepresentacionPartidas();

        //Esto castea a string.
        //Fuente:https://stackoverflow.com/a/6399098/13683575
        std::vector<std::string> representacionPartidasDisponibles(partidasDisponibles.begin(), partidasDisponibles.end());

        Cliente *clienteNuevo = new Cliente(std::move(conexionEntrante),
				    this->escenariosDisponibles,
				    representacionPartidasDisponibles,
				    this->partidas);
        clienteNuevo->start();

        //TODO reap dead
        this->lobby.push_back(clienteNuevo);
    }
};

// void Server::anadirJugadorAPartida(Protocolo &&protocoloJugador, id idPartida) {

// };
