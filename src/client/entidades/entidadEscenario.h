#ifndef ENTIDADESCENARIO_H_
#define ENTIDADESCENARIO_H_

#include <SDL2pp/SDL2pp.hh>

#include "entidad.h"
#include "../camara.h"
#include "../gestorAnimaciones.h"

// Clase que representa al escenario.
class EntidadEscenario : public Entidad {
 private:
    Renderer& renderizador;
    Camara& camara;
    int ancho_mapa;
    int alto_mapa;
    int pos_x;
    int pos_y;
    void dibujarBloque(int& posicion_x, int& posicion_y);
 public:
    EntidadEscenario(Renderer& renderizador,
        GestorAnimaciones& gestor_animaciones,
        Camara& camara,
        int& ancho_mapa,
        int& alto_mapa);

    void setDimensionesMapa(int ancho_mapa, int alto_mapa);

    void dibujar() override;

    ~EntidadEscenario() override = default;
};

#endif // ENTIDADESCENARIO_H_