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

MonitorPartida::~MonitorPartida() {
    //TODO free
}
