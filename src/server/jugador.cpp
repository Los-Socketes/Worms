#include "jugador.h"
#include "protocolo.h"

Jugador::Jugador(Protocolo &&):
    protocolo(std::move(protocolo)){
    this->gusanoActual = 0;
}

void Jugador::obtenerGusano(std::vector<Gusano*> gusanos) {
    this->gusanos = gusanos;
}
