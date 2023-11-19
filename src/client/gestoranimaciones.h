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
    PANORAMA,
    RETICULA,
    VIGA_GRANDE,
    VIGA_CHICA,
    EXPLOSION
};

class GestorAnimaciones {
 private:
    Camara& camara;
    std::map<std::pair<EstadoGusano, ArmaProtocolo>, std::shared_ptr<Animacion>> gusanos;
    std::map<ItemEscenario, std::shared_ptr<Animacion>> escenario;
    std::map<ArmaProtocolo, std::shared_ptr<Animacion>> iconos;
    std::map<std::pair<ArmaProtocolo, bool>, std::shared_ptr<Animacion>> proyectiles;
    int ancho_mapa;
    int alto_mapa;
 public:
    GestorAnimaciones(Camara& camara, int ancho_mapa, int alto_mapa);
    void setDimensionMapa(int ancho, int alto);
    void inicializar(Renderer& renderizador);
    void dibujarAgua(int& pos_x, int& pos_y, int& it);
    void dibujarFondo();
    void dibujarPanorama(int& pos_x, int& pos_y);
    void dibujarViga(int& pos_x, int& pos_y, int& largo, radianes& angulo);
    void dibujarGusano(EstadoGusano& estado, RepresentacionArma& arma, DireccionGusano& dir, int& pos_x, int& pos_y, int& it);
    void dibujarReticula(int& pos_x, int& pos_y, int& it);
    void dibujarCursor(int& pos_x, int& pos_y, int& it);
    void dibujarIconoArma(ArmaProtocolo arma, int& pos_x, int& pos_y);
    void dibujarProyectil(ArmaProtocolo& proyectil, bool& es_fragmento, int& pos_x, int& pos_y, radianes& angulo, int& it);
    void dibujarExplosion(ArmaProtocolo& proyectil, bool& es_fragmento, int& pos_x, int& pos_y, int& it);
};

#endif // GESTORANIMACIONES_H_
