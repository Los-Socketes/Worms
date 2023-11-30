#include "cliente.h"

Cliente::Cliente(Socket&& skt, ConfiguracionCliente& config):
    config(config),
    sdl(SDL_INIT_VIDEO),
    ttf(),
    ventana("Worms", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, config.getDimensionesIniciales().first, config.getDimensionesIniciales().second, SDL_WINDOW_HIDDEN),
    renderizador(ventana, -1, SDL_RENDERER_ACCELERATED),
    mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096),
    music("assets/sounds/music.ogg"),
    protocolo(std::move(skt)),
    estado_juego(std::make_shared<EstadoDelJuego>()),
    es_host(false),
    camara(0, 0, config.getDimensionesIniciales().first, config.getDimensionesIniciales().second, 0, 0),
    dibujador(renderizador, mixer, camara, estado_juego, config.getDimensionesIniciales().first, config.getDimensionesIniciales().second),
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
    enviador(protocolo, envio_comandos, es_turno),
    pos_cursor(0, 0),
    volumen(config.getVolumenInicial()),
    muteado(false) {}

void Cliente::configurarSDL(InformacionInicial& info_inicial) {
    ventana.SetResizable(true);
    ventana.Show();
    ventana.SetTitle("Worms - Jugador " + std::to_string(info_inicial.jugador));
    mixer.FadeInMusic(music, -1, 2000);
    mixer.SetMusicVolume(volumen);
    mixer.SetVolume(-1, volumen);
}

void Cliente::configurarCamara(InformacionInicial& info_inicial) {
    camara.setDimensionMapa(info_inicial.dimensiones.first, info_inicial.dimensiones.second);
}

void Cliente::configurarDibujador(InformacionInicial& info_inicial) {
    dibujador.setDimensionMapa(info_inicial.dimensiones.first, info_inicial.dimensiones.second);
    dibujador.setIdJugador(info_inicial.jugador - 1);
}

void Cliente::iniciarHilos(InformacionInicial& info_inicial) {
    recibidor.setIdJugador(info_inicial.jugador - 1);
    entrada_teclado.start();
    enviador.start();
    recibidor.start();
    inicio = true;
}

void Cliente::ejecutarComandoTeclado(Comando& comando, bool& continuar, bool& mover_camara) {
    switch (comando.tipo) {
        case SALIR:
            continuar = false;
            break;
        case MOVER_CAMARA:
            if (mover_camara)
                camara.moverConMouse(comando.parametros.first, comando.parametros.second);
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
            if (volumen <= MIX_MAX_VOLUME) {
                volumen += 8;
                if (volumen > MIX_MAX_VOLUME) {
                    volumen = MIX_MAX_VOLUME;
                }
                mixer.SetMusicVolume(volumen);
                mixer.SetVolume(-1, volumen);
                if (muteado) {
                    muteado = false;
                }
            }
            break;
        case VOLUMEN_MENOS:
            if (volumen > 0) {
                volumen -= 8;
                if (volumen < 0) {
                    volumen = 0;
                }
                mixer.SetMusicVolume(volumen);
                mixer.SetVolume(-1, volumen);
                if (muteado) {
                    muteado = false;
                }
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

void Cliente::actualizarObjetivoCamara() {
    // Busco en estado de juego algún objetivo en este orden de prioridad:
    // 1) Proyectil no explotado.
    // 2) Gusano no quieto.
    // 3) Gusano de turno.
    bool encontre = false;
    for (auto& proyectil : estado_juego->proyectiles) {
        if (!proyectil.exploto) {
            camara.actualizarObjetivo(proyectil.posicion.first, proyectil.posicion.second);
            encontre = true;
            break;
        }
    }
    if (!encontre) {
        for (auto& jugador : estado_juego->gusanos) {
            for(auto& gusano : jugador.second) {
                if (gusano.second.estado != QUIETO && gusano.second.estado != MUERTO) {
                    camara.actualizarObjetivo(gusano.second.posicion.first, gusano.second.posicion.second);
                    encontre = true;
                    break;
                }
            }
        }
    }
    if (!encontre) {
        for (auto& jugador : estado_juego->gusanos) {
            for(auto& gusano : jugador.second) {
                if (gusano.second.idGusano == estado_juego->gusanoDeTurno) {
                    camara.actualizarObjetivo(gusano.second.posicion.first, gusano.second.posicion.second);
                    encontre = true;
                    break;
                }
            }
        }
    }
    camara.moverHaciaObjetivo();
}

InformacionInicial Cliente::ejecutar_menu(int argc, char* argv[]) {
    return menu.ejecutar(argc, argv, es_host);
}

void Cliente::loop_principal(InformacionInicial& info_inicial) {
    // Configurar SDL.
    configurarSDL(info_inicial);
    // Configurar camara.
    configurarCamara(info_inicial);
    // Configurar dibujador.
    configurarDibujador(info_inicial);
    // Inicializar hilos.
    iniciarHilos(info_inicial);

    int tick_anterior = SDL_GetTicks();
    int rate = 1000 / FPS;
    bool continuar = true;

    bool mover_camara = true;

    Comando comando;
    
    // Obtengo colores de los jugadores.
    std::vector<colorJugador> colores = config.getColoresJugadores();

    while (continuar) {
        // Actualizo el estado del juego.
        recepcion_estados.try_pop(estado_juego);

        // Actualizo entidades en el iterador.
        control_iteracion.actualizarEntidades();     

        // Chequeo comandos de teclado.
        while (comandos_teclado.try_pop(comando)) {
            ejecutarComandoTeclado(comando, continuar, mover_camara);
        }

        // Actualizo la camara.
        actualizarObjetivoCamara();

        // Renderizo.
        dibujador.dibujar(control_iteracion,
            info_inicial.vigas,
            pos_cursor, colores,
            volumen, muteado,
            es_host);

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
