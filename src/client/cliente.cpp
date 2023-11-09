#include "cliente.h"

Cliente::Cliente(Socket&& skt):
    sdl(SDL_INIT_VIDEO),
    protocolo(std::move(skt)),
    estado_juego(),
    camara(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0),
    menu(protocolo),
    recepcion_estados(TAM_QUEUE),
    envio_comandos(TAM_QUEUE),
    comandos_teclado(TAM_QUEUE),
    entrada_teclado(envio_comandos, comandos_teclado),
    recibidor(protocolo, recepcion_estados),
    enviador(protocolo, envio_comandos) {
        //WARNING todo esto es momentaneo para que compile
        std::vector<RepresentacionGusano> listaGusanosIniciales;
        RepresentacionGusano gusi;
        gusi.idGusano = 0;
        gusi.vida = 100;
        gusi.dir = DERECHA;
        // gusi.estado = 
        gusi.posicion = std::pair<int, int>(0,0);
        // gusi.armaEquipada = NADA_P;
        listaGusanosIniciales.push_back(gusi);

        std::map<idJugador, std::vector<RepresentacionGusano>> gusanosNuevos;
        gusanosNuevos.insert({0, listaGusanosIniciales});

        estado_juego.gusanos = gusanosNuevos;
        // listaGusanosIniciales.
    }

void Cliente::iniciar() {
    entrada_teclado.start();
    enviador.start();
    recibidor.start();
}

// TODO: pasar el grupo de entidades a reenderizar. Estas deberían tener asignadas
// las animaciones correspondientes.
void Cliente::renderizar(Renderer& renderizador, Animacion& caminar, Animacion& agua, int it) {
    renderizador.Clear();

    // TODO: dibujar fondo.

    // Dibujo el agua, tiene dimensiones 128 * 50, así que tendré que 
    // dibujarla varias veces para que ocupe toda la parte inferior de la pantalla
    // en varias filas. TODO: Encapsular en una clase?
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < (MAPA_ANCHO / 128 + 1); j++) {
            agua.dibujar(camara, j * 128, MAPA_ALTO - 100 + 10 * (i + 1), false, it + 3*(i+1), 2);
        }
    }

    // Dibujo todos los gusanos.
    for (auto& par : estado_juego.gusanos) {
        for (auto& gusano : par.second) {
            caminar.dibujar(camara, gusano.posicion.enX, gusano.posicion.enY, gusano.dir == DERECHA, it, 1);
        }
    }
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < (MAPA_ANCHO / 128 + 1); j++) {
            agua.dibujar(camara, j * 128, MAPA_ALTO - 80 + 10 * (i + 1), false, it + 3*(i+3), 2);
        }
    }
    // Actualizo ventana.
    renderizador.Present();
}

bool Cliente::ejecutar_menu() {
    return menu.ejecutar();
}

void Cliente::loop_principal() {
    // Inicializar SDL.  
    Window ventana("Worms", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    Renderer renderizador(ventana, -1, SDL_RENDERER_ACCELERATED);

    // TODO: tener un mapa de texturas y/o animaciones ya inicializadas.
    Animacion agua(renderizador, "assets/sprites/water.png", 128, 100, 12, false);
    Animacion caminar(renderizador, "assets/sprites/wwalk.png", 60, 60, 15, true);

    // TODO: obtener info del mapa desde el menu.
    camara.setDimensionMapa(MAPA_ANCHO, MAPA_ALTO);

    iniciar();

    int it = 0;
    int tick_anterior = SDL_GetTicks();
    int rate = 1000 / FPS;
    bool continuar = true;

    bool mover_camara = true;

    while (continuar) {
        // Actualizo el estado del juego.
        recepcion_estados.try_pop(estado_juego);

        // Chequeo comandos de teclado.
        Comando comando;
        while (comandos_teclado.try_pop(comando)) {
            switch (comando.tipo) {
                case SALIR:
                    continuar = false;
                    break;
                case MOVER_CAMARA:
                    if (mover_camara) {
                        camara.mover(comando.parametros.first, comando.parametros.second);
                    }
                    break;
                case TOGGLE_CAMARA:
                    mover_camara = !mover_camara;
                    break;
                case TAMAÑO_VENTANA:
                    camara.setDimension(comando.parametros.first, comando.parametros.second);
                    break;
                default:
                    break;
            }
        }

        // Renderizo.
        renderizar(renderizador, caminar, agua, it);

        // Constant rate loop.
        int tick_actual = SDL_GetTicks();
        int descanso = rate - (tick_actual - tick_anterior);

        if (descanso < 0) {
            int ticks_detras = -descanso;
            int ticks_perdidos = ticks_detras / ticks_detras % rate;
            tick_anterior += ticks_perdidos;
            it += int(ticks_detras / rate);
        }
        else {
            SDL_Delay(descanso);
        }
    
        tick_anterior += rate;
        it++;

    }
}

Cliente::~Cliente() {
    comandos_teclado.close();
    recepcion_estados.close();
    envio_comandos.close();
    entrada_teclado.stop();
    enviador.stop();
    recibidor.stop();
    entrada_teclado.join();
    enviador.join();
    recibidor.join();
}     
