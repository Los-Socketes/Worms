#ifndef ENTIDADAGUA_H_
#define ENTIDADAGUA_H_

#include <SDL2pp/SDL2pp.hh>

#include "entidad.h"
#include "../camara.h"
#include "../gestorAnimaciones.h"

// Clase que representa a n capas de agua.
class EntidadAgua : public Entidad {
 private:
    Camara& camara;
    int ancho_mapa;
    int alto_mapa;
    int capas;
    int pos_y;
 public:
    EntidadAgua(GestorAnimaciones& gestor_animaciones,
        Camara& camara,
        int& ancho_mapa,
        int& alto_mapa,
        int capas,
        int pos_y);

    void setDimensionesMapa(int ancho_mapa, int alto_mapa);

    void dibujar() override;

    ~EntidadAgua() override = default;
};

#endif // ENTIDADAGUA_H_