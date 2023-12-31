#include "cliente.h"

Cliente::Cliente(Socket&& skt, ConfiguracionCliente& configuracion):
    config(configuracion),
    sdl(SDL_INIT_VIDEO),
    ttf(),
    ventana("Worms", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, config.getDimensionesIniciales().first,
        config.getDimensionesIniciales().second, SDL_WINDOW_HIDDEN),
    renderizador(ventana, -1, SDL_RENDERER_ACCELERATED),
#if DISTRO == 1 || DISTRO == 3
    mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,
        4, 4096),
#elif DISTRO == 2
    /*Fabri: Por algun en Gentoo (al menos en mi maquina) rompe el 4.
     *Como el codigo lo van a testear en Ubuntu, lo dejo como esta y
     *listo, este ifdef es mas que nada para que Fabri (yo) pueda
     *testear el programa :_(
     */
    mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,
       MIX_DEFAULT_CHANNELS , 4096),
#endif
    
    music("assets/sounds/music.ogg"),
    protocolo(std::move(skt)),
    estado_juego(std::make_shared<EstadoDelJuego>()),
    es_host(false),
    timeout(TIMEOUT * FPS),
    camara(0, 0, config.getDimensionesIniciales().first,
        config.getDimensionesIniciales().second, 0, 0),
    pos_cursor(0, 0),
    volumen(config.getVolumenInicial()),
    muteado(false),
    fuente1("assets/fonts/AdLibRegular.ttf", 32),
    fuente2("assets/fonts/ANDYB.TTF", 32),
    control_entidades(renderizador, mixer, camara, estado_juego,
        config.getDimensionesIniciales().first,
        config.getDimensionesIniciales().second,
        config.getColoresJugadores(),
        es_host, pos_cursor, volumen, muteado,
        fuente1, fuente2, timeout,
        config.getTiempoTurno(),
        config.getVientoMax(),
        config.getVientoMin()),
    menu(protocolo),
    recepcion_estados(TAM_QUEUE),
    envio_comandos(TAM_QUEUE),
    comandos_teclado(TAM_QUEUE),
    es_turno(false),
    municiones_agotadas(true),
    ejecuto_accion(false),
    inicio(false),
    entrada_teclado(envio_comandos,
        comandos_teclado, camara,
        municiones_agotadas,
        ejecuto_accion),
    recibidor(protocolo, recepcion_estados,
        es_turno, municiones_agotadas,
        ejecuto_accion),
    enviador(protocolo, envio_comandos, es_turno) {}

void Cliente::configurarSDL(InformacionInicial& info_inicial) {
    ventana.SetResizable(true);
    ventana.Show();
    ventana.SetTitle("Worms - Jugador " + std::to_string(info_inicial.jugador));
    mixer.AllocateChannels(5);
    mixer.GroupChannel(1, 0);
    mixer.GroupChannel(2, 0);
    mixer.GroupChannel(3, 0);
    mixer.GroupChannel(4, 0);
    mixer.FadeInMusic(music, -1, 2000);
    mixer.SetMusicVolume(volumen);
    mixer.SetVolume(-1, volumen);
}

void Cliente::configurarCamara(InformacionInicial& info_inicial) {
    camara.setDimensionMapa(info_inicial.dimensiones.first, info_inicial.dimensiones.second);
}

void Cliente::configurarControlEntidades(InformacionInicial& info_inicial) {
    control_entidades.setDimensionMapa(info_inicial.dimensiones.first, info_inicial.dimensiones.second);
    control_entidades.setIdJugador(info_inicial.jugador - 1);
    control_entidades.setIdPartida(info_inicial.idPartida);
    control_entidades.setVigas(info_inicial.vigas);
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
            control_entidades.getGestorSonidos().getSonido(SONIDO_SIN_MUNICIONES)->reproducir();
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
                if (gusano.second.estado != QUIETO && gusano.second.estado != MUERTO && gusano.second.estado != AHOGADO) {
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
    // Configurar el controlador de entidades.
    configurarControlEntidades(info_inicial);
    // Inicializar hilos.
    iniciarHilos(info_inicial);

    int tick_anterior = SDL_GetTicks();
    int rate = 1000 / FPS;
    bool continuar = true;

    bool mover_camara = true;

    Comando comando;
    
    while (continuar) {
        // Actualizo el estado del juego, si no se recibio nada, empiezo a contar el timeout.
        if (!recepcion_estados.try_pop(estado_juego)) {
            timeout--;
            if (timeout == 0) {
                continuar = false;
                std::cout << "Se perdio la conexion con el servidor." << std::endl;
                break;
            }
        } else {
            timeout = TIMEOUT * FPS;
        }

        // Actualizo entidades en el controlador.
        control_entidades.actualizarEntidades();     

        // Chequeo comandos de teclado.
        while (comandos_teclado.try_pop(comando)) {
            ejecutarComandoTeclado(comando, continuar, mover_camara);
        }

        // Actualizo la camara.
        actualizarObjetivoCamara();

        // Renderizo.
        control_entidades.dibujarEntidades();

        // Constant rate loop.
        int tick_actual = SDL_GetTicks();
        int descanso = rate - (tick_actual - tick_anterior);

        if (descanso < 0) {
            int ticks_detras = -descanso;
            int ticks_perdidos = ticks_detras - ticks_detras % rate;
            tick_anterior += ticks_perdidos;
            control_entidades.aumentarIteraciones(ticks_detras / rate);
        }
        else {
            SDL_Delay(descanso);
        }
    
        tick_anterior += rate;
        control_entidades.aumentarIteraciones(1);

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
