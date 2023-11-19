#ifndef CONTROLITERACION_H
#define CONTROLITERACION_H

#include <map>
#include <utility>
#include <memory>
#include <SDL2pp/SDL2pp.hh>

#include "animacion.h"
#include "defs.h"

struct IteradorGusano{
    int it;
    std::shared_ptr<Animacion> animacion_actual;
};

struct IteradorProyectil{
    int it;
    std::shared_ptr<Animacion> animacion_actual;
};

// Clase que controla la iteracion de cada entidad para saber que frame
// de animacion se debe dibujar.
class ControlIteracion {
 private:
    std::shared_ptr<EstadoDelJuego>& estado_juego;
    int it_global;
    std::map<id, IteradorGusano> it_gusanos;
    std::map<idProyectil, IteradorProyectil> it_proyectiles;

 public:
    ControlIteracion(std::shared_ptr<EstadoDelJuego>& estado_juego);
    void actualizarEntidades();

    int& getIteracionGlobal();
    void aumentarIteraciones(int cant);

    int& getIteracionGusano(id id);
    void actualizarAnimacionGusano(id id, std::shared_ptr<Animacion> animacion);

    int& getIteracionProyectil(idProyectil id); 
    void actualizarAnimacionProyectil(idProyectil id, std::shared_ptr<Animacion> animacion);
};

#endif // CONTROLITERACION_H