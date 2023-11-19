#include "dibujador.h"

Dibujador::Dibujador(Camara& camara, std::shared_ptr<EstadoDelJuego>& estado_juego, int ancho_mapa, int alto_mapa) :
    camara(camara),
    estado_juego(estado_juego),
    ancho_mapa(ancho_mapa),
    alto_mapa(alto_mapa),
    gestor_animaciones(camara, ancho_mapa, alto_mapa),
    fuente("assets/fonts/AdLibRegular.ttf", 12) {}

RepresentacionGusano Dibujador::getGusanoActual() {
    /* RepresentacionGusano gusano_actual;
    gusano_actual.idGusano = -1;
    gusano_actual.armaEquipada.arma = NADA_P;
    for (auto& jugador : estado_juego->gusanos) {
        for (auto& gusano : jugador.second) {
            if (gusano.idGusano == estado_juego->gusanoDeTurno) {
                gusano_actual = gusano;
            }
        }
    }
    return gusano_actual; */

    return estado_juego->gusanos[0][0];
}

std::pair<int, int> Dibujador::traducirCoordenadas(coordX& x, coordY& y) {
    // Paso de coordenadas en metros a coordenadas en pixeles.
    int coord_x = x * PIXELS_POR_METRO;
    int coord_y = alto_mapa - y * PIXELS_POR_METRO;
    return std::make_pair(coord_x, coord_y);
}

void Dibujador::dibujarReticula(std::pair<int, int>& posicion, radianes& angulo, int& direccion, int& it) {
    int pos_x = posicion.first + (sin(angulo + M_PI / 2) * 60) * direccion;
    int pos_y = posicion.second + (cos(angulo + M_PI / 2) * 60);
    gestor_animaciones.dibujarReticula(pos_x, pos_y, it);
}

void Dibujador::dibujarVida(Renderer& renderizador, std::pair<int, int>& posicion, hp& vida) {
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
    fuente.SetOutline(2);
    Texture textura_vida_outline(renderizador, fuente.RenderText_Blended(std::to_string(vida), {0, 0, 0, 255}));
    renderizador.Copy(textura_vida_outline, NullOpt, Rect(coord_x, coord_y, 16, 16));

    // Dibujo la vida.
    // TODO: colores segun jugador.
    fuente.SetOutline(0);
    Texture textura_vida(renderizador, fuente.RenderText_Blended(std::to_string(vida), {255, 255, 0, 255}));
    renderizador.Copy(textura_vida, NullOpt, Rect(coord_x, coord_y, 16, 16));
}

void Dibujador::setDimensionMapa(int ancho, int alto) {
    ancho_mapa = ancho;
    alto_mapa = alto;
    gestor_animaciones.setDimensionMapa(ancho, alto);
}

void Dibujador::inicializarAnimaciones(Renderer& renderizador) {
    gestor_animaciones.inicializar(renderizador);    
}


void Dibujador::dibujar(Renderer& renderizador, int& it, std::vector<RepresentacionViga>& vigas, std::vector<RepresentacionProyectil>& proyectiles) {
    renderizador.Clear();

    RepresentacionGusano gusano_actual = getGusanoActual();

    dibujarMapa(vigas);
    dibujarAguaDetras(it);
    dibujarGusanos(renderizador, it);
    dibujarProyectiles(proyectiles, it);
    dibujarAguaDelante(it);
    dibujarBarraArmas(renderizador, gusano_actual.armaEquipada.arma);

    renderizador.Present();
}

void Dibujador::dibujarMapa(std::vector<RepresentacionViga>& vigas) {
    // Dibujo la imagen de fondo.
    gestor_animaciones.dibujarFondo();
    // Dibujo el panorama del fondo.
    std::pair<int, int> posicion;
    for (int i = 0; i < (ancho_mapa / 640 + 1); i++) {
        posicion.first = i * 640;
        posicion.second = alto_mapa - 179;
        gestor_animaciones.dibujarPanorama(posicion.first, posicion.second);
    }
    // Dibujo las vigas.
    for (auto& viga : vigas) {
        // Traduzco las coordenadas de la viga.
        posicion = traducirCoordenadas(viga.posicionInicial.first, viga.posicionInicial.second);
        gestor_animaciones.dibujarViga(posicion.first, posicion.second, viga.longitud, viga.angulo);
    }
}


void Dibujador::dibujarGusanos(Renderer& renderizador, int& it) {
    // Recorro el mapa de jugador -> gusanos.
    for (auto& jugador : estado_juego->gusanos) {
        // Recorro los gusanos del jugador.
        for (auto& gusano : jugador.second) {
            // TODO: dibujar barra de potencia si esta disparando.
            // Dibujo al gusano.
            // Traduzco las coordenadas del gusano.
            std::pair<int, int> posicion = traducirCoordenadas(gusano.posicion.first, gusano.posicion.second);
            gestor_animaciones.dibujarGusano(gusano.estado, gusano.armaEquipada, gusano.dir, posicion.first, posicion.second, it);
            // Dibujo la vida del gusano.
            dibujarVida(renderizador, posicion, gusano.vida);
            // Dibujo la reticula del gusano si esta apuntando.
            if (gusano.estado == QUIETO && gusano.armaEquipada.tieneMira) {
                int direccion = gusano.dir == DERECHA ? 1 : -1;
                dibujarReticula(posicion, gusano.armaEquipada.anguloRad, direccion, it);
            }
                
        }
    }

}

void Dibujador::dibujarProyectiles(std::vector<RepresentacionProyectil>& proyectiles, int& it) {
    for(auto& proyectil : proyectiles) {
        // Traduzco las coordenadas del proyectil.
        std::pair<int, int> posicion = traducirCoordenadas(proyectil.posicion.first, proyectil.posicion.second);
        // Dibujo el proyectil.
        if (proyectil.exploto) {
            gestor_animaciones.dibujarExplosion(proyectil.proyectil, proyectil.esFragmento, posicion.first, posicion.second, it);
        } else {
            gestor_animaciones.dibujarProyectil(proyectil.proyectil, proyectil.esFragmento, posicion.first, posicion.second, proyectil.angulo, it);
        }
    }    
}


void Dibujador::dibujarAguaDetras(int& it) {
    std::pair<int, int> posicion;
    int iteracion;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < (ancho_mapa / 128 + 1); j++) {
            posicion.first = j * 128 + 64;
            posicion.second = alto_mapa - 70 + 10 * (i + 1);
            iteracion = it + 3*(i+1);
            gestor_animaciones.dibujarAgua(posicion.first, posicion.second, iteracion);
        }
    }
}

void Dibujador::dibujarAguaDelante(int& it) {
    std::pair<int, int> posicion;
    int iteracion;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < (ancho_mapa / 128 + 1); j++) {
            posicion.first = j * 128 + 64;
            posicion.second = alto_mapa - 50 + 10 * (i + 1);
            iteracion = it + 3*(i+3);
            gestor_animaciones.dibujarAgua(posicion.first, posicion.second, iteracion);
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
        gestor_animaciones.dibujarIconoArma(static_cast<ArmaProtocolo>(i), posicion.first, posicion.second);
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