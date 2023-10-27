#include "jugador.h"
#include "protocolo.h"

Jugador::Jugador(Protocolo &&protocolo)
    : protocolo(std::move(protocolo)),

      enviador(this->protocolo),
      recibidor(this->protocolo, this->acciones)
{
    this->gusanoActualPos = 0;

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

Gusano *Jugador::getGusanoActual() {
    Gusano *gusanoActual = nullptr;

    //Busco el primer gusano no muerto
    while (gusanoActual != nullptr) {
        gusanoActual = this->gusanos.at(gusanoActualPos);
        this->gusanoActualPos += 1;

        //WARNING Casteo falopa. En teoria nada deberia explotar
        if (this->gusanoActualPos > (int) this->gusanos.size())
	  this->gusanoActualPos = 0;
    }

    //Una vez encontrado dicho gusano, la proxima iteracion ya arranca
    //con el proximo

    return gusanoActual;
}
