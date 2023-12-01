#ifndef ENTIDADESCENARIO_H_
#define ENTIDADESCENARIO_H_

#include <SDL2pp/SDL2pp.hh>

#include "entidad.h"
#include "../camara.h"
#include "../gestorAnimaciones.h"

// Clase que representa al escenario.
class EntidadEscenario : public Entidad {
 private:
    Camara& camara;
    int ancho_mapa;
    int alto_mapa;
    int pos_x;
    int pos_y;
 public:
    EntidadEscenario(GestorAnimaciones& gestor_animaciones,
        Camara& camara,
        int& ancho_mapa,
        int& alto_mapa);

    void setDimensionesMapa(int ancho_mapa, int alto_mapa);

    void dibujar() override;

    ~EntidadEscenario() override = default;
};

#endif // ENTIDADESCENARIO_H_