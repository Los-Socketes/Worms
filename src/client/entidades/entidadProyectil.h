#ifndef ENTIDADPROYECTIL_H_
#define ENTIDADPROYECTIL_H_

#include <algorithm>
#include <SDL2pp/SDL2pp.hh>

#include "defs.h"
#include "entidadActualizable.h"
#include "../gestorAnimaciones.h"
#include "../gestorSonidos.h"

// Clase que representa a los proyectiles.
class EntidadProyectil : public EntidadActualizable {
 private:
    id id_proyectil;
    GestorSonidos& gestor_sonidos;
    void reproducirSonidoProyectil(std::shared_ptr<Animacion>& animacion,
        ArmaProtocolo& proyectil, bool exploto);
    void dibujarExplosion(ArmaProtocolo& proyectil, bool& es_fragmento,
        int& pos_x, int& pos_y);
    void dibujarProyectil(ArmaProtocolo& proyectil, bool& es_fragmento,
        int& pos_x, int& pos_y, radianes& angulo);
 public:
    EntidadProyectil(GestorAnimaciones& gestor_animaciones,
        Renderer& renderizador,
        std::shared_ptr<EstadoDelJuego>& estado_juego,
        idProyectil id_proyectil,
        GestorSonidos& gestor_sonidos,
        Camara& camara,
        Font& fuente1,
        Font& fuente2);

    void dibujar() override;

    ~EntidadProyectil() override = default;
};

#endif // ENTIDADPROYECTIL_H_