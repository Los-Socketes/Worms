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
      void dibujarReticula(std::pair<int, int>& posicion, radianes& angulo, int& direccion, ControlIteracion& iteraciones);
      void dibujarVida(Renderer& renderizador, std::pair<int, int>& posicion, hp& vida, colorJugador& color);
      void dibujarCuadradoPotencia(Renderer& renderizador, std::pair<int,int>& posicion, radianes& angulo, int& direccion, float& i);
      void dibujarBarraPotencia(Renderer& renderizador, std::pair<int,int>& posicion, radianes& angulo, int& direccion, float& potencia);
      void dibujarCuentaRegresiva(Renderer& renderizador, std::pair<int,int>& posicion, float& cuenta_regresiva);
 public:
      Dibujador(Camara& camara, std::shared_ptr<EstadoDelJuego>& estado_juego, int ancho_mapa, int alto_mapa);
      void setDimensionMapa(coordX& ancho, coordY& alto);
      void setIdJugador(int id);
      void inicializar(Renderer& renderizador, Mixer& mixer);
      void dibujar(Renderer& renderizador, ControlIteracion& iteraciones, std::vector<RepresentacionViga>& vigas, std::pair<int, int>& pos_cursor, std::vector<colorJugador>& colores);
      void dibujarMapa(std::vector<RepresentacionViga>& vigas);
      void dibujarGusanos(Renderer& renderizador, ControlIteracion& iteraciones, std::pair<int, int>& pos_cursor, std::vector<colorJugador>& colores);
      void dibujarProyectiles(Renderer& renderizador, ControlIteracion& iteraciones);
      void dibujarAguaDetras(ControlIteracion& iteraciones);
      void dibujarAguaDelante(ControlIteracion& iteraciones);
      void dibujarBarraArmas(Renderer& renderizador, ArmaProtocolo& arma_equipada);
      void dibujarBarrasVida(Renderer& renderizador, std::vector<colorJugador>& colores);
      void dibujarCuentaRegresivaTurno(Renderer& renderizador);
      void dibujarTextoTurno(Renderer& renderizador);
      void dibujarPantallaEspera(Renderer& renderizador);
      void dibujarFinalPartida(Renderer& renderizador, std::vector<colorJugador>& colores);
};

#endif // DIBUJADOR_H_