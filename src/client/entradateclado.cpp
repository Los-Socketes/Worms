#include "entradateclado.h"

EntradaTeclado::EntradaTeclado(Queue<std::string>& envio_comandos, Queue<std::string>& comandos_teclado)
    : envio_comandos(envio_comandos), comandos_teclado(comandos_teclado), cont(true) {}

void EntradaTeclado::run() {
    try {
        cont = true;
        SDL_Event evento;
        while (cont && SDL_WaitEvent(&evento)) {
            // Capturo eventos de teclado y los envío a la cola
            // de salida hacia al servidor, o bien a la cola
            // de comandos locales de teclado para que se encargue
            // el loop principal de procesarlos.
            if (evento.type == SDL_QUIT) {
                comandos_teclado.push("salir");
                cont = false;
            } else if (evento.type == SDL_KEYDOWN) {
                switch (evento.key.keysym.sym) {
                    case SDLK_LEFT:
                        if(evento.key.repeat == 0)
                            envio_comandos.push("Presionando izquierda");
                        break;
                    case SDLK_RIGHT:
                        if(evento.key.repeat == 0)
                            envio_comandos.push("Presionando derecha");
                        break;
                    case SDLK_RETURN:
                        if(evento.key.repeat == 0)
                            envio_comandos.push("Enter");
                        break;
                    case SDLK_BACKSPACE:
                        if(evento.key.repeat == 0)
                            envio_comandos.push("Retroceso");
                        break;
                    case SDLK_q:
                        comandos_teclado.push("q");
                        break;
                    default:
                        break;
                }
            } else if (evento.type == SDL_KEYUP) {
                switch (evento.key.keysym.sym) {
                    case SDLK_LEFT:
                        envio_comandos.push("Soltando izquierda");
                        break;
                    case SDLK_RIGHT:
                        envio_comandos.push("Soltando derecha");
                        break;
                    default:
                        break;
                }
            }
        }
    } catch (const ClosedQueue& e) {
        syslog(LOG_INFO, "%s", e.what());
    }
}

void EntradaTeclado::stop() {
    cont = false;
}
