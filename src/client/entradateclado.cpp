#include "entradateclado.h"

EntradaTeclado::EntradaTeclado(Queue<std::shared_ptr<AccionCliente>>& envio_comandos,
    Queue<Comando>& comandos_teclado,
    Camara& camara) :
    envio_comandos(envio_comandos),
    comandos_teclado(comandos_teclado),
    cont(true),
    cuenta_regresiva(5),
    camara(camara) {}

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
                            envio_comandos.push(std::make_shared<AccionMoverGusano>(INICIO_IZQ));
                        break;
                    case SDLK_RIGHT:
                        if(evento.key.repeat == 0)
                            envio_comandos.push(std::make_shared<AccionMoverGusano>(INICIO_DER));
                        break;
                    case SDLK_UP:
                        envio_comandos.push(std::make_shared<AccionCambiarAngulo>(0.1));
                        break;
                    case SDLK_DOWN:
                        envio_comandos.push(std::make_shared<AccionCambiarAngulo>(-0.1));
                        break;
                    case SDLK_RETURN:
                        if(evento.key.repeat == 0)
                            envio_comandos.push(std::make_shared<AccionMoverGusano>(SALTO));
                        break;
                    case SDLK_BACKSPACE:
                        if(evento.key.repeat == 0)
                            envio_comandos.push(std::make_shared<AccionMoverGusano>(PIRUETA));
                        break;
                    case SDLK_q:
                        comandos_teclado.push(Comando(SALIR));
                        cont = false;
                        break;
                    case SDLK_ESCAPE:
                        if(evento.key.repeat == 0)
                            comandos_teclado.push(Comando(TOGGLE_CAMARA));
                        break;
                    case SDLK_SPACE:
                        envio_comandos.push(std::make_shared<AccionCambiarPotencia>(5));
                        break;                    
                    case SDLK_1:
                        if(evento.key.repeat == 0)
                            envio_comandos.push(std::make_shared<AccionEquiparArma>(BAZOOKA_P));
                        break;
                    case SDLK_2:
                        if(evento.key.repeat == 0)
                            envio_comandos.push(std::make_shared<AccionEquiparArma>( MORTERO_P));
                        break;
                    case SDLK_3:
                        if(evento.key.repeat == 0)
                            envio_comandos.push(std::make_shared<AccionEquiparArma>(GRANADA_VERDE_P));
                        break;
                    case SDLK_4:
                        if(evento.key.repeat == 0)
                            envio_comandos.push(std::make_shared<AccionEquiparArma>(GRANADA_ROJA_P));
                        break;
                    case SDLK_5:
                        if(evento.key.repeat == 0)
                            envio_comandos.push(std::make_shared<AccionEquiparArma>(GRANADA_SANTA_P));
                        break;
                    case SDLK_6:
                        if(evento.key.repeat == 0)
                            envio_comandos.push(std::make_shared<AccionEquiparArma>(BANANA_P));
                        break;
                    case SDLK_7:
                        if(evento.key.repeat == 0)
                            envio_comandos.push(std::make_shared<AccionEquiparArma>(DINAMITA_P));
                        break;
                    case SDLK_8:
                        if(evento.key.repeat == 0)
                            envio_comandos.push(std::make_shared<AccionEquiparArma>(BATE_P));
                        break;
                    case SDLK_9:
                        if(evento.key.repeat == 0)
                            envio_comandos.push(std::make_shared<AccionEquiparArma>(ATAQUE_AEREO_P));
                        break;
                    case SDLK_0:
                        if(evento.key.repeat == 0)
                            envio_comandos.push(std::make_shared<AccionEquiparArma>(TELETRANSPORTACION_P));
                        break;
                    case SDLK_r:
                        if(evento.key.repeat == 0)
                            envio_comandos.push(std::make_shared<AccionEquiparArma>(NADA_P));
                        break;
                    case SDLK_t:
                        cuenta_regresiva = cuenta_regresiva - 1;
                        if (cuenta_regresiva == 0)
                            cuenta_regresiva = 5;
                        envio_comandos.push(std::make_shared<AccionCambiarCuentaRegresiva>(cuenta_regresiva));
                        break;
                    case SDLK_PLUS:
                        comandos_teclado.push(Comando(VOLUMEN_MAS));
                        break;
                    case SDLK_MINUS:
                        comandos_teclado.push(Comando(VOLUMEN_MENOS));
                        break;
                    case SDLK_m:
                        comandos_teclado.push(Comando(TOGGLE_MUTEAR));
                        break;                    
                    default:
                        break;
                }
            } else if (evento.type == SDL_KEYUP) {
                switch (evento.key.keysym.sym) {
                    case SDLK_LEFT:
                        envio_comandos.push(std::make_shared<AccionMoverGusano>(FIN_IZQ));
                        break;
                    case SDLK_RIGHT:
                        envio_comandos.push(std::make_shared<AccionMoverGusano>(FIN_DER));
                        break;
                    case SDLK_SPACE:
                        envio_comandos.push(std::make_shared<AccionDisparar>());
                        envio_comandos.push(std::make_shared<AccionCambiarPotencia>(0));
                        break;
                    default:
                        break;
                }
            } else if (evento.type == SDL_MOUSEMOTION) {
                comandos_teclado.push(Comando(MOVER_CAMARA, evento.motion.xrel, evento.motion.yrel));
            } else if (evento.type == SDL_WINDOWEVENT) {
                if (evento.window.event == SDL_WINDOWEVENT_RESIZED) {
                    comandos_teclado.push(Comando(TAMANIO_VENTANA, evento.window.data1, evento.window.data2));
                }
            } else if (evento.type == SDL_MOUSEBUTTONDOWN) {
                if (evento.button.button == SDL_BUTTON_LEFT) {
                    // Obtengo la posición de la camara para calcular la posición en el mapa.
                    // Puede llegar a haber una race condition si la camara se mueve mientras
                    // se está procesando el evento, pero no es un problema grave porque se
                    // obtiene una posición aproximada.
                    comandos_teclado.push(Comando(MOVER_CURSOR, evento.button.x + camara.getPosicionX(), evento.button.y + camara.getPosicionY()));
                    std::pair<coordX, coordY> coordenadas = camara.traducirCoordenadas(evento.button.x, evento.button.y);
                    envio_comandos.push(std::make_shared<AccionCambiarCoordenadas>(coordenadas.first, coordenadas.second));
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