#ifndef ENTIDADGUSANO_H_
#define ENTIDADGUSANO_H_

#include <SDL2pp/SDL2pp.hh>

#include "defs.h"
#include "entidadActualizable.h"
#include "../configuracionCliente.h"
#include "../gestorAnimaciones.h"
#include "../gestorSonidos.h"

// Clase que representa a los gusanos.
class EntidadGusano : public EntidadActualizable {
 private:
    id id_gusano;
    int id_jugador;
    GestorSonidos& gestor_sonidos;
    std::vector<colorJugador>& colores;
    void dibujarVida(std::pair<int, int>& posicion, hp& vida, colorJugador& color);
    void dibujarCuadradoPotencia(std::pair<int,int>& posicion, radianes& angulo,
        int& direccion, float& i);
    void dibujarBarraPotencia(std::pair<int,int>& posicion, radianes& angulo,
        int& direccion, float& potencia);
    void reproducirSonidoGusano(std::shared_ptr<Animacion>& animacion,
        EstadoGusano& estado, ArmaProtocolo& arma);
    void dibujarGusano(EstadoGusano& estado, RepresentacionArma& arma,
        DireccionGusano& dir, int& pos_x, int& pos_y);
    
 public:
    EntidadGusano(GestorAnimaciones& gestor_animaciones,
        Renderer& renderizador,
        std::shared_ptr<EstadoDelJuego>& estado_juego,
        id id_gusano,
        int id_jugador,
        GestorSonidos& gestor_sonidos,
        Camara& camara,
        std::vector<colorJugador>& colores,
        Font& fuente1,
        Font& fuente2);

    void dibujar() override;

    ~EntidadGusano() override = default;
};

#endif // ENTIDADGUSANO_H_