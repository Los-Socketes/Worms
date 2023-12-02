#include "controlEntidades.h"

ControlEntidades::ControlEntidades(Renderer& renderizador,
    Mixer& mixer, Camara& camara,
    std::shared_ptr<EstadoDelJuego>& estado_juego,
    int ancho_mapa, int alto_mapa,
    std::vector<colorJugador>& colores,
    bool& es_host, std::pair<int, int>& pos_cursor,
    int& volumen, bool& muteado, Font& fuente1, Font& fuente2) :
    renderizador(renderizador),
    mixer(mixer),
    camara(camara),
    colores(colores),
    fuente1(fuente1),
    fuente2(fuente2),
    estado_juego(estado_juego),
    gestor_animaciones(renderizador, ancho_mapa, alto_mapa),
    gestor_sonidos(this->mixer),
    escenario(gestor_animaciones, camara, ancho_mapa, alto_mapa),
    agua_fondo(gestor_animaciones, camara, ancho_mapa, alto_mapa, 3, 80),
    vigas(gestor_animaciones, camara),
    gusanos(),
    proyectiles(),
    provisiones(),
    agua_delante(gestor_animaciones, camara, ancho_mapa, alto_mapa, 2, 50),
    interfaz(renderizador, gestor_animaciones, gestor_sonidos, camara,
        estado_juego, pos_cursor, colores, es_host, volumen, muteado,
        fuente1, fuente2) {}


void ControlEntidades::setDimensionMapa(coordX& ancho_mapa, coordY& alto_mapa) {
    int ancho = ancho_mapa * PIXELS_POR_METRO;
    int alto = alto_mapa * PIXELS_POR_METRO;
    escenario.setDimensionesMapa(ancho, alto);
    agua_fondo.setDimensionesMapa(ancho, alto);
    agua_delante.setDimensionesMapa(ancho, alto);
}

void ControlEntidades::setIdJugador(int id) {
    interfaz.setIdJugador(id);
}

void ControlEntidades::setVigas(std::vector<RepresentacionViga>& vigas) {
    this->vigas.setVigas(vigas);
}

void ControlEntidades::actualizarEntidades() {
    // Añado gusanos, proyectiles y provisiones que no estaban en los mapas.
    for (auto& jugador : estado_juego->gusanos) {
        for(auto& gusano : jugador.second) {
            // Si el gusano no estaba en el mapa, lo agrego.GestorAnimaciones& gestor_animaciones,
            if (this->gusanos.find(gusano.second.idGusano) == this->gusanos.end()) {
                this->gusanos[gusano.second.idGusano] = std::make_shared<EntidadGusano>(gestor_animaciones,
                    renderizador, estado_juego, gusano.second.idGusano, jugador.first,
                    gestor_sonidos, camara, colores, fuente1, fuente2);
            }
        }
    }
    for (auto& proyectil : estado_juego->proyectiles) {
        // Si el proyectil no estaba en el mapa, lo agrego.
        if (this->proyectiles.find(proyectil.id) == this->proyectiles.end()) {
            this->proyectiles[proyectil.id] = std::make_shared<EntidadProyectil>(gestor_animaciones,
                renderizador, estado_juego, proyectil.id, gestor_sonidos, camara, fuente1, fuente2);
        }
    }
    for (auto& provision : estado_juego->provisiones) {
        // Si la provision no estaba en el mapa, la agrego.
        if (this->provisiones.find(provision.id) == this->provisiones.end()) {
            this->provisiones[provision.id] = std::make_shared<EntidadProvision>(gestor_animaciones,
                renderizador, estado_juego, provision.id, gestor_sonidos, camara, fuente1, fuente2);
        }
    }

    // Elimino gusanos, proyectiles y provisiones que estan muertos.
    std::vector<id> gusanos_a_eliminar;
    for (auto& gusano : gusanos) {
        if (gusano.second->estaMuerta()) {
            gusanos_a_eliminar.push_back(gusano.first);
        }
    }
    for (auto& id : gusanos_a_eliminar) {
        gusanos.erase(id);
    }
    gusanos_a_eliminar.clear();
    std::vector<idProyectil> proyectiles_a_eliminar;
    for (auto& proyectil : proyectiles) {
        if (proyectil.second->estaMuerta()) {
            proyectiles_a_eliminar.push_back(proyectil.first);
        }
    } 
    for (auto& id : proyectiles_a_eliminar) {
        proyectiles.erase(id);
    }
    proyectiles_a_eliminar.clear();
    std::vector<idProvision> provisiones_a_eliminar;
    for (auto& provision : provisiones) {
        if (provision.second->estaMuerta()) {
            provisiones_a_eliminar.push_back(provision.first);
        }
    }
    for (auto& id : provisiones_a_eliminar) {
        provisiones.erase(id);
    }
    provisiones_a_eliminar.clear();
}


void ControlEntidades::aumentarIteraciones(int cant) {
    // Aumento las iteraciones de cada entidad.
    escenario.aumentarIteraciones(cant);
    agua_fondo.aumentarIteraciones(cant);
    vigas.aumentarIteraciones(cant);
    for (auto& jugador : estado_juego->gusanos) {
        for(auto& gusano : jugador.second) {
            gusanos.at(gusano.second.idGusano)->aumentarIteraciones(cant);
        }
    }
    for (auto& proyectil : estado_juego->proyectiles) {
        proyectiles.at(proyectil.id)->aumentarIteraciones(cant);
    }
    for (auto& provision : estado_juego->provisiones) {
        provisiones.at(provision.id)->aumentarIteraciones(cant);
    }
    agua_delante.aumentarIteraciones(cant);
    interfaz.aumentarIteraciones(cant);
}

void ControlEntidades::dibujarEntidades() {
    renderizador.Clear();
    // Dibujo las entidades.
    if (estado_juego->momento == EN_MARCHA || estado_juego->momento == TERMINADA) {
        escenario.dibujar();
        agua_fondo.dibujar();
        vigas.dibujar();
        for (auto& gusano : gusanos) {
            gusano.second->dibujar();
        }
        for (auto& proyectil : proyectiles) {
            proyectil.second->dibujar();
        }
        for (auto& provision : provisiones) {
            provision.second->dibujar();
        }
        agua_delante.dibujar();
    }
    interfaz.dibujar();
    renderizador.Present();
}

GestorAnimaciones& ControlEntidades::getGestorAnimaciones() {
    return gestor_animaciones;
}

GestorSonidos& ControlEntidades::getGestorSonidos() {
    return gestor_sonidos;
}