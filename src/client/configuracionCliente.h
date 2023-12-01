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
private:
    std::pair<int, int> dimensionesIniciales;
    int volumenInicial;
    std::vector<colorJugador> coloresJugadores;
    int pixelsPorMetro;

public:
    ConfiguracionCliente();
    std::pair<int, int> getDimensionesIniciales();
    int getVolumenInicial();
    std::vector<colorJugador>& getColoresJugadores();
    colorJugador getColorJugador(int numJugador);
    int getPixelsPorMetro();
};

#endif
