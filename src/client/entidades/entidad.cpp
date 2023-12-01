#include "entidad.h"

Entidad::Entidad(GestorAnimaciones &gestor_animaciones) :
    gestor_animaciones(gestor_animaciones),
    it(0) {}

void Entidad::dibujar() {}

void Entidad::aumentarIteraciones(int cant) {	
    it = it + cant;
}
