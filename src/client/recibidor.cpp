#include "recibidor.h"

Recibidor::Recibidor(Protocolo& protocolo, Queue<EstadoDelJuego>& recepcion_estados): 
    protocolo(protocolo), recepcion_estados(recepcion_estados), cont(true) {}

void Recibidor::run() {
    try {
        cont = true;
        while (cont) {
            EstadoDelJuego estado_juego = protocolo.obtenerEstadoDelJuego();
            recepcion_estados.push(estado_juego);                                              
        }
    } catch (const ClosedQueue& e) {
        syslog(LOG_INFO, "%s", e.what());
    }
}

void Recibidor::stop() {
    cont = false;
}
