#include "provision.h"


Provision::Provision(tipoProvision miTipo) {
    this->tipo = miTipo;
}

void Provision::darVida(Gusano *gusano) {
    gusano->anadirVida(CANTIDADCURACION);
}

void Provision::darMunicion(Gusano *gusano) {
    gusano->anadirMunicion(CANTIDADMUNICION, this->armaMunicion);
}

void Provision::provisionar(Gusano *gusano) {
    switch (this->tipo) {
    case VIDA:
        this->darVida(gusano);
        break;
    case MUNICION:
        this->darMunicion(gusano);
        break;
    }
}
