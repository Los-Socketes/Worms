#ifndef ENTIDADACTUALIZABLE_H_
#define ENTIDADACTUALIZABLE_H_

#include <memory>
#include <utility>
#include <SDL2pp/SDL2pp.hh>

#include "entidad.h"
#include "../gestorAnimaciones.h"

// Clase padre de todas las entidades.
class EntidadActualizable : public Entidad {
 protected:
    std::shared_ptr<EstadoDelJuego>& estado_juego;
    bool muerta;
    Renderer& renderizador;
    Camara& camara;
    Font& fuente1;
    Font& fuente2;
    std::shared_ptr<Animacion> animacion_actual;
 public:
    EntidadActualizable(std::shared_ptr<EstadoDelJuego>& estado_juego, 
        Renderer& renderizador, GestorAnimaciones& gestor_animaciones,
        Camara& camara, Font& fuente1, Font& fuente2);
    virtual void dibujar() override = 0;
    void actualizarAnimacion(std::shared_ptr<Animacion> animacion);
    void dibujarCuentaRegresiva(std::pair<int,int>& posicion, float& cuenta_regresiva);
    bool estaMuerta();
    virtual ~EntidadActualizable() override = default;
};

#endif // ENTIDADACTUALIZABLE_H_