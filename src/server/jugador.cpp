#include "jugador.h"

#include <iostream>

Jugador::Jugador() {
    this->gusanoActualPos = 0;
    this->miSituacion = JUGANDO;
}


Gusano *Jugador::getGusanoDeTurno() {
    return this->gusanos.at(this->gusanoActualPos);
}


Gusano *Jugador::getGusanoActual() {
    Gusano *gusanoActual = nullptr;

    //Busco el primer gusano no muerto
    for (int i = 0; i <= (int)this->gusanos.size(); i++) {
        this->gusanoActualPos += 1;
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
    return gusanoActual;
}

void Jugador::chequearSiPerdi() {
    int cantVivos;
    cantVivos = 0;
    for (Gusano *gusano : this->gusanos) {
        EstadoGusano estado;
        estado = gusano->getEstado();
        if (estado == MUERTO ||
	  estado == AHOGADO)
	  continue;

        cantVivos += 1;
    }

    bool perdi;
    perdi = (cantVivos == 0);

    if (perdi)
        this->miSituacion = PERDISTE;
}

SituacionJugador Jugador::getSituacion() {
    return this->miSituacion;
}

void Jugador::avisarQueGane() {
    this->miSituacion = GANASTE;
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


void Jugador::setGusanos(std::vector<Gusano*>gusanos) {
    this->gusanos = gusanos;
}
void Jugador::anadirGusano(Gusano *nuevoGusano) {
    this->gusanos.push_back(nuevoGusano);
}
void Jugador::darMasVidaAGusanos() {
    for (auto &&gusano : this->gusanos) {
        gusano->anadirVida(25);
    }
    
}

std::vector<Gusano *> Jugador::getGusanos() {
    return this->gusanos;
}

void Jugador::setID(idJugador id) {
    this->miID = id;
}

idJugador Jugador::getID() {
    return this->miID;
}
