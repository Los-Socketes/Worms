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

class Dibujador {
 private:
      Camara& camara;
      EstadoDelJuego& estado_juego;
      int ancho_mapa;
      int alto_mapa;
      GestorAnimaciones gestor_animaciones;
 public:
      Dibujador(Camara& camara, EstadoDelJuego& estado_juego, int ancho_mapa, int alto_mapa);
      void inicializarAnimaciones(Renderer& renderizador);
      void dibujar(Renderer& renderizador, int it, radianes angulo);
      void dibujarMapa();
      void dibujarGusanos(int it, radianes angulo);
      // void dibujarProyectiles(int it);
      void dibujarAguaDetras(int it);
      void dibujarAguaDelante(int it);
};

#endif // DIBUJADOR_H_