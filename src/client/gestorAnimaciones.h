#ifndef GESTORANIMACIONES_H_
#define GESTORANIMACIONES_H_

#include <map>
#include <utility>
#include <string>
#include <memory>
#include <SDL2pp/SDL2pp.hh>

#include "animacion.h"
#include "defs.h"

enum ItemEscenario {
    AGUA,
    FONDOESPERA,
    IMAGENGUSANOESPERA,
    FONDO,
    PANORAMA,
    RETICULA,
    VIGA_GRANDE,
    VIGA_CHICA,
    EXPLOSION,
    FLECHA_GUSANO,
    VOLUMEN_0,
    VOLUMEN_1,
    VOLUMEN_2,
    VOLUMEN_3,
    VOLUMEN_MUTEADO,
    TIMEOUT,
    GUSANO_GANADOR,
    PROVISION_MEDICINA,
    PROVISION_MEDICINA_CAYENDO,
    PROVISION_MUNICION,
    PROVISION_MUNICION_CAYENDO,
    VIENTO_IZQUIERDA,
    VIENTO_DERECHA
};

// Clase que se encarga de cargar las animaciones y devolverlas para ser dibujadas.
class GestorAnimaciones {
 private:
    std::map<std::pair<EstadoGusano, ArmaProtocolo>, std::shared_ptr<Animacion>> gusanos;
    std::map<ItemEscenario, std::shared_ptr<Animacion>> escenario;
    std::map<ArmaProtocolo, std::shared_ptr<Animacion>> iconos;
    std::map<std::pair<ArmaProtocolo, bool>, std::shared_ptr<Animacion>> proyectiles;
 public:
    GestorAnimaciones(Renderer& renderizador, int& alto_mapa, int& ancho_mapa);
    std::shared_ptr<Animacion> getAnimacionGusano(EstadoGusano& estado, ArmaProtocolo& arma);
    std::shared_ptr<Animacion> getAnimacionEscenario(ItemEscenario item);
    std::shared_ptr<Animacion> getAnimacionProyectil(ArmaProtocolo& proyectil, bool& es_fragmento);
    std::shared_ptr<Animacion> getImagenIcono(ArmaProtocolo arma);
};

#endif // GESTORANIMACIONES_H_
