#include "server.h"

Server::Server(const char *puerto)
    :aceptador(puerto) {
    // :recibidor(puerto) {
};

// TODO: Preguntar si este approach naive es correcto. Tecnicamente,
// esto prevendria que dos jugadores se conecten al mismo mismo tiempo
// Esta bien asi? O deberia hacerlo para que el proceso de aceptar
// sea, a su vez, multithread
void Server::recibirCliente() {
    Socket conexionEntrante = this->aceptador.accept();

    // TODO: Aca creo el protocolo, ya me empiezo a comunicar con
    // el protocolo
    Cliente clienteNuevo(std::move(conexionEntrante));

    // clienteNuevo.elegirMapaEntre(

    
};
