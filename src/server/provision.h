#ifndef PROVISION_H
#define PROVISION_H

#include "partida.h"
#include "defs.h"
#include "box2dDefs.h"

#define CANTIDADCURACION 25
#define CANTIDADMUNICION 2

class Provision {
 private:
    void darVida(Gusano *gusano);
    
    void darMunicion(Gusano *gusano);

    tipoProvision tipo;
    ArmaProtocolo armaMunicion;
    b2Body *cuerpo;
    
 public:
    bool estaEnElAire;
    bool fueAgarrada;

    Provision(tipoProvision miTipo, ArmaProtocolo tipomunicion, b2Body *cuerpo);

    void provisionar(Gusano *gusano);

    RepresentacionProvisiones getRepresentacin();
};

#endif
