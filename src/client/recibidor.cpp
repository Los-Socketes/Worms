#include "recibidor.h"

Recibidor::Recibidor(Protocolo& protocolo,
    Queue<std::shared_ptr<EstadoDelJuego>>& recepcion_estados,
    // Queue<std::shared_ptr<AccionCliente>>& envio_comandos,
    std::atomic<bool>& es_turno) : 
    protocolo(protocolo),
    recepcion_estados(recepcion_estados),
    // envio_comandos(envio_comandos),
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
            // if (estado_juego->segundosRestantes == 0)
            //     envio_comandos.push(std::make_shared<AccionEquiparArma>(NADA_P));
                
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
