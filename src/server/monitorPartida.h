#ifndef TSMAP_H
#define TSMAP_H

#include <map>
#include <mutex>
#include "defs.h"
#include "partida.h"

typedef int id;

class MonitorPartida {

    std::mutex mtx;

    id contador;
    std::map<id, Partida*> mapa;

public:
    MonitorPartida();

    [[nodiscard]] id anadirPartida(std::string mapaNombre);

    [[nodiscard]] InformacionInicial anadirJugadorAPartida(Cliente *nuevoJugador, id partidaEspecifica);

    [[nodiscard]] std::vector<RepresentacionPartida> partidasDisponibles();

    ~MonitorPartida();

};


#endif
