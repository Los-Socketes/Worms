#ifndef GESTORANIMACIONES_H_
#define GESTORANIMACIONES_H_

#include <map>
#include <utility>
#include <string>
#include <memory>
#include <SDL2pp/SDL2pp.hh>

#include "animacion.h"
#include "defs.h"
#include "camara.h"

enum ItemEscenario{
    AGUA,
    FONDO,
    PANORAMA
};

class GestorAnimaciones {
 private:
    Camara& camara;
    std::map<std::pair<EstadoGusano, ArmaProtocolo>, std::shared_ptr<Animacion>> gusanos;
    std::map<ItemEscenario, std::shared_ptr<Animacion>> escenario;
    //std::map<ProyectilProtocolo, std::shared_ptr<Animacion>> proyectiles; ???
    int ancho_mapa;
    int alto_mapa;
 public:
    GestorAnimaciones(Camara& camara, int ancho_mapa, int alto_mapa);
    void inicializar(Renderer& renderizador);
    void dibujarAgua(int pos_x, int pos_y, int it);
    void dibujarFondo();
    void dibujarPanorama(int pos_x, int pos_y);
    void dibujarGusano(EstadoGusano estado, ArmaProtocolo arma, DireccionGusano dir, int pos_x, int pos_y, int it, radianes angulo);
    //void dibujarProyectil(ProyectilProtocolo protectil??, radianes angulo??, int pos_x, int pos_y, int it);
    //void dibujarExplosion(int pos_x, int pos_y, int it);
};

#endif // GESTORANIMACIONES_H_
