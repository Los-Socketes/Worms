#include "entidadGusano.h"

EntidadGusano::EntidadGusano(GestorAnimaciones& gestor_animaciones,
        Renderer& renderizador,
        std::shared_ptr<EstadoDelJuego>& estado_juego,
        id id_gusano,
        int id_jugador,
        GestorSonidos& gestor_sonidos,
        Camara& camara,
        std::vector<colorJugador>& colores,
        Font& fuente1,
        Font& fuente2) : 
        EntidadActualizable(estado_juego, renderizador, gestor_animaciones, camara, fuente1, fuente2),
        id_gusano(id_gusano),
        id_jugador(id_jugador),
        gestor_sonidos(gestor_sonidos),
        colores(colores) {}

void EntidadGusano::dibujarVida(std::pair<int, int>& posicion, hp& vida, colorJugador& color) {
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

void EntidadGusano::dibujarCuadradoPotencia(std::pair<int,int>& posicion, radianes& angulo, int& direccion, float& i) {
    int tamanio = i * 0.2;
    int pos_x = posicion.first + (sin(angulo + M_PI / 2) * (10 + i/100 * (60 - 10))) * direccion - tamanio / 2;
    int pos_y = posicion.second + (cos(angulo + M_PI / 2) * (10 + i/100 * (60 - 10))) - tamanio / 2;

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

void EntidadGusano::dibujarBarraPotencia(std::pair<int,int>& posicion, radianes& angulo, int& direccion, float& potencia) {
    // Potencia va de 0 a 1, y avanza en 0.05.
    // Dibujo cuadrado con SDL que van creciendo en tamaño, para el angulo y la direccion.
    for (float i = 0; i <= potencia; i += 10) {
        dibujarCuadradoPotencia(posicion, angulo, direccion, i);
    }
}

void EntidadGusano::reproducirSonidoGusano(std::shared_ptr<Animacion>& animacion,
    EstadoGusano& estado, ArmaProtocolo& arma) {
    // Si el iterador está en 0 es porque la animacion cambio, reproduzco sonidos donde
    // corresponda. Para las animaciones de gusano caminando se reproducen dos sonidos, 
    // uno al inicio y otro a la mitad.
    switch (estado) {
        case CAMINANDO:
            if (it % animacion->getFrames() == 0) {
                gestor_sonidos.getSonido(SONIDO_GUSANO_CAMINA_INICIO)->reproducir();
            } else if (it % animacion->getFrames() == animacion->getFrames() / 2) {
                gestor_sonidos.getSonido(SONIDO_GUSANO_CAMINA_FIN)->reproducir();
            }
            break;
        case SALTANDO:
             if (it == 0) {
                gestor_sonidos.getSonido(SONIDO_GUSANO_SALTA)->reproducir();
             }
             break;
        case HACE_PIRUETA:
             if (it == 0) {
                gestor_sonidos.getSonido(SONIDO_GUSANO_PIRUETA)->reproducir();
             }
             break;
        case DISPARANDO:
            if (it == 0) {
                if (arma == BATE_P)
                    gestor_sonidos.getSonido(SONIDO_BATE)->reproducir();
                else if (arma == ATAQUE_AEREO_P)
                    gestor_sonidos.getSonido(SONIDO_COMUNICACION)->reproducir();
                else if (arma == TELETRANSPORTACION_P)
                    gestor_sonidos.getSonido(SONIDO_TELETRANSPORTE)->reproducir();
            }                
            break;
        case AHOGADO:
            if (it == 0) {
                gestor_sonidos.getSonido(SONIDO_AGUA)->reproducir();
            }
            break;
        case MUERTO:
            if (it == 59) {
                gestor_sonidos.getSonido(SONIDO_EXPLOSION)->reproducir();
            }
        default:
            break;
    }
}

void EntidadGusano::dibujarGusano(EstadoGusano& estado, RepresentacionArma& arma, DireccionGusano& dir, int& pos_x, int& pos_y) {
    // Si la animacion cambio, reseteo el iterador.
    std::shared_ptr<Animacion> animacion;
    if (estado_juego->momento != TERMINADA && estado != MUERTO && estado != AHOGADO) {
        animacion = gestor_animaciones.getAnimacionGusano(estado, arma.arma);
        actualizarAnimacion(animacion);
        if(arma.tieneMira && (estado == QUIETO || estado == DISPARANDO)) {
            animacion->dibujar(camara, pos_x, pos_y, dir == DERECHA, arma.anguloRad);
        } else {
            animacion->dibujar(camara, pos_x, pos_y, dir == DERECHA, it, 1);
        }
    }
    else {
        animacion = gestor_animaciones.getAnimacionEscenario(GUSANO_GANADOR);
        actualizarAnimacion(animacion);
        animacion->dibujar(camara, pos_x, pos_y, dir == DERECHA, it, 1);
    }
    reproducirSonidoGusano(animacion, estado, arma.arma);
}

void EntidadGusano::dibujar() {
    RepresentacionGusano gusano;
    // Si el gusano no existe no se dibuja y se pone como muerto.
    if (estado_juego->gusanos.find(id_jugador) != estado_juego->gusanos.end() &&
        estado_juego->gusanos.at(id_jugador).find(id_gusano) != estado_juego->gusanos.at(id_jugador).end()) {
        gusano = estado_juego->gusanos.at(id_jugador).at(id_gusano);
    } else {
        muerta = true;
        return;
    }
    
    // Traduzco las coordenadas del gusano.
    std::pair<int, int> posicion = camara.traducirCoordenadas(gusano.posicion.first, gusano.posicion.second);
    
    // Dibujo la barra de potencia del gusano si esta cargando.
    if (gusano.estado == QUIETO && gusano.armaEquipada.tienePotenciaVariable) {
        int direccion = gusano.dir == DERECHA ? 1 : -1;
        dibujarBarraPotencia(posicion, gusano.armaEquipada.anguloRad, direccion, gusano.armaEquipada.potencia);
    }
    // Dibujo al gusano.
    dibujarGusano(gusano.estado, gusano.armaEquipada, gusano.dir, posicion.first, posicion.second);
    // Dibujo la vida del gusano.
    if ((gusano.estado != MUERTO && gusano.estado != AHOGADO) || gusano.vida > 0) {
        dibujarVida(posicion, gusano.vida, colores.at(id_jugador));
    }
    // Dibujo la cuenta regresiva del arma si la tiene.
    if (gusano.estado == QUIETO && gusano.armaEquipada.tieneCuentaRegresiva) {
        float cuenta_regresiva = gusano.armaEquipada.cuentaRegresiva;
        dibujarCuentaRegresiva(posicion, cuenta_regresiva);
    }
}
