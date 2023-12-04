#include "provision.h"

#include "box2dDefs.h"


class Gusano {
public:
    void anadirVida(int vidaExtra);
    void anadirMunicion(int cantiMunicion, ArmaProtocolo queArma);
};


Provision::Provision(tipoProvision miTipo, ArmaProtocolo tipomunicion, b2Body *cuerpo, int id, bool esTrampa) {
    this->tipo = miTipo;
    this->cuerpo = cuerpo;
    this->armaMunicion = tipomunicion;
    this->id = id;
    this->esTrampa = esTrampa;

    this->exploto = false;
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
    if (this->esTrampa)
        return;
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

    miRepresentacion.id = this->id;

    return miRepresentacion;
}
