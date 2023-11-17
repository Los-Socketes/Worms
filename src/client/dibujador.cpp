#include "dibujador.h"

Dibujador::Dibujador(Camara& camara, std::shared_ptr<EstadoDelJuego>& estado_juego, int ancho_mapa, int alto_mapa) :
    camara(camara),
    estado_juego(estado_juego),
    ancho_mapa(ancho_mapa),
    alto_mapa(alto_mapa),
    gestor_animaciones(camara, ancho_mapa, alto_mapa),
    fuente("assets/fonts/AdLibRegular.ttf", 12) {}

std::pair<int, int> Dibujador::traducirCoordenadas(coordX x, coordY y) {
    // Paso de coordenadas en metros a coordenadas en pixeles.
    // TODO: ver como hacerlo bien. Pruebo con 20 pixeles por metro.
    int coord_x = x * PIXELS_POR_METRO;
    int coord_y = alto_mapa - y * PIXELS_POR_METRO;
    return std::make_pair(coord_x, coord_y);
}

void Dibujador::dibujarVida(Renderer& renderizador, std::pair<int, int> posicion, int vida) {
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


void Dibujador::inicializarAnimaciones(Renderer& renderizador) {
    gestor_animaciones.inicializar(renderizador);    
}


void Dibujador::dibujar(Renderer& renderizador, int it, std::vector<RepresentacionViga> vigas) {
    renderizador.Clear();

    RepresentacionGusano gusano_actual = estado_juego->gusanos[0][0];

    dibujarMapa(vigas);
    dibujarAguaDetras(it);
    dibujarGusanos(renderizador, it, 0);
    //dibujarProyectiles(it);
    dibujarAguaDelante(it);
    dibujarBarraArmas(renderizador, NADA_P);

    renderizador.Present();
}

void Dibujador::dibujarMapa(std::vector<RepresentacionViga> vigas) {
    // Dibujo la imagen de fondo.
    gestor_animaciones.dibujarFondo();
    // Dibujo el panorama del fondo.
    for (int i = 0; i < (ancho_mapa / 640 + 1); i++) {
        gestor_animaciones.dibujarPanorama(i * 640, alto_mapa - 179);
    }
    // Dibujo las vigas.
    for (auto& viga : vigas) {
        // Traduzco las coordenadas de la viga.
        std::pair<int, int> posicion = traducirCoordenadas(viga.posicionInicial.first, viga.posicionInicial.second);
        gestor_animaciones.dibujarViga(posicion.first, posicion.second, viga.longitud, viga.angulo);
    }
}


void Dibujador::dibujarGusanos(Renderer& renderizador, int it, radianes angulo) {
    // Recorro el mapa de jugador -> gusanos.
    for (auto& jugador : estado_juego->gusanos) {
        // Recorro los gusanos del jugador.
        for (auto& gusano : jugador.second) {
            // TODO: dibujar barra de potencia si esta disparando.
            // Dibujo al gusano.
            // Traduzco las coordenadas del gusano.
            std::pair<int, int> posicion = traducirCoordenadas(gusano.posicion.first, gusano.posicion.second);
            gestor_animaciones.dibujarGusano(gusano.estado, gusano.armaEquipada.arma, gusano.dir, posicion.first, posicion.second, it, angulo);
            // Dibujo la vida del gusano.
            dibujarVida(renderizador, posicion, gusano.vida);
            // Dibujo la reticula del gusano si esta apuntando.
            int direccion = gusano.dir == DERECHA ? 1 : -1;
            if (gusano.estado == QUIETO && gusano.armaEquipada.arma != NADA_P) {
                gestor_animaciones.dibujarReticula(
                    posicion.first + (sin(angulo) * 60) * direccion,
                    posicion.second + (cos(angulo) * 60),
                    it);
            }
                
        }
    }

}

// TODO: implementar.
// void Dibujador::dibujarProyectiles(int it) {}


void Dibujador::dibujarAguaDetras(int it) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < (ancho_mapa / 128 + 1); j++) {
            gestor_animaciones.dibujarAgua(j * 128 + 64, alto_mapa - 70 + 10 * (i + 1), it + 3*(i+1));
        }
    }
}

void Dibujador::dibujarAguaDelante(int it) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < (ancho_mapa / 128 + 1); j++) {
            gestor_animaciones.dibujarAgua(j * 128 + 64, alto_mapa - 50 + 10 * (i + 1), it + 3*(i+3));
        }
    }
}

void Dibujador::dibujarBarraArmas(Renderer& renderizador, ArmaProtocolo arma_equipada) {
    int ancho_pantalla = renderizador.GetOutputSize().x;
    int alto_pantalla = renderizador.GetOutputSize().y;
    // Dibujo barra de armas abajo a la derecha.
    for (int i = 0; i < 11; i++) {
        // Dibujo el icono del arma.
        gestor_animaciones.dibujarIconoArma(static_cast<ArmaProtocolo>(i), ancho_pantalla - 32 * (11 - i) - 14, alto_pantalla - 46);
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