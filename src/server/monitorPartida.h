#ifndef TSMAP_H
#define TSMAP_H

#include <map>
#include <mutex>
#include "partida.h"

typedef int id;

class MonitorPartida {

    std::mutex mtx;

    id contador;
    std::map<id, Partida*> mapa;

public:
    MonitorPartida();

    [[nodiscard]] id anadirPartida(std::string mapaNombre);

    void anadirJugadorAPartida(Jugador *nuevoJugador, id partidaEspecifica);

    [[nodiscard]] std::vector<RepresentacionPartida> partidasDisponibles();

    ~MonitorPartida();

};


#endif
