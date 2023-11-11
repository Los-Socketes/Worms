#ifndef DIBUJADOR_H_
#define DIBUJADOR_H_

#include <map>
#include <string>
#include <memory>
#include <SDL2pp/SDL2pp.hh>

#include "animacion.h"
#include "camara.h"
#include "defs.h"

class Dibujador {
 private:
      std::map<std::string, std::shared_ptr<Animacion>> animaciones;
      Camara& camara;
      EstadoDelJuego& estado_juego;
      int ancho_pantalla;
      int alto_pantalla;
      int ancho_mapa;
      int alto_mapa;
 public:
      Dibujador(Camara& camara, EstadoDelJuego& estado_juego, int ancho_pantalla, int alto_pantalla, int ancho_mapa, int alto_mapa);
      void inicializarAnimaciones(Renderer& renderizador);
      void dibujar(Renderer& renderizador, int it);
      void dibujarMapa();
      void dibujarGusanos(int it);
      // void dibujarProyectiles(int it);
      void dibujarAguaDetras(int it);
      void dibujarAguaDelante(int it);
};

#endif // DIBUJADOR_H_