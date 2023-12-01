#ifndef ENTIDAD_H_
#define ENTIDAD_H_

#include <memory>
#include <SDL2pp/SDL2pp.hh>

#include "../gestorAnimaciones.h"

// Clase padre de todas las entidades.
class Entidad {
 protected:
    GestorAnimaciones& gestor_animaciones;
    int it;
 public:
    Entidad(GestorAnimaciones& gestor_animaciones);
    virtual void dibujar() = 0;
    void aumentarIteraciones(int cant);
    virtual ~Entidad() = default;
};

#endif // ENTIDAD_H_