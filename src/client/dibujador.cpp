#include "dibujador.h"

Dibujador::Dibujador(Camara& camara, std::shared_ptr<EstadoDelJuego>& estado_juego, int ancho_mapa, int alto_mapa) :
    camara(camara),
    estado_juego(estado_juego),
    ancho_mapa(ancho_mapa),
    alto_mapa(alto_mapa),
    gestor_multimedia(camara, ancho_mapa, alto_mapa),
    fuente1("assets/fonts/AdLibRegular.ttf", 32),
    fuente2("assets/fonts/ANDYB.TTF", 32) {}

RepresentacionGusano Dibujador::getGusanoActual() {
    return estado_juego->gusanos[estado_juego->jugadorDeTurno][estado_juego->gusanoDeTurno];
}

std::pair<int, int> Dibujador::traducirCoordenadas(coordX& x, coordY& y) {
    // Paso de coordenadas en metros a coordenadas en pixeles.
    int coord_x = x * PIXELS_POR_METRO;
    int coord_y = alto_mapa - y * PIXELS_POR_METRO;
    return std::make_pair(coord_x, coord_y);
}

void Dibujador::dibujarReticula(std::pair<int, int>& posicion, radianes& angulo, int& direccion, ControlIteracion& iteraciones) {
    int pos_x = posicion.first + (sin(angulo + M_PI / 2) * 60) * direccion;
    int pos_y = posicion.second + (cos(angulo + M_PI / 2) * 60);
    gestor_multimedia.dibujarReticula(pos_x, pos_y, iteraciones.getIteracionGlobal());
}

void Dibujador::dibujarVida(Renderer& renderizador, std::pair<int, int>& posicion, hp& vida, colorJugador& color) {
    // Acomodo la posicion para que quede centrada en el gusano.
    int pos_x = posicion.first - 8;
    int pos_y = posicion.second - 30;

    std::optional<Rect> rect_interseccion = camara.getRectangulo().GetIntersection(Rect(pos_x, pos_y, 16, 16));
    
    int coord_x = pos_x - camara.getPosicionX();
    int coord_y = pos_y - camara.getPosicionY();
    
    // Si no hay interseccion no se renderiza.
    if (!rect_interseccion) {
        return;
    }

    // Dibujo un rectangulo negro transparente detrás de la vida.
    renderizador.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
    renderizador.SetDrawColor(0, 0, 0, 50);
    renderizador.FillRect(coord_x - 2, coord_y - 2, coord_x + 18, coord_y + 14);
    renderizador.SetDrawColor(255, 255, 255, 100);
    renderizador.DrawRect(coord_x - 2, coord_y - 2, coord_x + 18, coord_y + 14);

    // Dibujo el borde de la vida.
    fuente1.SetOutline(2);
    Texture textura_vida_outline(renderizador, fuente1.RenderText_Blended(std::to_string(vida), {0, 0, 0, 255}));
    renderizador.Copy(textura_vida_outline, NullOpt, Rect(coord_x, coord_y, 16, 16));

    // Dibujo la vida del color del jugador.
    fuente1.SetOutline(0);
    Texture textura_vida(renderizador, fuente1.RenderText_Blended(std::to_string(vida), {color[0], color[1], color[2], 255}));
    renderizador.Copy(textura_vida, NullOpt, Rect(coord_x, coord_y, 16, 16));
}

void Dibujador::dibujarCuadradoPotencia(Renderer& renderizador, std::pair<int,int>& posicion, radianes& angulo, int& direccion, float& i) {
    int tamanio = i * 20;
    int pos_x = posicion.first + (sin(angulo + M_PI / 2) * (10 + i * (60 - 10))) * direccion - tamanio / 2;
    int pos_y = posicion.second + (cos(angulo + M_PI / 2) * (10 + i * (60 - 10))) - tamanio / 2;

    std::optional<Rect> rect_interseccion = camara.getRectangulo().GetIntersection(Rect(pos_x, pos_y, tamanio, tamanio));
    
    int coord_x = pos_x - camara.getPosicionX();
    int coord_y = pos_y - camara.getPosicionY();
    
    // Si no hay interseccion no se renderiza.
    if (!rect_interseccion) {
        return;
    }

    // Dibujo el cuadrado, el color depende de la potencia, va de amarillo a rojo.
    renderizador.SetDrawColor(255, 255 * (1 - i), 0, 255);
    renderizador.FillRect(coord_x, coord_y, coord_x + tamanio, coord_y + tamanio);
}

void Dibujador::dibujarBarraPotencia(Renderer& renderizador, std::pair<int,int>& posicion, radianes& angulo, int& direccion, float& potencia) {
    // Potencia va de 0 a 1, y avanza en 0.05.
    // Dibujo cuadrado con SDL que van creciendo en tamaño, para el angulo y la direccion.
    for (float i = 0; i <= potencia; i += 0.1) {
        dibujarCuadradoPotencia(renderizador, posicion, angulo, direccion, i);
    }
}

void Dibujador::dibujarCuentaRegresiva(Renderer& renderizador, std::pair<int,int>& posicion, int& cuenta_regresiva) {
    // Dibujo el temporizador al lado del gusano.
    int pos_x = posicion.first + 20;
    int pos_y = posicion.second - 30;

    std::optional<Rect> rect_interseccion = camara.getRectangulo().GetIntersection(Rect(pos_x, pos_y, 16, 16));

    int coord_x = pos_x - camara.getPosicionX();
    int coord_y = pos_y - camara.getPosicionY();

    // Si no hay interseccion no se renderiza.
    if (!rect_interseccion) {
        return;
    }

    // Dibujo el borde del temporizador.
    fuente1.SetOutline(2);
    Texture textura_cuenta_outline(renderizador, fuente1.RenderText_Blended(std::to_string(cuenta_regresiva), {0, 0, 0, 255}));
    renderizador.Copy(textura_cuenta_outline, NullOpt, Rect(coord_x, coord_y, 12, 12));

    // Dibujo el temporizador.
    fuente1.SetOutline(0);
    Texture textura_cuenta(renderizador, fuente1.RenderText_Blended(std::to_string(cuenta_regresiva), {255, 255, 255, 255}));
    renderizador.Copy(textura_cuenta, NullOpt, Rect(coord_x, coord_y, 12, 12));
}

void Dibujador::setDimensionMapa(int ancho, int alto) {
    ancho_mapa = ancho * PIXELS_POR_METRO;
    alto_mapa = alto * PIXELS_POR_METRO;
    gestor_multimedia.setDimensionMapa(ancho_mapa, alto_mapa);
}

void Dibujador::inicializar(Renderer& renderizador, Mixer& mixer) {
    gestor_multimedia.inicializar(renderizador, mixer);
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


void Dibujador::dibujar(Renderer& renderizador,
    ControlIteracion& iteraciones,
    std::vector<RepresentacionViga>& vigas,
    std::pair<int, int>& pos_cursor,
    std::vector<colorJugador>& colores) {
    renderizador.Clear();

    RepresentacionGusano gusano_actual = getGusanoActual();

    dibujarMapa(vigas);
    dibujarAguaDetras(iteraciones);
    dibujarGusanos(renderizador, iteraciones, pos_cursor, colores);
    dibujarProyectiles(iteraciones);
    dibujarAguaDelante(iteraciones);
    dibujarBarraArmas(renderizador, gusano_actual.armaEquipada.arma);
    dibujarBarrasVida(renderizador, colores);

    renderizador.Present();
}

void Dibujador::dibujarMapa(std::vector<RepresentacionViga>& vigas) {
    // Dibujo la imagen de fondo.
    gestor_multimedia.dibujarFondo();
    // Dibujo el panorama del fondo.
    std::pair<int, int> posicion;
    for (int i = 0; i < (ancho_mapa / 640 + 1); i++) {
        posicion.first = i * 640;
        posicion.second = alto_mapa - 179;
        gestor_multimedia.dibujarPanorama(posicion.first, posicion.second);
    }
    // Dibujo las vigas.
    for (auto& viga : vigas) {
        // Traduzco las coordenadas de la viga.
        posicion = traducirCoordenadas(viga.posicionInicial.first, viga.posicionInicial.second);
        gestor_multimedia.dibujarViga(posicion.first, posicion.second, viga.longitud, viga.angulo);
    }
}


void Dibujador::dibujarGusanos(Renderer& renderizador,
    ControlIteracion& iteraciones,
    std::pair<int, int>& pos_cursor,
    std::vector<colorJugador>& colores) {
    RepresentacionGusano gusano;
    // Recorro el mapa de jugador -> gusanos.
    for (auto& jugador : estado_juego->gusanos) {
        // Recorro los gusanos del jugador.
        for (auto& gusano_id : jugador.second) {
            gusano = gusano_id.second;
            // Traduzco las coordenadas del gusano.
            std::pair<int, int> posicion = traducirCoordenadas(gusano.posicion.first, gusano.posicion.second);
            
            // Dibujo la barra de potencia del gusano si esta cargando.
            if (gusano.estado == QUIETO && gusano.armaEquipada.tienePotenciaVariable) {
                int direccion = gusano.dir == DERECHA ? 1 : -1;
                dibujarBarraPotencia(renderizador, posicion, gusano.armaEquipada.anguloRad, direccion, gusano.armaEquipada.potencia);
            }
            // Dibujo al gusano.
            gestor_multimedia.dibujarGusano(gusano.idGusano, gusano.estado, gusano.armaEquipada, gusano.dir, posicion.first, posicion.second, iteraciones);
            // Dibujo la vida del gusano.
            dibujarVida(renderizador, posicion, gusano.vida, colores.at(jugador.first));
            // Dibujo la reticula del gusano si esta apuntando.
            if (gusano.estado == QUIETO && gusano.armaEquipada.tieneMira) {
                int direccion = gusano.dir == DERECHA ? 1 : -1;
                dibujarReticula(posicion, gusano.armaEquipada.anguloRad, direccion, iteraciones);
            }
            // Dibujo la cuenta regresiva del arma si la tiene.
            if (gusano.estado == QUIETO && gusano.armaEquipada.tieneCuentaRegresiva) {
                dibujarCuentaRegresiva(renderizador, posicion, gusano.armaEquipada.cuentaRegresiva);
            }
            // Dibujo el cursor si el gusano no disparo y no esta usando un arma que requiera apuntar.
            if (gusano.estado != DISPARANDO && 
                (gusano.armaEquipada.arma == ATAQUE_AEREO_P ||
                gusano.armaEquipada.arma == TELETRANSPORTACION_P)) {
                int pos_x = pos_cursor.first;
                int pos_y = pos_cursor.second;
                gestor_multimedia.dibujarCursor(pos_x, pos_y, iteraciones.getIteracionGlobal());
            }
                                
        }
    }

}

void Dibujador::dibujarProyectiles(ControlIteracion& iteraciones) {
    for(auto& proyectil : estado_juego->proyectiles) {
        // Traduzco las coordenadas del proyectil.
        std::pair<int, int> posicion = traducirCoordenadas(proyectil.posicion.first, proyectil.posicion.second);
        // Dibujo el proyectil.
        if (proyectil.exploto) {
            gestor_multimedia.dibujarExplosion(proyectil.id, proyectil.proyectil, proyectil.esFragmento, posicion.first, posicion.second, iteraciones);
        } else {
            gestor_multimedia.dibujarProyectil(proyectil.id, proyectil.proyectil, proyectil.esFragmento, posicion.first, posicion.second, proyectil.angulo, iteraciones);
        }
    }    
}


void Dibujador::dibujarAguaDetras(ControlIteracion& iteraciones) {
    std::pair<int, int> posicion;
    int iteracion;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < (ancho_mapa / 128 + 1); j++) {
            posicion.first = j * 128 + 64;
            posicion.second = alto_mapa - 70 + 10 * (i + 1);
            iteracion = iteraciones.getIteracionGlobal() + 3*(i+1);
            gestor_multimedia.dibujarAgua(posicion.first, posicion.second, iteracion);
        }
    }
}

void Dibujador::dibujarAguaDelante(ControlIteracion& iteraciones) {
    std::pair<int, int> posicion;
    int iteracion;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < (ancho_mapa / 128 + 1); j++) {
            posicion.first = j * 128 + 64;
            posicion.second = alto_mapa - 50 + 10 * (i + 1);
            iteracion = iteraciones.getIteracionGlobal() + 3*(i+3);
            gestor_multimedia.dibujarAgua(posicion.first, posicion.second, iteracion);
        }
    }
}

void Dibujador::dibujarBarraArmas(Renderer& renderizador, ArmaProtocolo& arma_equipada) {
    int ancho_pantalla = renderizador.GetOutputSize().x;
    int alto_pantalla = renderizador.GetOutputSize().y;
    std::pair<int, int> posicion;
    // Dibujo barra de armas abajo a la derecha.
    for (int i = 0; i < 11; i++) {
        posicion.first = ancho_pantalla - 32 * (11 - i) - 14;
        posicion.second = alto_pantalla - 46;
        // Dibujo el icono del arma.
        gestor_multimedia.dibujarIconoArma(static_cast<ArmaProtocolo>(i), posicion.first, posicion.second);
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

void Dibujador::dibujarBarrasVida(Renderer& renderizador, std::vector<colorJugador>& colores) {
    int ancho_pantalla = renderizador.GetOutputSize().x;
    int alto_pantalla = renderizador.GetOutputSize().y;
    std::pair<int, int> posicion;
    // Para cada jugador, dibujo una barra de vida de su color.
    // La misma se calcula en base a la vida de cada gusano sobre la vida total 
    // (gusanos x vida máxima inicial (100)).
    // Escribo "Jugador X" a la izquierda de la barra.
    for (auto& jugador : estado_juego->gusanos) {
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

