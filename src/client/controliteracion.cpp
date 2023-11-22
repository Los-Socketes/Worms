#include "controliteracion.h"

ControlIteracion::ControlIteracion(std::shared_ptr<EstadoDelJuego>& estado_juego) :
    estado_juego(estado_juego),
    it_global(0) {}

void ControlIteracion::actualizarEntidades() {
    // Añado gusanos y proyectiles que no estaban en los mapas.
    for (auto& jugador : estado_juego->gusanos) {
        for(auto& gusano : jugador.second) {
            if (it_gusanos.find(gusano.idGusano) == it_gusanos.end()) {
                it_gusanos[gusano.idGusano] = {0, nullptr};
            }
        }
    }
    for (auto& proyectil : estado_juego->proyectiles) {
        if (it_proyectiles.find(proyectil.id) == it_proyectiles.end()) {
            it_proyectiles[proyectil.id] = {0, nullptr};
        }
    }
}

int& ControlIteracion::getIteracionGlobal() {
    return it_global;
}

void ControlIteracion::aumentarIteraciones(int cant) {
    // Aumento la iteracion global.
    it_global += cant;
    // Aumento la iteracion de cada gusano.
    for (auto& gusano : it_gusanos) {
        gusano.second.it += cant;
    }
    // Aumento la iteracion de cada proyectil.
    for (auto& proyectil : it_proyectiles) {
        proyectil.second.it += cant;
    }
}

IteradorGusano& ControlIteracion::getIteradorGusano(id id) {
    return it_gusanos[id];
}

IteradorProyectil& ControlIteracion::getIteradorProyectil(idProyectil id) {
    return it_proyectiles[id];
}

int& ControlIteracion::getIteracionGusano(id id) {
    return it_gusanos[id].it;
}

void ControlIteracion::actualizarAnimacionGusano(id id, std::shared_ptr<Animacion> animacion) {
    // Si la animacion actual es distinta a la nueva, la actualizo.
    if (it_gusanos[id].animacion_actual != animacion) {
        it_gusanos[id].animacion_actual = animacion;
        it_gusanos[id].it = 0;
    }
}

int& ControlIteracion::getIteracionProyectil(idProyectil id) {
    return it_proyectiles[id].it;
}

void ControlIteracion::actualizarAnimacionProyectil(idProyectil id, std::shared_ptr<Animacion> animacion) {
    // Si la animacion actual es distinta a la nueva, la actualizo.
    if (it_proyectiles[id].animacion_actual != animacion) {
        it_proyectiles[id].animacion_actual = animacion;
        it_proyectiles[id].it = 0;
    }
}

