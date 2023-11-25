#include "jugador.h"

#include <iostream>

Jugador::Jugador(std::vector<Gusano*> gusanos) {
    this->gusanoActualPos = 0;
    this->gusanos = gusanos;
}


Gusano *Jugador::getGusanoDeTurno() {
    return this->gusanos.at(this->gusanoActualPos);
}


Gusano *Jugador::getGusanoActual() {
    Gusano *gusanoActual = nullptr;

    // int posicionInicial = this->gusanoActualPos;
    //Busco el primer gusano no muerto
    for (int i = 0; i <= (int)this->gusanos.size(); i++) {
        this->gusanoActualPos += 1;
        // if (this->gusanoActualPos == posicionInicial) {
        //     return nullptr;
        // }

        //WARNING Casteo falopa. En teoria nada deberia explotar
        if (this->gusanoActualPos >= (int) this->gusanos.size()) {
	        this->gusanoActualPos = 0;
        }

        if (!((this->gusanos.at(gusanoActualPos))->getEstado() == MUERTO) &&
            !((this->gusanos.at(gusanoActualPos))->getEstado() == AHOGADO)) {

                gusanoActual = this->gusanos.at(gusanoActualPos);
                break;
        }

    }

    //Una vez encontrado dicho gusano, la proxima iteracion ya arranca
    //con el proximo
    std::cout << "gusano actual: " << gusanoActualPos << "\n";
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
