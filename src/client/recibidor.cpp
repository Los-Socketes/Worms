#include "recibidor.h"

Recibidor::Recibidor(Protocolo& protocolo, Queue<EstadoJuego>& recepcion_estados): 
    protocolo(protocolo), recepcion_estados(recepcion_estados), cont(true) {}

// Recibidor::Recibidor(Queue<EstadoJuego>& recepcion_estados): 
//     recepcion_estados(recepcion_estados), cont(true) {}

void Recibidor::run() {
    try {
        cont = true;
        // TODO: Recibir estados del juego de protocolo.

        // Por ahora solo devuelvo un estado con
        // coordenadas incrementadas de a 1.
        EstadoJuego estado_juego(50, 250, 1);
        int dirY = 1;
        while (cont) {
            
            if (estado_juego.dir_x == 1) {
                estado_juego.coord_x++;
            } else if (estado_juego.dir_x == 0) {
                estado_juego.coord_x--;
            }
            /*
            if (dirY == 1) {
                estado_juego.coord_y++;
            } else if (dirY == 0) {
                estado_juego.coord_y--;
            }
            */
            if (estado_juego.coord_x == 600) {
                estado_juego.dir_x = 0;
            } else if (estado_juego.coord_x == 0) {
                estado_juego.dir_x = 1;
            }
            /*
            if (estado_juego.coord_y == 200) {
                dirY = 0;
            } else if (estado_juego.coord_y == 0) {
                dirY = 1;
            }
            */
            
            recepcion_estados.push(estado_juego);
            
            SDL_Delay(1000 / 60);
        }
    } catch (const ClosedQueue& e) {
        syslog(LOG_INFO, "%s", e.what());
    }
}

void Recibidor::stop() {
    cont = false;
}
