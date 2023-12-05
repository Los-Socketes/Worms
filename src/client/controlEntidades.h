#ifndef CONTROLENTIDADES_H_
#define CONTROLENTIDADES_H_

#include <map>
#include <utility>
#include <memory>
#include <SDL2pp/SDL2pp.hh>

#include "defs.h"
#include "entidades/entidadEscenario.h"
#include "entidades/entidadAgua.h"
#include "entidades/entidadVigas.h"
#include "entidades/entidadGusano.h"
#include "entidades/entidadProyectil.h"
#include "entidades/entidadProvision.h"
#include "entidades/entidadInterfaz.h"
#include "gestorAnimaciones.h"
#include "gestorSonidos.h"

// Clase que controla las entidades del juego.
class ControlEntidades {
 private:
    // Referencias a recursos externos (para creacion de entidades).
    Renderer& renderizador;
    Mixer& mixer;
    Camara& camara;
    std::vector<colorJugador>& colores;
    Font& fuente1;
    Font& fuente2;
    // Estado del juego.
    std::shared_ptr<EstadoDelJuego>& estado_juego;
    // Gestores de recursos.
    GestorAnimaciones gestor_animaciones;
    GestorSonidos gestor_sonidos;
    // Entidades
    EntidadEscenario escenario;
    EntidadAgua agua_fondo;
    EntidadVigas vigas;
    std::map<id, std::shared_ptr<EntidadGusano>> gusanos;
    std::map<idProyectil, std::shared_ptr<EntidadProyectil>> proyectiles;
    std::map<idProvision, std::shared_ptr<EntidadProvision>> provisiones;
    EntidadAgua agua_delante;
    EntidadInterfaz interfaz;
 public:
    ControlEntidades(Renderer& renderizador,
        Mixer& mixer, Camara& camara,
        std::shared_ptr<EstadoDelJuego>& estado_juego,
        int ancho_mapa, int alto_mapa,
        std::vector<colorJugador>& colores,
        bool& es_host, std::pair<int, int>& pos_cursor,
        int& volumen, bool& muteado,
        Font& fuente1, Font& fuente2,
        int& timeout, int tiempo_turno,
        int viento_max, int viento_min);

    // Asigna las dimensiones del mapa.
    void setDimensionMapa(coordX& ancho_mapa, coordY& alto_mapa);

    // Asigna el id del jugador.
    void setIdJugador(int id);

    // Asigna el id de la partida.
    void setIdPartida(id id_partida);

    // Asigna las vigas del mapa.
    void setVigas(std::vector<RepresentacionViga>& vigas);

    // Actualiza las entidades del juego segun el estado del juego.
    void actualizarEntidades();

    // Aumenta las iteraciones de cada entidad.
    void aumentarIteraciones(int cant);

    // Indica a las entidades que deben dibujarse.
    void dibujarEntidades();

    GestorAnimaciones& getGestorAnimaciones();
    GestorSonidos& getGestorSonidos();
};

#endif // CONTROLENTIDADES_H_