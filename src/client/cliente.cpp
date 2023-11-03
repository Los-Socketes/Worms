#include "cliente.h"

Cliente::Cliente(Socket&& skt):
    protocolo(std::move(skt)),
    estado_juego(0,0,1),
    menu(protocolo),
    recepcion_estados(TAM_QUEUE),
    envio_comandos(TAM_QUEUE),
    comandos_teclado(TAM_QUEUE),
    entrada_teclado(envio_comandos, comandos_teclado),
    recibidor(protocolo, recepcion_estados),
    enviador(protocolo, envio_comandos) {}

void Cliente::iniciar() {
    entrada_teclado.start();
    enviador.start();
    recibidor.start();
}

void Cliente::renderizar(Renderer& renderizador, Animacion& caminar) {
    renderizador.Clear();

    caminar.siguiente_frame(estado_juego.coord_x, estado_juego.coord_y, estado_juego.dir_x);

    // Actualizo ventana.
    renderizador.Present();
}

bool Cliente::ejecutar_menu() {
    return menu.ejecutar();
}

void Cliente::loop_principal() {
    // Inicializar SDL.  
    SDL sdl(SDL_INIT_VIDEO);
    Window ventana("Worms", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    Renderer renderizador(ventana, -1, SDL_RENDERER_ACCELERATED);

    // TODO: tener un mapa de texturas y/o animaciones ya inicializadas.
    Animacion caminar(renderizador, "assets/sprites/wwalk.png", 15);

    iniciar();

    bool continuar = true;
    while (continuar) {
        // Actualizo el estado del juego.
        recepcion_estados.try_pop(estado_juego);

        // Renderizo.
        renderizar(renderizador, caminar);

        // Chequeo comandos de teclado.
        std::string comando;
        if (comandos_teclado.try_pop(comando)) {
            if (comando == "q" || comando == "salir") {
                continuar = false;
            }
        }
                
        // Delay para controlar el framerate.
        // TODO: implementar constant rate loop.
        SDL_Delay(1000 / 60);
    }
}

Cliente::~Cliente() {
    comandos_teclado.close();
    entrada_teclado.stop();
    entrada_teclado.join();
    recepcion_estados.close();
    envio_comandos.close();
    enviador.stop();
    recibidor.stop();
    enviador.join();
    recibidor.join();
}     