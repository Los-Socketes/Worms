#ifndef GESTORMULTIMEDIA_H_
#define GESTORMULTIMEDIA_H_

#include <map>
#include <utility>
#include <string>
#include <memory>
#include <SDL2pp/SDL2pp.hh>

#include "animacion.h"
#include "defs.h"
#include "camara.h"
#include "controliteracion.h"
#include "sonido.h"

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

enum TipoSonido {
    SONIDO_GUSANO_CAMINA_INICIO,
    SONIDO_GUSANO_CAMINA_FIN,
    SONIDO_GUSANO_SALTA,
    SONIDO_GUSANO_PIRUETA,
    SONIDO_CARGANDO_ARMA,
    SONIDO_CARGANDO_LANZAMIENTO,
    SONIDO_DISPARO,
    SONIDO_LANZAMIENTO,
    SONIDO_EXPLOSION,
    SONIDO_EXPLOSION_GRANDE,
    SONIDO_GRANADA_SANTA,
    SONIDO_ATAQUE_AEREO,
    SONIDO_COMUNICACION,
    SONIDO_TELETRANSPORTE,
    SONIDO_TICK,
    SONIDO_DINAMITA,
    SONIDO_BATE,
    SONIDO_AGUA,
    SONIDO_SIN_MUNICIONES
};

// Clase que se encarga de cargar y reproducir todos los recursos multimedia del juego.
// Dado un patrón de estado, tanto para gusanos como para proyectiles, dibuja la animación
// correspondiente y reproduce sonidos segun la iteracion actual.
class GestorMultimedia {
 private:
    Camara& camara;
    std::map<std::pair<EstadoGusano, ArmaProtocolo>, std::shared_ptr<Animacion>> gusanos;
    std::map<ItemEscenario, std::shared_ptr<Animacion>> escenario;
    std::map<ArmaProtocolo, std::shared_ptr<Animacion>> iconos;
    std::map<std::pair<ArmaProtocolo, bool>, std::shared_ptr<Animacion>> proyectiles;
    std::map<TipoSonido, std::shared_ptr<Sonido>> sonidos;
    int ancho_mapa;
    int alto_mapa;

    void reproducirSonidoGusano(IteradorGusano& iterador, EstadoGusano& estado, ArmaProtocolo& arma);
    void reproducirSonidoProyectil(IteradorProyectil& iterador, ArmaProtocolo& proyectil, bool exploto);
 public:
    GestorMultimedia(Camara& camara, int ancho_mapa, int alto_mapa);
    void setDimensionMapa(int ancho, int alto);
    void inicializar(Renderer& renderizador, Mixer& mixer);
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
    void reproducirSonido(TipoSonido tipo);
};

#endif // GESTORMULTIMEDIA_H_
