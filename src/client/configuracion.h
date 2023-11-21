#ifndef CONFIGURACION_H_
#define CONFIGURACION_H_

#include <pair>

class Configuracion {
private:
    std::pair<int, int> dimensionesIniciales;
    int volumenInicial;

public:
    Configuracion();
    std::pair<int, int> getDimensionesIniciales();
    int getVolumenInicial();
}

#endif
