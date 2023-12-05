#ifndef ENTIDADINTERFAZ_H_
#define ENTIDADINTERFAZ_H_

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <SDL2pp/SDL2pp.hh>

#include "defs.h"
#include "entidad.h"
#include "../configuracionCliente.h"
#include "../gestorAnimaciones.h"
#include "../gestorSonidos.h"
#include "../camara.h"

// Clase que representa a la interfaz.
class EntidadInterfaz : public Entidad {
 private:
    Renderer& renderizador;
    GestorAnimaciones& gestor_animaciones;
    GestorSonidos& gestor_sonidos;
    Camara& camara;
    std::shared_ptr<EstadoDelJuego>& estado_juego;
    std::pair<int, int>& pos_cursor;
    std::vector<colorJugador>& colores;
    int tiempo_turno;
    bool& es_host;
    int& timeout;
    int& volumen;
    bool& muteado;
    Font& fuente1;
    Font& fuente2;
    int id_jugador;
    id id_partida;
    int viento_actual;
    int viento_max;
    int viento_min;
    RepresentacionGusano gusano_actual;
    bool esperando_movimiento;
    std::map<ArmaProtocolo, std::string> teclas_armas;
    int segundos_turno;
    bool final_partida;

    void actualizarGusanoActual();
    void dibujarCursor();
    void dibujarReticula();
    void dibujarFlechaGusanoActual();
    void dibujarFondoEspera(int& ancho_pantalla, int& alto_pantalla);
    void dibujarGusanoEspera(int& ancho_pantalla, int& alto_pantalla);
    void dibujarPantallaEspera(MomentoDePartida& momento);
    void dibujarBarraArmas(ArmaProtocolo& arma_equipada);
    void dibujarMuniciones(RepresentacionArma& arma);
    void dibujarBarrasVida();
    void dibujarViento();
    void actualizarViento();
    void dibujarRonda();
    void dibujarCuentaRegresivaTurno();
    void dibujarVolumen();
    void dibujarTextoTurno();
    void dibujarFinalPartida();
    void dibujarTimeout();
 public:
    EntidadInterfaz(Renderer& renderizador,
        GestorAnimaciones& gestor_animaciones,
        GestorSonidos& gestor_sonidos, Camara& camara,
        std::shared_ptr<EstadoDelJuego>& estado_juego,
        std::pair<int, int>& pos_cursor,
        std::vector<colorJugador>& colores,
        int tiempo_turno, bool& es_host, 
        int& volumen, bool& muteado,
        Font& fuente1, Font& fuente2,
        int& timeout, int viento_max, int viento_min);


    // Asigna el id del jugador.
    void setIdJugador(int id);

    // Asigna el id de la partida.
    void setIdPartida(id id_partida);

    void dibujar() override;

    ~EntidadInterfaz() override = default;
};

#endif // ENTIDADINTERFAZ_H_
