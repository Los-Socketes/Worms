#include "recibidor.h"

Recibidor::Recibidor(Queue<EstadoJuego>& recepcion_estados): 
    recepcion_estados(recepcion_estados), cont(true) {}

void Recibidor::run() {
    try {
        // TODO: Recibir estados del juego de protocolo.

        // Por ahora solo devuelvo un estado con
        // coordenadas incrementadas de a 1.
        EstadoJuego estado_juego(0, 0);
        int i = 0;
        while (cont && i++ < 50) {
            estado_juego.coord_x++;
            estado_juego.coord_y++;
            recepcion_estados.push(estado_juego);
        }
    } catch (const ClosedQueue& e) {
        syslog(LOG_INFO, "%s", e.what());
    }
}

void Recibidor::stop() {
    cont = false;
}