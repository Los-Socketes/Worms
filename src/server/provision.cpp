#include "provision.h"


Provision::Provision(tipoProvision miTipo, ArmaProtocolo tipomunicion, b2Body *cuerpo) {
    this->tipo = miTipo;
    this->cuerpo = cuerpo;
    this->armaMunicion = tipomunicion;

    this->estaEnElAire = true;
    this->fueAgarrada = false;
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

RepresentacionProvisiones Provision::getRepresentacin() {
    RepresentacionProvisiones miRepresentacion;

    std::pair<coordX, coordY> miPosicion;
    miPosicion = deb2VecACoord(this->cuerpo->GetPosition());
    miRepresentacion.posicion = miPosicion;

    miRepresentacion.estaEnElAire = this->estaEnElAire;

    miRepresentacion.tipo = this->tipo; 

    miRepresentacion.armaMunicion = this->armaMunicion; 

    miRepresentacion.fueAgarrada = this->fueAgarrada; 

    return miRepresentacion;
}
