#include "entidadProyectil.h"

EntidadProyectil::EntidadProyectil(GestorAnimaciones& gestor_animaciones,
    Renderer& renderizador,
    std::shared_ptr<EstadoDelJuego>& estado_juego,
    idProyectil id_proyectil,
    GestorSonidos& gestor_sonidos,
    Camara& camara,
    Font& fuente1,
    Font& fuente2) :
    EntidadActualizable(estado_juego, renderizador, gestor_animaciones, camara, fuente1, fuente2),
    id_proyectil(id_proyectil),
    gestor_sonidos(gestor_sonidos) {}

void EntidadProyectil::reproducirSonidoProyectil(std::shared_ptr<Animacion>& animacion,
    ArmaProtocolo& proyectil, bool exploto) {
    // Si el iterador está en 0 es porque la animacion cambio, reproduzco sonidos donde
    // corresponda. Para las animaciones con loop, se divide el iterador por la cantidad
    // de frames de la animacion y se reproduce el sonido cuando el resto es 0.
    if (it == 0) {
        if (proyectil == BAZOOKA_P || proyectil == MORTERO_P) {
            if (exploto)
                gestor_sonidos.getSonido(SONIDO_EXPLOSION)->reproducir();
            else
                gestor_sonidos.getSonido(SONIDO_DISPARO)->reproducir();
        }
        else if (proyectil == GRANADA_VERDE_P || proyectil == GRANADA_ROJA_P || proyectil == GRANADA_SANTA_P || proyectil == BANANA_P || proyectil == DINAMITA_P)
            if (exploto) {
                if (proyectil == GRANADA_SANTA_P) {
                    gestor_sonidos.getSonido(SONIDO_GRANADA_SANTA)->reproducir();
                    gestor_sonidos.getSonido(SONIDO_EXPLOSION_GRANDE)->reproducir();
                }
                else if (proyectil == DINAMITA_P)
                    gestor_sonidos.getSonido(SONIDO_EXPLOSION_GRANDE)->reproducir();
                else
                    gestor_sonidos.getSonido(SONIDO_EXPLOSION)->reproducir();
            } else {
                gestor_sonidos.getSonido(SONIDO_LANZAMIENTO)->reproducir();
            }
        else if (proyectil == ATAQUE_AEREO_P)
            gestor_sonidos.getSonido(SONIDO_ATAQUE_AEREO)->reproducir();
    }
    if (!exploto && proyectil == DINAMITA_P && it % animacion->getFrames() == 0) {
        gestor_sonidos.getSonido(SONIDO_DINAMITA)->reproducir();
    }
}


void EntidadProyectil::dibujarExplosion(ArmaProtocolo& proyectil, bool& es_fragmento, int& pos_x, int& pos_y) {
    // Si la animacion cambio, reseteo el iterador.
    std::shared_ptr<Animacion> animacion = gestor_animaciones.getAnimacionEscenario(EXPLOSION);
    actualizarAnimacion(animacion);
    if (es_fragmento) {
        animacion->setDimensiones(70, 70);
    } else if (proyectil == DINAMITA_P) {
        animacion->setDimensiones(120, 120);
    } else if (proyectil == GRANADA_SANTA_P) {
        animacion->setDimensiones(200, 200);
    } else {
        animacion->setDimensiones(100, 100);
    }
    animacion->dibujar(camara, pos_x, pos_y, false, it, 1);
    reproducirSonidoProyectil(animacion, proyectil, true);
}

void EntidadProyectil::dibujarProyectil(ArmaProtocolo& proyectil, bool& es_fragmento, int& pos_x, int& pos_y, radianes& angulo) {
    // Si la animacion cambio, reseteo el iterador.
    std::shared_ptr<Animacion> animacion = gestor_animaciones.getAnimacionProyectil(proyectil, es_fragmento);
    actualizarAnimacion(animacion);
    if (proyectil == DINAMITA_P)
        animacion->dibujar(camara, pos_x, pos_y, false, it, 1, angulo);
    else
        animacion->dibujar(camara, pos_x, pos_y, false, angulo);
    reproducirSonidoProyectil(animacion, proyectil, false);
}

void EntidadProyectil::dibujar() {
    RepresentacionProyectil proyectil;
    // Si el proyectil no existe, no se dibuja y se pone como muerto.
    bool existe = false;
    for (auto& proyectil_estado : estado_juego->proyectiles) {
        if (proyectil_estado.id == id_proyectil) {
            proyectil = proyectil_estado;
            existe = true;
            break;
        }
    }
    if (!existe) {
        muerta = true;
        return;
    }
    // Traduzco las coordenadas del proyectil.
    std::pair<int, int> posicion = camara.traducirCoordenadas(proyectil.posicion.first, proyectil.posicion.second);
    // Dibujo el proyectil.
    if (proyectil.exploto) {
        dibujarExplosion(proyectil.proyectil, proyectil.esFragmento, posicion.first, posicion.second);
    } else {
        dibujarProyectil(proyectil.proyectil, proyectil.esFragmento, posicion.first, posicion.second, proyectil.angulo);
        // Dibujo la cuenta regresiva del proyectil si la tiene.
        if (proyectil.cuentaRegresiva >= 0 && (proyectil.proyectil != BAZOOKA_P && proyectil.proyectil != MORTERO_P
            && proyectil.proyectil != ATAQUE_AEREO_P) && !proyectil.esFragmento) {
            // Dibujo la cuenta regresiva del proyectil
            // La cuenta regresiva es un int de segundos * 30, lo paso a float de segundos.
            float cuenta_regresiva = proyectil.cuentaRegresiva / 30.0;
            dibujarCuentaRegresiva(posicion, cuenta_regresiva);
        }
    } 
}
