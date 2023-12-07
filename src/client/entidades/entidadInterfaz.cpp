#include "entidadInterfaz.h"

EntidadInterfaz::EntidadInterfaz(Renderer& renderizador,
    GestorAnimaciones& gestor_animaciones,
    GestorSonidos& gestor_sonidos, Camara& camara,
    std::shared_ptr<EstadoDelJuego>& estado_juego,
    std::pair<int, int>& pos_cursor,
    std::vector<colorJugador>& colores,
    int tiempo_turno, bool& es_host, 
    int& volumen, bool& muteado,
    Font& fuente1, Font& fuente2,
    int& timeout, int viento_max, int viento_min) :
    Entidad(gestor_animaciones),
    renderizador(renderizador),
    gestor_animaciones(gestor_animaciones),
    gestor_sonidos(gestor_sonidos),
    camara(camara),
    estado_juego(estado_juego),
    pos_cursor(pos_cursor),
    colores(colores),
    tiempo_turno(tiempo_turno),
    es_host(es_host),
    timeout(timeout),    
    volumen(volumen),
    muteado(muteado),
    fuente1(fuente1),
    fuente2(fuente2),
    id_jugador(0),
    id_partida(0),
    viento_actual(0),
    viento_max(viento_max),
    viento_min(viento_min),
    gusano_actual(),
    esperando_movimiento(false),
    teclas_armas(),
    segundos_turno(0),
    final_partida(false) { 
    // Inicializo el gusano actual con valores por defecto.
    gusano_actual.idGusano = -1;
    gusano_actual.estado = QUIETO;
    gusano_actual.armaEquipada.arma = NADA_P;

    // Inicializo el mapa de teclas de armas.
    teclas_armas[NADA_P] = "R";
    teclas_armas[BAZOOKA_P] = "1";
    teclas_armas[MORTERO_P] = "2";
    teclas_armas[GRANADA_VERDE_P] = "3";
    teclas_armas[GRANADA_ROJA_P] = "4";
    teclas_armas[GRANADA_SANTA_P] = "5";
    teclas_armas[BANANA_P] = "6";
    teclas_armas[DINAMITA_P] = "7";
    teclas_armas[BATE_P] = "8";
    teclas_armas[ATAQUE_AEREO_P] = "9";
    teclas_armas[TELETRANSPORTACION_P] = "0";
}



void EntidadInterfaz::actualizarGusanoActual() {
    // Chequeo que el jugador y el gusano de turno existan.
    if (estado_juego->gusanos.find(estado_juego->jugadorDeTurno) != estado_juego->gusanos.end() &&
        estado_juego->gusanos[estado_juego->jugadorDeTurno].find(estado_juego->gusanoDeTurno) != estado_juego->gusanos[estado_juego->jugadorDeTurno].end()) {
        gusano_actual = estado_juego->gusanos.at(estado_juego->jugadorDeTurno).at(estado_juego->gusanoDeTurno);
    }
    if (estado_juego->segundosRestantes == tiempo_turno) {
        esperando_movimiento = true;
    } else if (gusano_actual.estado != QUIETO) {
        esperando_movimiento = false;
    }
}

void EntidadInterfaz::dibujarFlechaGusanoActual() {
    // Si recien empezo el turno y el gusano todavía no se movio, dibujo la flecha.
    if (gusano_actual.idGusano != -1 && esperando_movimiento) {
        std::pair<int, int> posicion = camara.traducirCoordenadas(gusano_actual.posicion.first, gusano_actual.posicion.second);
        int pos_x = posicion.first;
        int pos_y = posicion.second - 60;
        gestor_animaciones.getAnimacionEscenario(FLECHA_GUSANO)->dibujar(camara, pos_x, pos_y, false, it, 1);
    }
}

void EntidadInterfaz::dibujarCursor() {
    // Dibujo el cursor si es mi turno, el gusano actual no disparo y
    // no esta usando un arma que requiera apuntar.
    if (estado_juego->jugadorDeTurno == id_jugador &&
        gusano_actual.estado != DISPARANDO && 
        (gusano_actual.armaEquipada.arma == ATAQUE_AEREO_P ||
        gusano_actual.armaEquipada.arma == TELETRANSPORTACION_P)) {
        int pos_x = pos_cursor.first;
        int pos_y = pos_cursor.second;
        gestor_animaciones.getAnimacionEscenario(RETICULA)->dibujar(camara, pos_x, pos_y, false, it, 1);
    }
}

void EntidadInterfaz::dibujarReticula() {
    // Dibujo la reticula del gusano actual si esta apuntando.
    if (gusano_actual.estado == QUIETO && gusano_actual.armaEquipada.tieneMira) {
        std::pair<int, int> posicion = camara.traducirCoordenadas(gusano_actual.posicion.first, gusano_actual.posicion.second);
        int direccion = gusano_actual.dir == DERECHA ? 1 : -1;
        int pos_x = posicion.first + (sin(gusano_actual.armaEquipada.anguloRad + M_PI / 2) * 60) * direccion;
        int pos_y = posicion.second + (cos(gusano_actual.armaEquipada.anguloRad + M_PI / 2) * 60);
        gestor_animaciones.getAnimacionEscenario(RETICULA)->dibujar(camara, pos_x, pos_y, false, it, 1);
    }
}

void EntidadInterfaz::dibujarBarraArmas(ArmaProtocolo& arma_equipada) {
    int ancho_pantalla = renderizador.GetOutputSize().x;
    int alto_pantalla = renderizador.GetOutputSize().y;
    std::pair<int, int> posicion;
    // Dibujo barra de armas abajo a la derecha.
    for (int i = 0; i < 11; i++) {
        posicion.first = ancho_pantalla - 32 * (11 - i) - 14;
        posicion.second = alto_pantalla - 46;
        // Dibujo el icono del arma.
        gestor_animaciones.getImagenIcono(static_cast<ArmaProtocolo>(i))->dibujar(camara, posicion.first, posicion.second, false, it, 1);
        // Dibujo la tecla asociada al arma.
        fuente2.SetOutline(0);
        Texture textura_tecla(renderizador, fuente2.RenderText_Blended(teclas_armas[static_cast<ArmaProtocolo>(i)], {255, 255, 255, 255}));
        renderizador.Copy(textura_tecla, NullOpt, Rect(posicion.first - 6, posicion.second - 32, 12, 16));
        // Dibujo el borde del icono.
        if (static_cast<ArmaProtocolo>(i) == arma_equipada) {
            renderizador.SetDrawColor(255, 255, 255, 255);
        } else {
            renderizador.SetDrawColor(0, 0, 0, 255);
        }
        renderizador.DrawRect(
            ancho_pantalla - 32 * (11 - i) - 30,
            alto_pantalla - 62,
            ancho_pantalla - 32 * (10 - i) - 30 - 1,
            alto_pantalla - 30 - 1);   
    }       
}

void EntidadInterfaz::dibujarMuniciones(RepresentacionArma& arma) {
    if (arma.arma != NADA_P) {
        int ancho_pantalla = renderizador.GetOutputSize().x;
        int alto_pantalla = renderizador.GetOutputSize().y;
        std::pair<int, int> posicion;
        // Dibujo la cantidad de municiones del arma equipada.
        posicion.first = ancho_pantalla - 32 * 11 - 14;
        posicion.second = alto_pantalla - 46;
        std::string texto = "Municiones: ";
        if (arma.municiones == -1) {
            texto += "Infinitas";
        } else {
            texto += std::to_string(arma.municiones);
        }
        fuente1.SetOutline(2);
        Texture textura_municiones_outline(renderizador, fuente1.RenderText_Blended(texto, {0, 0, 0, 255}));
        renderizador.Copy(textura_municiones_outline, NullOpt, Rect(posicion.first, posicion.second + 20, 120, 20));
        fuente1.SetOutline(0);
        Texture textura_municiones(renderizador, fuente1.RenderText_Blended(texto, {255, 255, 255, 255}));
        renderizador.Copy(textura_municiones, NullOpt, Rect(posicion.first, posicion.second + 20, 120, 20));
    }
}

void EntidadInterfaz::dibujarBarrasVida() {
    int ancho_pantalla = renderizador.GetOutputSize().x;
    int alto_pantalla = renderizador.GetOutputSize().y;
    std::pair<int, int> posicion;
    // Para cada jugador, dibujo una barra de vida de su color.
    // La misma se calcula en base a la vida de cada gusano sobre la vida total 
    // (gusanos x vida máxima inicial (100)).
    // Escribo "Jugador X" a la izquierda de la barra.
    for (auto& jugador : estado_juego->gusanos) {
        // Solo dibujo la barra si el jugador tiene gusanos.
        if (jugador.second.size() > 0) {
            int vida_total = 0;
            int vida_actual = 0;
            for (auto& gusano : jugador.second) {
                vida_total += 100;
                vida_actual += gusano.second.vida;
            }
            int vida_relativa = vida_actual * 100 / vida_total;
            posicion.first = 10;
            posicion.second = alto_pantalla - 30 * (jugador.first + 1) - 10;
            // Dibujo el texto.
            // Si es el turno le escribo '>' al principio.
            std::string texto = "";
            if (jugador.first == estado_juego->jugadorDeTurno) {
                texto += "> ";
            }
            fuente1.SetOutline(2);
            Texture textura_jugador_outline(renderizador, fuente1.RenderText_Blended(texto + "Jugador " + std::to_string(jugador.first + 1), {0, 0, 0, 255}));
            renderizador.Copy(textura_jugador_outline, NullOpt, Rect(posicion.first, posicion.second, 100, 20));
            fuente1.SetOutline(0);
            Texture textura_jugador(renderizador,
                fuente1.RenderText_Blended(texto + "Jugador " + std::to_string(jugador.first + 1),
                {colores.at(jugador.first)[0], colores.at(jugador.first)[1], colores.at(jugador.first)[2], 255}));
            renderizador.Copy(textura_jugador, NullOpt, Rect(posicion.first, posicion.second, 100, 20));
            // Dibujo la barra de vida.
            renderizador.SetDrawColor(colores.at(jugador.first)[0], colores.at(jugador.first)[1], colores.at(jugador.first)[2], 255);
            renderizador.FillRect(posicion.first + 120, posicion.second + 5, posicion.first + 120 + vida_relativa, posicion.second + 15);
            renderizador.SetDrawColor(0, 0, 0, 255);
            renderizador.DrawRect(posicion.first + 120, posicion.second + 5, posicion.first + 220, posicion.second + 15);
        }
    }    
}

void EntidadInterfaz::dibujarViento() {
    int ancho_pantalla = renderizador.GetOutputSize().x;
    int alto_pantalla = renderizador.GetOutputSize().y;
    // Primero dibujo un recuadro negro que contenga ambas imagenes.
    renderizador.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
    renderizador.SetDrawColor(0, 0, 0, 100);
    renderizador.FillRect((ancho_pantalla / 2) - 100, 15, (ancho_pantalla / 2) + 100, 33);
    // Las imágenes de viento tienen 96x13. Las dibujo arriba en el centro de la pantalla,
    // sobre el recuadro negro. Las tapo según la dirección e intensidad del viento.
    std::pair<int, int> posicion;
    int pixeles_a_dibujar = 0;
    // Para la barra izquierda, si el viento no sopla/sopla a la derecha la tapo completamente.
    if (viento_actual >= 0)
        pixeles_a_dibujar = 96;
    // Si no, la tapo según la intensidad.
    else
        pixeles_a_dibujar = (abs(viento_min) + viento_actual) * 96 / abs(viento_min);
    posicion.first = ancho_pantalla / 2 - 97;
    posicion.second = 18;
    gestor_animaciones.getAnimacionEscenario(VIENTO_IZQUIERDA)->dibujar(camara, posicion.first, posicion.second, false, it, 1);
    // Tapo las imágenes según el valor del viento.
    renderizador.SetDrawBlendMode(SDL_BLENDMODE_NONE);
    renderizador.SetDrawColor(0, 0, 0, 255);
    renderizador.FillRect(posicion.first,
        posicion.second,
        posicion.first + pixeles_a_dibujar,
        posicion.second + 12);
    posicion.first = ancho_pantalla / 2 + 2;
    // Calculo el viento para la barra derecha, si sopla a la izquierda o no sopla, se tapa toda.
    if (viento_actual <= 0)
        pixeles_a_dibujar = 96;
    // Si no, la tapo según la intensidad.
    else
        pixeles_a_dibujar = (abs(viento_max) - viento_actual) * 96 / abs(viento_max);
    gestor_animaciones.getAnimacionEscenario(VIENTO_DERECHA)->dibujar(camara, posicion.first, posicion.second, false, it, 1);
    renderizador.FillRect(posicion.first + 96,
        posicion.second + 13,
        posicion.first + 96 - pixeles_a_dibujar - 1,
        posicion.second - 1);
}

void EntidadInterfaz::actualizarViento() {
    if (viento_actual == estado_juego->viento)
        return;
    else if (viento_actual > estado_juego->viento)
        viento_actual--;
    else if (viento_actual < estado_juego->viento)
        viento_actual++;
}

void EntidadInterfaz::dibujarRonda() {
    int ancho_pantalla = renderizador.GetOutputSize().x;
    int alto_pantalla = renderizador.GetOutputSize().y;
    // Dibujo el numero de ronda en la esquina superior derecha.
    std::pair<int, int> posicion;
    posicion.first = ancho_pantalla - 70;
    posicion.second = 12;
    std::string texto = "Ronda " + std::to_string(estado_juego->ronda);
    fuente1.SetOutline(2);
    Texture textura_ronda_outline(renderizador, fuente1.RenderText_Blended(texto, {0, 0, 0, 255}));
    renderizador.Copy(textura_ronda_outline, NullOpt, Rect(posicion.first, posicion.second, 50, 20));
    fuente1.SetOutline(0);
    Texture textura_ronda(renderizador, fuente1.RenderText_Blended(texto, {255, 255, 255, 255}));
    renderizador.Copy(textura_ronda, NullOpt, Rect(posicion.first, posicion.second, 50, 20));
}

void EntidadInterfaz::dibujarCuentaRegresivaTurno() {
    if (estado_juego->segundosRestantes >= 0) {
        // Dibujo los segundos restantes arriba a la derecha de la pantalla.
        int ancho_pantalla = renderizador.GetOutputSize().x;
        int alto_pantalla = renderizador.GetOutputSize().y;
        std::pair<int, int> posicion;
        posicion.first = ancho_pantalla - 40;
        posicion.second = 35;
        // Determino el color blanco como default.
        SDL_Color color = {255, 255, 255, 255};
        // Si quedan 5 o menos segundos, dibujo en rojo.
        if (estado_juego->segundosRestantes <= 5) {
            color = {255, 0, 0, 255};
        }
        // Dibujo el borde del temporizador.
        fuente1.SetOutline(2);
        Texture textura_cuenta_outline(renderizador, fuente1.RenderText_Blended(std::to_string(estado_juego->segundosRestantes), {0, 0, 0, 255}));
        renderizador.Copy(textura_cuenta_outline, NullOpt, Rect(posicion.first, posicion.second, 20, 40));
        // Dibujo el temporizador.
        fuente1.SetOutline(0);
        Texture textura_cuenta(renderizador, fuente1.RenderText_Blended(std::to_string(estado_juego->segundosRestantes), color));
        renderizador.Copy(textura_cuenta, NullOpt, Rect(posicion.first, posicion.second, 20, 40));
        // Si quedan 5 o menos segundos, reproduzco el sonido de tick al principio de cada segundo.
        if (estado_juego->segundosRestantes <= 5 && segundos_turno != estado_juego->segundosRestantes) {
            gestor_sonidos.getSonido(SONIDO_TICK)->reproducir();
        }
        segundos_turno = estado_juego->segundosRestantes;
    }
}



void EntidadInterfaz::dibujarVolumen() {
    // Dibujo el volumen en la esquina superior izquierda.
    std::pair<int, int> posicion;
    posicion.first = 30;
    posicion.second = 30;
    ItemEscenario icono;
    // El volumen va de 0 a 128, lo divido en 4 partes.
    if (muteado) {
        icono = VOLUMEN_MUTEADO;
    } else if (volumen < 32) {
        icono = VOLUMEN_0;
    } else if (volumen < 64) {
        icono = VOLUMEN_1;
    } else if (volumen < 96) {
        icono = VOLUMEN_2;
    } else {
        icono = VOLUMEN_3;
    }
    gestor_animaciones.getAnimacionEscenario(icono)->dibujar(camara, posicion.first, posicion.second, false, it, 1);
}

void EntidadInterfaz::dibujarTextoTurno() {
    // En los primeros 2 segundos del turno, dibujo el texto "Turno de Jugador X".
    // Luego hago que desaparezca gradualmente.
    int ancho_pantalla = renderizador.GetOutputSize().x;
    int alto_pantalla = renderizador.GetOutputSize().y;
    std::pair<int, int> posicion;
    posicion.first = ancho_pantalla / 2 - 100;
    posicion.second = alto_pantalla / 2 - 50;
    // Dibujo el texto en blanco.
    SDL_Color color = {255, 255, 255, 255};
    if (estado_juego->segundosRestantes >= tiempo_turno - 2) {
        fuente1.SetOutline(2);
        Texture textura_turno_outline(renderizador, fuente1.RenderText_Blended("Turno de Jugador " + std::to_string(estado_juego->jugadorDeTurno + 1), {0, 0, 0, 255}));
        renderizador.Copy(textura_turno_outline, NullOpt, Rect(posicion.first, posicion.second, 200, 50));
        fuente1.SetOutline(0);
        Texture textura_turno(renderizador, fuente1.RenderText_Blended("Turno de Jugador " + std::to_string(estado_juego->jugadorDeTurno + 1), color));
        renderizador.Copy(textura_turno, NullOpt, Rect(posicion.first, posicion.second, 200, 50));
    }
}

void EntidadInterfaz::dibujarFondoEspera(int& ancho_pantalla, int& alto_pantalla) {
    int pos_x = ancho_pantalla / 2;
    int pos_y = alto_pantalla / 2;
    gestor_animaciones.getAnimacionEscenario(FONDOESPERA)->setDimensiones(ancho_pantalla, alto_pantalla);
    gestor_animaciones.getAnimacionEscenario(FONDOESPERA)->dibujar(camara, pos_x, pos_y, false, it, 1);
}

void EntidadInterfaz::dibujarGusanoEspera(int& ancho_pantalla, int& alto_pantalla) {
    // Cambio el tamaño y la posicion segun el tamaño de la pantalla.
    // La imagen tiene relacion de aspecto 4:3, hago que siempre ocupe el 50% del ancho de la pantalla.
    int tam_x = ancho_pantalla / 2;
    int tam_y = tam_x * 3 / 4;
    gestor_animaciones.getAnimacionEscenario(IMAGENGUSANOESPERA)->setDimensiones(tam_x, tam_y);
    // Lo dibujo abajo a la izquierda.
    int pos_x = tam_x / 2;
    int pos_y = alto_pantalla - tam_y / 2;
    gestor_animaciones.getAnimacionEscenario(IMAGENGUSANOESPERA)->dibujar(camara, pos_x, pos_y, false, it, 1);
}

void EntidadInterfaz::dibujarPantallaEspera(MomentoDePartida& momento) {
    int ancho_pantalla = renderizador.GetOutputSize().x;
    int alto_pantalla = renderizador.GetOutputSize().y;
    // Dibujo el fondo de espera.
    dibujarFondoEspera(ancho_pantalla, alto_pantalla);
    dibujarGusanoEspera(ancho_pantalla, alto_pantalla);
    // Dibujo el texto "Esperando a los demas jugadores..." en el color del jugador.
    std::pair<int, int> posicion;
    posicion.first = ancho_pantalla / 2 - 250;
    posicion.second = alto_pantalla / 2 - 50;
    SDL_Color color = {colores.at(id_jugador)[0], colores.at(id_jugador)[1], colores.at(id_jugador)[2], 255};
    fuente1.SetOutline(4);
    Texture textura_espera_outline(renderizador, fuente1.RenderText_Blended("Esperando a los demas jugadores...", {0, 0, 0, 255}));
    renderizador.Copy(textura_espera_outline, NullOpt, Rect(posicion.first, posicion.second, 500, 50));
    fuente1.SetOutline(0);
    Texture textura_espera(renderizador, fuente1.RenderText_Blended("Esperando a los demas jugadores...", color));
    renderizador.Copy(textura_espera, NullOpt, Rect(posicion.first, posicion.second, 500, 50));
    // Dibujo el id de la partida.
    posicion.first = ancho_pantalla - 120;
    posicion.second = alto_pantalla - 30;
    fuente2.SetOutline(2);
    Texture textura_id_outline(renderizador, fuente2.RenderText_Blended("ID Partida: " + std::to_string(id_partida), {0, 0, 0, 255}));
    renderizador.Copy(textura_id_outline, NullOpt, Rect(posicion.first, posicion.second, 100, 20));
    fuente2.SetOutline(0);
    Texture textura_id(renderizador, fuente2.RenderText_Blended("ID Partida: " + std::to_string(id_partida), {255, 255, 255, 255}));
    renderizador.Copy(textura_id, NullOpt, Rect(posicion.first, posicion.second, 100, 20));
    // Si ya hay suficientes jugadores y soy el host, dibujo el texto "Presiona C para comenzar la partida".
    if (momento == POR_INICIAR && es_host) {
        posicion.first = ancho_pantalla / 2 - 125;
        posicion.second = alto_pantalla / 2 + 30;
        fuente1.SetOutline(4);
        Texture textura_comenzar_outline(renderizador, fuente1.RenderText_Blended("Presiona C para comenzar la partida", {0, 0, 0, 255}));
        renderizador.Copy(textura_comenzar_outline, NullOpt, Rect(posicion.first, posicion.second, 250, 25));
        fuente1.SetOutline(0);
        Texture textura_comenzar(renderizador, fuente1.RenderText_Blended("Presiona C para comenzar la partida", {255, 255, 255, 255}));
        renderizador.Copy(textura_comenzar, NullOpt, Rect(posicion.first, posicion.second, 250, 25));        
    }
}

void EntidadInterfaz::dibujarFinalPartida() {
    // Dibujo el texto segun el resultado de la partida.
    int ancho_pantalla = renderizador.GetOutputSize().x;
    int alto_pantalla = renderizador.GetOutputSize().y;
    std::pair<int, int> posicion;
    posicion.first = ancho_pantalla / 2 - 100;
    posicion.second = alto_pantalla / 2 - 50;
    SDL_Color color = {colores.at(id_jugador)[0], colores.at(id_jugador)[1], colores.at(id_jugador)[2], 255};
    if (estado_juego->situacionJugadores.at(id_jugador) == GANASTE) {
        // Dibujo el texto "Ganaste!" en el color del jugador.
        fuente1.SetOutline(2);
        Texture textura_ganaste_outline(renderizador, fuente1.RenderText_Blended("Ganaste!", {0, 0, 0, 255}));
        renderizador.Copy(textura_ganaste_outline, NullOpt, Rect(posicion.first, posicion.second, 200, 50));
        fuente1.SetOutline(0);
        Texture textura_ganaste(renderizador, fuente1.RenderText_Blended("Ganaste!", color));
        renderizador.Copy(textura_ganaste, NullOpt, Rect(posicion.first, posicion.second, 200, 50));
        if (final_partida == false) {
            gestor_sonidos.getSonido(SONIDO_GANADOR)->reproducir();
            final_partida = true;
        }
    } else if (estado_juego->situacionJugadores.at(id_jugador) == PERDISTE) {
        // Dibujo el texto "Perdiste!" en el color del jugador.
        fuente1.SetOutline(2);
        Texture textura_perdiste_outline(renderizador, fuente1.RenderText_Blended("Perdiste!", {0, 0, 0, 255}));
        renderizador.Copy(textura_perdiste_outline, NullOpt, Rect(posicion.first, posicion.second, 200, 50));
        fuente1.SetOutline(0);
        Texture textura_perdiste(renderizador, fuente1.RenderText_Blended("Perdiste!", color));
        renderizador.Copy(textura_perdiste, NullOpt, Rect(posicion.first, posicion.second, 200, 50));
    }
}

void EntidadInterfaz::dibujarTimeout() {
    std::pair<int, int> posicion;
    posicion.first = 35;
    posicion.second = 80;    
    gestor_animaciones.getAnimacionEscenario(TIMEOUT)->dibujar(camara, posicion.first, posicion.second, false, it, 1);
}

void EntidadInterfaz::setIdJugador(int id) {
    id_jugador = id;
}

void EntidadInterfaz::setIdPartida(id id_partida) {
    this->id_partida = id_partida;
}

void EntidadInterfaz::dibujar(){
    actualizarGusanoActual();

    if (estado_juego->momento == ESPERANDO || estado_juego->momento == POR_INICIAR) {
        dibujarPantallaEspera(estado_juego->momento);
    }
    else {
        dibujarCursor();
        dibujarReticula();
        dibujarFlechaGusanoActual();
        dibujarBarraArmas(gusano_actual.armaEquipada.arma);
        dibujarMuniciones(gusano_actual.armaEquipada);
        dibujarBarrasVida();
        actualizarViento();
        dibujarViento();
        dibujarVolumen();
        if (estado_juego->momento != TERMINADA) {
            dibujarRonda();
            dibujarCuentaRegresivaTurno();
            dibujarTextoTurno();
        }
    }
    if (estado_juego->momento == TERMINADA) {
        dibujarFinalPartida();
    }
    // El timeout parpadea lentamente cuando quedan 5 segundos o menos.
    if (timeout <= 150 && timeout % 30 < 15) {
        dibujarTimeout();
    }    
}
