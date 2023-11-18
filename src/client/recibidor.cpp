#include "recibidor.h"

Recibidor::Recibidor(Protocolo& protocolo, Queue<std::shared_ptr<EstadoDelJuego>>& recepcion_estados, std::atomic<bool>& es_turno) : 
    protocolo(protocolo),
    recepcion_estados(recepcion_estados),
    estado_juego(std::make_shared<EstadoDelJuego>()),
    cont(true),
    id_jugador(0),
    es_turno(es_turno) {}

void Recibidor::run() {
    try {
        cont = true;
        while (cont) {
            estado_juego = protocolo.obtenerEstadoDelJuego();
            if (estado_juego->jugadorDeTurno == id_jugador)
                es_turno = true;
            else
                es_turno = false;
            recepcion_estados.push(estado_juego);                                              
        }
    } catch (const ClosedQueue& e) {
        syslog(LOG_INFO, "%s", e.what());
    } 
}

void Recibidor::setIdJugador(idJugador id_jugador) {
    this->id_jugador = id_jugador;
}

void Recibidor::stop() {
    cont = false;
}
