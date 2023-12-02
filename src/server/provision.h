#ifndef PROVISION_H
#define PROVISION_H

#include "defs.h"
#include <box2d/box2d.h>

#define CANTIDADCURACION 25
#define CANTIDADMUNICION 2

class Gusano;

class Provision {
 private:
    void darVida(Gusano *gusano);
    
    void darMunicion(Gusano *gusano);

    tipoProvision tipo;
    ArmaProtocolo armaMunicion;

    int id;
    
 public:
    bool estaEnElAire;
    bool fueAgarrada;

    b2Body *cuerpo;

    Provision(tipoProvision miTipo, ArmaProtocolo tipomunicion, b2Body *cuerpo, int id);

    void provisionar(Gusano *gusano);

    RepresentacionProvisiones getRepresentacin();
};

#endif
