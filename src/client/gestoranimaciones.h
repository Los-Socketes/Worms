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
    VOLUMEN_MUTEADO
};

// Clase que se encarga de cargar las animaciones y devolverlas para ser dibujadas.
class GestorAnimaciones {
 private:
    std::map<std::pair<EstadoGusano, ArmaProtocolo>, std::shared_ptr<Animacion>> gusanos;
    std::map<ItemEscenario, std::shared_ptr<Animacion>> escenario;
    std::map<ArmaProtocolo, std::shared_ptr<Animacion>> iconos;
    std::map<std::pair<ArmaProtocolo, bool>, std::shared_ptr<Animacion>> proyectiles;
 public:
    GestorAnimaciones(Renderer& renderizador);
    std::shared_ptr<Animacion> getAnimacionGusano(EstadoGusano estado, ArmaProtocolo arma);
    std::shared_ptr<Animacion> getAnimacionEscenario(ItemEscenario item);
    std::shared_ptr<Animacion> getAnimacionProyectil(ArmaProtocolo proyectil, bool es_fragmento);
    std::shared_ptr<Animacion> getImagenIcono(ArmaProtocolo arma);
    /*
    void dibujarAgua(int& pos_x, int& pos_y, int& it);
    void dibujarFondo();
    void dibujarFondoEspera(int& ancho_pantalla, int& alto_pantalla);
    void dibujarGusanoEspera(int& ancho_pantalla, int& alto_pantalla);
    void dibujarPanorama(int& pos_x, int& pos_y);
    void dibujarViga(int& pos_x, int& pos_y, int& largo, radianes& angulo);
    void dibujarGusano(id& id_gusano, EstadoGusano& estado, RepresentacionArma& arma, DireccionGusano& dir, int& pos_x, int& pos_y, ControlIteracion& iteraciones);
    void dibujarReticula(int& pos_x, int& pos_y, int& it);
    void dibujarCursor(int& pos_x, int& pos_y, int& it);
    void dibujarIconoArma(ArmaProtocolo arma, int& pos_x, int& pos_y);
    void dibujarProyectil(idProyectil& id_proyectil, ArmaProtocolo& proyectil, bool& es_fragmento, int& pos_x, int& pos_y, radianes& angulo, ControlIteracion& iteraciones);
    void dibujarExplosion(idProyectil& id_proyectil, ArmaProtocolo& proyectil, bool& es_fragmento, int& pos_x, int& pos_y, ControlIteracion& iteraciones);
    void dibujarFlechaGusano(int& pos_x, int& pos_y, int& it);
    void dibujarVolumen(int& volumen, bool& muteado, int& pos_x, int& pos_y);
    */
};

#endif // GESTORANIMACIONES_H_
