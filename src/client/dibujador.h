#ifndef DIBUJADOR_H_
#define DIBUJADOR_H_

#include <map>
#include <string>
#include <memory>
#include <utility>
#include <SDL2pp/SDL2pp.hh>

#include "animacion.h"
#include "camara.h"
#include "defs.h"
#include "gestormultimedia.h"
#include "controliteracion.h"
#include "configuracionCliente.h"

using namespace SDL2pp;

// Clase que se encarga de dibujar todos los elementos del juego.
class Dibujador {
 private:
      Renderer& renderizador;
      Mixer& mixer;
      Camara& camara;
      std::shared_ptr<EstadoDelJuego>& estado_juego;
      int ancho_mapa;
      int alto_mapa;
      GestorMultimedia gestor_multimedia;
      std::map<ArmaProtocolo, std::string> teclas_armas;
      RepresentacionGusano gusano_actual;    
      Font fuente1;
      Font fuente2;
      int segundos_turno;
      bool esperando_movimiento;
      int idJugador;

      void actualizarGusanoActual();
      std::pair<int, int> traducirCoordenadas(coordX& x, coordY& y);
      void dibujarReticula(std::pair<int, int>& posicion,
          radianes& angulo, int& direccion, ControlIteracion& iteraciones);
      void dibujarVida(std::pair<int, int>& posicion, hp& vida, colorJugador& color);
      void dibujarCuadradoPotencia(std::pair<int,int>& posicion,
          radianes& angulo, int& direccion, float& i);
      void dibujarBarraPotencia(std::pair<int,int>& posicion,
          radianes& angulo, int& direccion, float& potencia);
      void dibujarCuentaRegresiva(std::pair<int,int>& posicion, float& cuenta_regresiva);
      void dibujarMapa(std::vector<RepresentacionViga>& vigas);
      void dibujarGusanos(ControlIteracion& iteraciones,
          std::pair<int, int>& pos_cursor, std::vector<colorJugador>& colores);
      void dibujarProyectiles(ControlIteracion& iteraciones);
      void dibujarAguaDetras(ControlIteracion& iteraciones);
      void dibujarAguaDelante(ControlIteracion& iteraciones);
      void dibujarBarraArmas(ArmaProtocolo& arma_equipada);
      void dibujarMuniciones(RepresentacionArma& arma);
      void dibujarBarrasVida(std::vector<colorJugador>& colores);
      void dibujarCuentaRegresivaTurno();
      void dibujarTextoTurno();
      void dibujarPantallaEspera(MomentoDePartida& momento, 
          std::vector<colorJugador>& colores, bool& es_host);
      void dibujarFinalPartida(std::vector<colorJugador>& colores);
 public:
      Dibujador(Renderer& renderizador,
          Mixer& mixer,
          Camara& camara,
          std::shared_ptr<EstadoDelJuego>& estado_juego,
          int ancho_mapa, int alto_mapa);

      void setDimensionMapa(coordX& ancho, coordY& alto);
      void setIdJugador(int id);
      void reproducirSonido(TipoSonido tipo); // A refactorizar.

      void dibujar(ControlIteracion& iteraciones,
          std::vector<RepresentacionViga>& vigas,
          std::pair<int, int>& pos_cursor,
          std::vector<colorJugador>& colores,
          bool& es_host);
};

#endif // DIBUJADOR_H_