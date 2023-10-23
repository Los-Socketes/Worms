#include "server.h"

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

// TODO: Preguntar si este approach naive es correcto. Tecnicamente,
// esto prevendria que dos jugadores se conecten al mismo mismo tiempo
// Esta bien asi? O deberia hacerlo para que el proceso de aceptar
// sea, a su vez, multithread
void Server::recibirCliente() {
    Socket conexionEntrante = this->aceptador.accept();

    // TODO: Aca creo el protocolo, ya me empiezo a comunicar con
    // el protocolo
    Cliente clienteNuevo(std::move(conexionEntrante));

    // clienteNuevo.mostrar
    clienteNuevo.mostrar(this->escenariosDisponibles);
    // clienteNuevo.elegirMapaEntre(

    
};
