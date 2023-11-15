#ifndef DIBUJADOR_H_
#define DIBUJADOR_H_

#include <map>
#include <string>
#include <memory>
#include <SDL2pp/SDL2pp.hh>

#include "animacion.h"
#include "camara.h"
#include "defs.h"
#include "gestoranimaciones.h"

using namespace SDL2pp;

class Dibujador {
 private:
      Camara& camara;
      EstadoDelJuego& estado_juego;
      int ancho_mapa;
      int alto_mapa;
      GestorAnimaciones gestor_animaciones;
      Font fuente;

      void dibujarVida(Renderer& renderizador, std::pair<int, int> posicion, int vida);
 public:
      Dibujador(Camara& camara, EstadoDelJuego& estado_juego, int ancho_mapa, int alto_mapa);
      void inicializarAnimaciones(Renderer& renderizador);
      void dibujar(Renderer& renderizador, int it, radianes angulo, ArmaProtocolo arma_equipada);
      void dibujarMapa();
      void dibujarGusanos(Renderer& renderizador, int it, radianes angulo);
      // void dibujarProyectiles(int it);
      void dibujarAguaDetras(int it);
      void dibujarAguaDelante(int it);
      void dibujarBarraArmas(Renderer& renderizador, ArmaProtocolo arma_equipada);
};

#endif // DIBUJADOR_H_