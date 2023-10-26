#include "cliente.h"
#include "socket.h"
#include <string>

// class Server {
// public:
//     void anadirJugadorAPartida(Protocolo&& socketDeJugador, id idPartida);
// };

Cliente::Cliente(Socket&& socket, std::vector<std::string> mapasDisponibles,
	       std::vector<std::string> partidasPosibles, TSList<Partida>& avisar) 
    : protocolo(std::move(socket)), avisar(avisar) {
    this->conectadoAPartida = false;
    this->mapasDisponibles = mapasDisponibles;
    this->partidasPosibles = partidasPosibles;
    // this->avisar = avisar;
};

//Esto corre en un thread
void Cliente::elegirPartida() {
    this->protocolo.mostrarMapas(this->mapasDisponibles);

    this->protocolo.mostrarPartidas(this->partidasPosibles);

    int partidaElejida = this->protocolo.obtenerPartidaDeseada();

    // this->avisar->anadirJugadorAPartida(std::move(this->protocolo), partidaElejida);
    this->avisar.at(partidaElejida);

    this->conectadoAPartida = true;
}
