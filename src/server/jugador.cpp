#include "jugador.h"

Jugador::Jugador(std::vector<Gusano*> gusanos) {
    this->gusanoActualPos = 0;
    this->esMiTurno = false;
    this->gusanos = gusanos;
}

Gusano *Jugador::getGusanoActual() {
    Gusano *gusanoActual = nullptr;

    //Busco el primer gusano no muerto
    while (gusanoActual == nullptr) {
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

//Flatmap
std::map<id, RepresentacionGusano> Jugador::getRepresentacionGusanos() {
    std::map<id, RepresentacionGusano> misGusanos;

    for (Gusano *gusano : this->gusanos) {
        int idActual;
        idActual = gusano->getId();

        RepresentacionGusano repreActual;
        repreActual = gusano->getRepresentacion();

        misGusanos.insert({idActual, repreActual});
    }

    return misGusanos;
}
