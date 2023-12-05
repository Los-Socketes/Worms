#ifndef CONFIGURACION_CLIENTE_H_
#define CONFIGURACION_CLIENTE_H_

#define ROJO 0
#define VERDE 1
#define AZUL 2

#include <vector>
#include "defs.h"
#include <cstdint>

typedef std::vector<uint8_t> colorJugador;

// Clase que se encarga de leer el archivo de configuracion del cliente.
class ConfiguracionCliente {
public:
    int viento_max;
    int viento_min;
    int tiempo_turno;
    int tiempo_changui;
    std::pair<int, int> dimensionesIniciales;
    int volumenInicial;
    std::vector<colorJugador> coloresJugadores;
    int pixelsPorMetro;


    ConfiguracionCliente();
    int getVientoMax();
    int getVientoMin();
    int getTiempoTurno();
    int getTiempoChangui();
    std::pair<int, int> getDimensionesIniciales();
    int getVolumenInicial();
    std::vector<colorJugador>& getColoresJugadores();
    colorJugador getColorJugador(int numJugador);
    int getPixelsPorMetro();
};

#endif
