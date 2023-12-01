#ifndef ENTIDADVIGAS_H_
#define ENTIDADVIGAS_H_

#include <SDL2pp/SDL2pp.hh>

#include "entidad.h"
#include "../camara.h"
#include "../gestorAnimaciones.h"

// Clase que representa a las vigas del escenario.
class EntidadVigas : public Entidad {
 private:
    Camara& camara;
    std::vector<RepresentacionViga> vigas;
 public:
    EntidadVigas(GestorAnimaciones& gestor_animaciones,
        Camara& camara);

    void setVigas(std::vector<RepresentacionViga> vigas);

    void dibujar() override;

    ~EntidadVigas() override = default;
};

#endif // ENTIDADVIGAS_H_