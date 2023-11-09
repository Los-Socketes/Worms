#include "entradateclado.h"

EntradaTeclado::EntradaTeclado(Queue<std::shared_ptr<AccionCliente>>& envio_comandos, Queue<Comando>& comandos_teclado)
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
                comandos_teclado.push(Comando(SALIR));
                cont = false;
            } else if (evento.type == SDL_KEYDOWN) {
                switch (evento.key.keysym.sym) {
                    case SDLK_LEFT:
                        if(evento.key.repeat == 0)
                            envio_comandos.push(std::make_shared<AccionMoverGusano>(0, INICIO_IZQ));
                        break;
                    case SDLK_RIGHT:
                        if(evento.key.repeat == 0)
                            envio_comandos.push(std::make_shared<AccionMoverGusano>(0, INICIO_DER));
                        break;
                    case SDLK_RETURN:
                        if(evento.key.repeat == 0)
                            envio_comandos.push(std::make_shared<AccionMoverGusano>(0, SALTO));
                        break;
                    case SDLK_BACKSPACE:
                        if(evento.key.repeat == 0)
                            envio_comandos.push(std::make_shared<AccionMoverGusano>(0, PIRUETA));
                        break;
                    case SDLK_q:
                        comandos_teclado.push(Comando(SALIR));
                        cont = false;
                        break;
                    default:
                        break;
                }
            } else if (evento.type == SDL_KEYUP) {
                switch (evento.key.keysym.sym) {
                    case SDLK_LEFT:
                        envio_comandos.push(std::make_shared<AccionMoverGusano>(0, FIN_IZQ));
                        break;
                    case SDLK_RIGHT:
                        envio_comandos.push(std::make_shared<AccionMoverGusano>(0, FIN_DER));
                        break;
                    default:
                        break;
                }
            } else if (evento.type == SDL_MOUSEMOTION) {
                comandos_teclado.push(Comando(MOVER_CAMARA, evento.motion.xrel, evento.motion.yrel));
            }
        }
    } catch (const ClosedQueue& e) {
        syslog(LOG_INFO, "%s", e.what());
    }
}

void EntradaTeclado::stop() {
    cont = false;
}
