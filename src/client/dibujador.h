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
#include "gestoranimaciones.h"
#include "controliteracion.h"

using namespace SDL2pp;

class Dibujador {
 private:
      Camara& camara;
      std::shared_ptr<EstadoDelJuego>& estado_juego;
      int ancho_mapa;
      int alto_mapa;
      GestorAnimaciones gestor_animaciones;
      Font fuente;

      RepresentacionGusano getGusanoActual();
      std::pair<int, int> traducirCoordenadas(coordX& x, coordY& y);
      void dibujarReticula(std::pair<int, int>& posicion, radianes& angulo, int& direccion, ControlIteracion& iteraciones);
      void dibujarVida(Renderer& renderizador, std::pair<int, int>& posicion, hp& vida);
 public:
      Dibujador(Camara& camara, std::shared_ptr<EstadoDelJuego>& estado_juego, int ancho_mapa, int alto_mapa);
      void setDimensionMapa(int ancho, int alto);
      void inicializarAnimaciones(Renderer& renderizador);
      void dibujar(Renderer& renderizador, ControlIteracion& iteraciones, std::vector<RepresentacionViga>& vigas, std::pair<int, int>& pos_cursor);
      void dibujarMapa(std::vector<RepresentacionViga>& vigas);
      void dibujarGusanos(Renderer& renderizador, ControlIteracion& iteraciones, std::pair<int, int>& pos_cursor);
      void dibujarProyectiles(ControlIteracion& iteraciones);
      void dibujarAguaDetras(ControlIteracion& iteraciones);
      void dibujarAguaDelante(ControlIteracion& iteraciones);
      void dibujarBarraArmas(Renderer& renderizador, ArmaProtocolo& arma_equipada);
};

#endif // DIBUJADOR_H_