#include "cliente.h"

Cliente::Cliente(Socket&& skt, ConfiguracionCliente& config):
    sdl(SDL_INIT_VIDEO),
    ttf(),
    protocolo(std::move(skt)),
    config(config),
    estado_juego(std::make_shared<EstadoDelJuego>()),
    camara(0, 0, config.getDimensionesIniciales().first, config.getDimensionesIniciales().second, 0, 0),
    dibujador(camara, estado_juego, config.getDimensionesIniciales().first, config.getDimensionesIniciales().second),
    control_iteracion(estado_juego),
    menu(protocolo),
    recepcion_estados(TAM_QUEUE),
    envio_comandos(TAM_QUEUE),
    comandos_teclado(TAM_QUEUE),
    es_turno(false),
    municiones_agotadas(true),
    inicio(false),
    entrada_teclado(envio_comandos, comandos_teclado, camara, municiones_agotadas),
    recibidor(protocolo, recepcion_estados, es_turno, municiones_agotadas),
    // recibidor(protocolo, recepcion_estados, envio_comandos, es_turno),
    enviador(protocolo, envio_comandos, es_turno),
    pos_cursor(0, 0),
    volumen(config.getVolumenInicial()),
    muteado(false) {}

void Cliente::iniciar() {
    entrada_teclado.start();
    enviador.start();
    recibidor.start();
    inicio = true;
}

InformacionInicial Cliente::ejecutar_menu(int argc, char* argv[]) {
    return menu.ejecutar(argc, argv);
}

void Cliente::loop_principal(InformacionInicial& info_inicial) {
    // Inicializar SDL.  
    Window ventana("Worms - Jugador " + std::to_string(info_inicial.jugador),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        config.getDimensionesIniciales().first,
        config.getDimensionesIniciales().second,
        SDL_WINDOW_RESIZABLE);
    Renderer renderizador(ventana, -1, SDL_RENDERER_ACCELERATED);
    Mixer mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);

    Music music("assets/sounds/music.ogg");

    mixer.FadeInMusic(music, -1, 2000);
    mixer.SetMusicVolume(volumen);
    mixer.SetVolume(-1, volumen);

    camara.setDimensionMapa(info_inicial.dimensiones.first, info_inicial.dimensiones.second);
    dibujador.setDimensionMapa(info_inicial.dimensiones.first, info_inicial.dimensiones.second);

    // Inicializar animaciones.
    dibujador.inicializar(renderizador, mixer);

    // Seteo el id del jugador.
    recibidor.setIdJugador(info_inicial.jugador - 1);
    dibujador.setIdJugador(info_inicial.jugador - 1);
    printf("Mi id es: %d\n", info_inicial.jugador - 1);

    // Obtengo colores de los jugadores.
    std::vector<colorJugador> colores = config.getColoresJugadores();

    iniciar();

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

        // Actualizo entidades en el iterador.
        control_iteracion.actualizarEntidades();     

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
                case MOVER_CURSOR:
                    pos_cursor.first = comando.parametros.first;
                    pos_cursor.second = comando.parametros.second;
                    break;
                case VOLUMEN_MAS:
                    if (volumen + 8 <= MIX_MAX_VOLUME) {
                        volumen += 8;
                        mixer.SetMusicVolume(volumen);
                        mixer.SetVolume(-1, volumen);
                    }
                    break;
                case VOLUMEN_MENOS:
                    if (volumen - 8 >= 0) {
                        volumen -= 8;
                        mixer.SetMusicVolume(volumen);
                        mixer.SetVolume(-1, volumen);
                    }
                    break;
                case TOGGLE_MUTEAR:
                    if (muteado) {
                        muteado = false;
                        mixer.SetMusicVolume(volumen);
                        mixer.SetVolume(-1, volumen);
                    }
                    else {
                        muteado = true;
                        mixer.SetMusicVolume(0);
                        mixer.SetVolume(-1, 0);
                    }
                    break;
                case SIN_MUNICIONES:
                    dibujador.reproducirSonido(SONIDO_SIN_MUNICIONES);
                    break;
                default:
                    break;
            }
        }

        // Renderizo.
        dibujador.dibujar(renderizador, control_iteracion, info_inicial.vigas, pos_cursor, colores);

        // Constant rate loop.
        int tick_actual = SDL_GetTicks();
        int descanso = rate - (tick_actual - tick_anterior);

        if (descanso < 0) {
            int ticks_detras = -descanso;
            int ticks_perdidos = ticks_detras - ticks_detras % rate;
            tick_anterior += ticks_perdidos;
            control_iteracion.aumentarIteraciones(ticks_detras / rate);
        }
        else {
            SDL_Delay(descanso);
        }
    
        tick_anterior += rate;
        control_iteracion.aumentarIteraciones(1);

    }
}

bool Cliente::salioDesdeMenu() {
    return menu.salioDelMenu();
}

Cliente::~Cliente() {
    if (inicio) {  
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
}     
