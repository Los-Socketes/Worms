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
    float metrosSinDanio;
    float degtorad;
    float anchoViga;

    float velocidadMovimiento;
    float masaCuerpoEstatico;
    int distanciaEntreVigas;

    int tamanioGusano;
    float potenciaSalto;
    int vidaGusano;

    int esperaDinamita;
    int esperaProvision;
    int esperaViento;

    ConfiguracionServer();
};

#endif
