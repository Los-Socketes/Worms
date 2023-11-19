#include "cliente.h"

Cliente::Cliente(Socket&& skt):
    sdl(SDL_INIT_VIDEO),
    ttf(),
    protocolo(std::move(skt)),
    estado_juego(std::make_shared<EstadoDelJuego>()),
    camara(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0),
    dibujador(camara, estado_juego, SCREEN_WIDTH, SCREEN_HEIGHT),
    menu(protocolo),
    recepcion_estados(TAM_QUEUE),
    envio_comandos(TAM_QUEUE),
    comandos_teclado(TAM_QUEUE),
    es_turno(false),
    entrada_teclado(envio_comandos, comandos_teclado),
    recibidor(protocolo, recepcion_estados, es_turno),
    enviador(protocolo, envio_comandos, es_turno) {
        //WARNING todo esto es momentaneo para que compile
        std::vector<RepresentacionGusano> listaGusanosIniciales;
        RepresentacionGusano gusi;
        gusi.idGusano = 0;
        gusi.vida = 100;
        gusi.dir = DERECHA;
        gusi.estado = QUIETO;
        gusi.posicion = std::pair<int, int>(0,0);
        gusi.armaEquipada.arma = NADA_P;
        gusi.armaEquipada.tieneMira = false;
        gusi.armaEquipada.tienePotenciaVariable = false;
        gusi.armaEquipada.tieneCuentaRegresiva = false;
        listaGusanosIniciales.push_back(gusi);

        std::map<idJugador, std::vector<RepresentacionGusano>> gusanosNuevos;
        gusanosNuevos.insert({0, listaGusanosIniciales});

        estado_juego->gusanos = gusanosNuevos;
        // listaGusanosIniciales.
    }

void Cliente::iniciar() {
    entrada_teclado.start();
    enviador.start();
    recibidor.start();
}

InformacionInicial Cliente::ejecutar_menu(int argc, char* argv[]) {
    return menu.ejecutar(argc, argv);
}

void Cliente::loop_principal(InformacionInicial& info_inicial) {
    // Inicializar SDL.  
    Window ventana("Worms", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    Renderer renderizador(ventana, -1, SDL_RENDERER_ACCELERATED);

    // TODO: obtener info del mapa desde el servidor.
    int ancho_mapa = MAPA_ANCHO;
    int alto_mapa = MAPA_ALTO;

    camara.setDimensionMapa(ancho_mapa, alto_mapa);
    dibujador.setDimensionMapa(ancho_mapa, alto_mapa);

    // Inicializar animaciones.
    dibujador.inicializarAnimaciones(renderizador);

    // Seteo el id del jugador.
    recibidor.setIdJugador(info_inicial.jugador - 1);

    iniciar();

    int it = 0;
    int tick_anterior = SDL_GetTicks();
    int rate = 1000 / FPS;
    bool continuar = true;

    bool mover_camara = true;

    // Imprimo las vigas.
    for (auto& viga : info_inicial.vigas) {
        std::cout << "Viga: " << viga.posicionInicial.first << " " << viga.posicionInicial.second << " " << viga.longitud << " " << viga.angulo << std::endl;
    }

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
                    if (mover_camara)
                        camara.mover(comando.parametros.first, comando.parametros.second);
                    break;
                case TOGGLE_CAMARA:
                    mover_camara = !mover_camara;
                    break;
                case TAMANIO_VENTANA:
                    camara.setDimension(comando.parametros.first, comando.parametros.second);
                    break;
                default:
                    break;
            }
        }

        // Renderizo.
        // Temporalmente solo utilizo el arma del primer gusano del primer jugador.
        dibujador.dibujar(renderizador, it, info_inicial.vigas);

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
