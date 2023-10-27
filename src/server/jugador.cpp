#include "jugador.h"
#include "protocolo.h"

Jugador::Jugador(Protocolo &&protocolo)
    : protocolo(std::move(protocolo)),

      enviador(this->protocolo),
      recibidor(this->protocolo, this->acciones)
{
    this->gusanoActual = 0;

    this->enviador.start();
    this->recibidor.start();
}


void Jugador::obtenerGusanosIniciales(std::vector<Gusano*> gusanos) {
    this->gusanos = gusanos;
}


bool Jugador::obtenerAccion(Direccion& primeraAccion) {
    bool pudeObtenerla;
    pudeObtenerla = this->acciones.try_pop(primeraAccion);

    return pudeObtenerla;
}
