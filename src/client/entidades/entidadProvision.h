#ifndef ENTIDADPROVISION_H_
#define ENTIDADPROVISION_H_

#include <algorithm>
#include <SDL2pp/SDL2pp.hh>

#include "defs.h"
#include "entidadActualizable.h"
#include "../gestorAnimaciones.h"
#include "../gestorSonidos.h"

// Clase que representa a los proyectiles.
class EntidadProvision : public EntidadActualizable {
 private:
    idProvision id_provision;
    GestorSonidos& gestor_sonidos;
    bool agarrada;
    const std::string nombreArma(ArmaProtocolo& armaMunicion);
    void dibujarNombreArma(ArmaProtocolo& armaMunicion, int& pos_x, int& pos_y);
    void dibujarProvision(tipoProvision& tipo, bool& estaEnElAire, ArmaProtocolo& armaMunicion, int& pos_x, int& pos_y);
 public:
    EntidadProvision(GestorAnimaciones& gestor_animaciones,
        Renderer& renderizador,
        std::shared_ptr<EstadoDelJuego>& estado_juego,
        idProvision id_provision,
        GestorSonidos& gestor_sonidos,
        Camara& camara,
        Font& fuente1,
        Font& fuente2);

    void dibujar() override;

    ~EntidadProvision() override = default;
};

#endif // ENTIDADPROVISION_H_