#ifndef CONFIG_SERVER_HEADER
#define CONFIG_SERVER_HEADER

#include <vector>
#include "defs.h"
#include <cstdint>

// Clase que se encarga de leer el archivo de configuracion del server.
class ConfiguracionServer {
public:
    std::pair<float, float> gravedad;
    std::pair<int, int> viento;

    int maxViento;
    int minViento;

    float metrosSinDanio;

    float velocidadMovimiento;

    float potenciaSalto;
    int vidaGusano;

    int esperaDinamita;
    int esperaProvision;
    int esperaViento;

    ConfiguracionServer();
};

#endif
