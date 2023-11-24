#include "monitorPartida.h"
#include "defs.h"
#include "partida.h"

MonitorPartida::MonitorPartida() {
    this->contador = 0;
}

id MonitorPartida::anadirPartida(const std::string mapaNombre) {
    std::unique_lock<std::mutex> lck(mtx);

    // recorro toda la lista para que si hay alguna libre
    // se ocupe de vuelta y los ids no suban tan rapido
    for (auto const& [idPartida, partida] : this->mapa) {
        if (partida == nullptr || !partida->is_alive()) {
            Partida *partidaNueva = new Partida(mapaNombre);
            partidaNueva->start();
            this->mapa[idPartida] = partidaNueva;
            return idPartida;
        }
    }
    // aca entra si todas las partidas estan ocupadas
    id idPartidaNueva;
    idPartidaNueva = this->contador;

    //TODO: Check new for no memory
    Partida *partidaNueva = new Partida(mapaNombre);
    partidaNueva->start();

    this->mapa.insert({idPartidaNueva, partidaNueva});

    this->contador += 1;

    return idPartidaNueva;
}

InformacionInicial MonitorPartida::obtenerInfoInicialDePartida(id partidaEspecifica) {
    Partida *partidaRecibidora;
    partidaRecibidora = this->mapa.at(partidaEspecifica);

    InformacionInicial infoInicial;
    infoInicial = partidaRecibidora->obtenerInfoInicial();


    return infoInicial;
}

void MonitorPartida::anadirJugadorAPartida(Cliente *nuevoCliente, id partidaEspecifica) {
    Partida *partidaRecibidora;
    partidaRecibidora = this->mapa.at(partidaEspecifica);

    // InformacionInicial infoInicial;
    // infoInicial = partidaRecibidora->anadirCliente(nuevoCliente);
    partidaRecibidora->anadirCliente(nuevoCliente);
}

std::vector<RepresentacionPartida> MonitorPartida::partidasDisponibles() {
    std::unique_lock<std::mutex> lck(mtx);
    std::vector<RepresentacionPartida> partidasAEnviar;

    //Fuente: https://stackoverflow.com/a/26282004/13683575
    for (auto const& [idPartida, partida] : this->mapa)
    {
        if (partida == nullptr || !partida->is_alive()) {
            continue;
        }


        RepresentacionPartida repreActual;
        repreActual.ID = idPartida;
        partidasAEnviar.push_back(repreActual);
    }

    return partidasAEnviar;
}

void MonitorPartida::reapDead() {
    for (auto const& [idPartida, partida] : this->mapa) {
        if (partida == nullptr) {
            continue;
        }
        
        if (!partida->is_alive()) {
            partida->join();
            delete partida;
            this->mapa[idPartida] = nullptr;
        }

    }
}

void MonitorPartida::kill() {
    for (auto const& [idPartida, partida] : this->mapa) {
        if (partida == nullptr || !partida->is_alive()) {
            continue;
        }

        // TODO: verificar que el destructor de partida esta bien y no leakea memoria
        partida->stop();
        partida->join();
        delete partida;
        this->mapa[idPartida] = nullptr;
    }
}

// MonitorPartida::~MonitorPartida() {
//     for (auto const& [idPartida, partida] : this->mapa) {
//         if (partida == nullptr || !partida->is_alive()) {
//             continue;
//         }

//         // TODO: verificar que el destructor de partida esta bien y no leakea memoria
//         partida->join();
//         delete partida;
//     }
// }
