#include "monitorPartida.h"
#include "partida.h"

MonitorPartida::MonitorPartida() {
    this->contador = 0;
}

id MonitorPartida::anadirPartida(const std::string mapaNombre) {
    std::unique_lock<std::mutex> lck(mtx);

    id idPartidaNueva;
    idPartidaNueva = this->contador;

    //TODO: Check new
    Partida *partidaNueva = new Partida(mapaNombre);

    // this->mapa.insert(idPartidaNueva, partidaNueva);
    this->mapa.insert({idPartidaNueva, partidaNueva});

    this->contador += 1;

    return idPartidaNueva;
}

void MonitorPartida::anadirJugadorAPartida(Jugador *nuevoJugador, id partidaEspecifica) {
    Partida *partidaRecibidora;
    partidaRecibidora = this->mapa.at(partidaEspecifica);

    partidaRecibidora->anadirJugador(nuevoJugador);
}

std::vector<RepresentacionPartida> MonitorPartida::partidasDisponibles() {
    std::vector<RepresentacionPartida> partidasAEnviar;

    //Fuente: https://stackoverflow.com/a/26282004/13683575
    for (auto const& [idPartida, partida] : this->mapa)
    {
        if (partida == nullptr)
	  continue;


        RepresentacionPartida repreActual;
        repreActual.ID = idPartida;
        partidasAEnviar.push_back(repreActual);
    }

    return partidasAEnviar;
}

MonitorPartida::~MonitorPartida() {
    //TODO free
}
