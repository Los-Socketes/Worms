#ifndef PROVISION_H
#define PROVISION_H

#include "partida.h"
#include "defs.h"

#define CANTIDADCURACION 25
#define CANTIDADMUNICION 2

class Provision {
 private:
    void darVida(Gusano *gusano);
    
    void darMunicion(Gusano *gusano);
    
 public:
    std::pair<coordX, coordY> posicion;
    bool estaEnElAire;
    tipoProvision tipo;
    ArmaProtocolo armaMunicion;
    bool fueAgarrada;

    Provision(tipoProvision miTipo);

    void provisionar(Gusano *gusano);
};

#endif
