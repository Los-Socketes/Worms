#include "recibidor.h"

Recibidor::Recibidor(Protocolo& protocolo,
    Queue<std::shared_ptr<EstadoDelJuego>>& recepcion_estados,
    std::atomic<bool>& es_turno, 
    std::atomic<bool>& municiones_agotadas,
    std::atomic<bool>& ejecuto_accion) : 
    protocolo(protocolo),
    recepcion_estados(recepcion_estados),
    estado_juego(std::make_shared<EstadoDelJuego>()),
    cont(true),
    id_jugador(0),
    es_turno(es_turno),
    municiones_agotadas(municiones_agotadas),
    ejecuto_accion(ejecuto_accion) {}

void Recibidor::run() {
    try {
        cont = true;
        while (cont) {
            estado_juego = protocolo.obtenerEstadoDelJuego();
            // Me fijo si el gusano actual tiene municiones. Tengo que chequear
            // que exista el jugador, el gusano y que tenga municiones.
            if (estado_juego->gusanos.find(estado_juego->jugadorDeTurno) != estado_juego->gusanos.end() &&
                estado_juego->gusanos.at(estado_juego->jugadorDeTurno).find(estado_juego->gusanoDeTurno) !=
                    estado_juego->gusanos.at(estado_juego->jugadorDeTurno).end()) {
                // Si no tiene municiones o el arma actual es NADA_P, entonces
                // seteo el flag de municiones_agotadas.
                if (estado_juego->gusanos.at(estado_juego->jugadorDeTurno)
                    .at(estado_juego->gusanoDeTurno).armaEquipada.municiones == 0 ||
                    estado_juego->gusanos.at(estado_juego->jugadorDeTurno)
                    .at(estado_juego->gusanoDeTurno).armaEquipada.arma == NADA_P)
                    municiones_agotadas = true;
                else
                    municiones_agotadas = false;
            }
            // Me fijo si es el turno del jugador actual.
            if ((estado_juego->jugadorDeTurno == id_jugador && estado_juego->momento == EN_MARCHA) || 
                (estado_juego->momento == POR_INICIAR && id_jugador == 0)) {
                // Si no era el turno del jugador y ahora lo es, entonces
                // reinicio el flag de ejecuto_accion.
                if (!es_turno)
                    ejecuto_accion = false;
                es_turno = true;
            } else
                es_turno = false;
            if (estado_juego->momento != ESTADO_INVALIDO)
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
