#ifndef ARMA_HEADER
#define ARMA_HEADER


#include "defs.h"

class Arma {
private: 
    bool tieneMira;
    bool esCuerpoACuerpo;
    bool tienePotenciaVariable;
    bool tieneCuentaRegresiva;
    bool esTeledirigido;
    bool esAfectadoPorViento;
    int municiones;
    int fragmentos;

    std::pair<int,int> danio;
    std::pair<int,int> danioFragmento;
    float potencia;
    int cuentaRegresiva;

public:
    Arma(ArmaProtocolo idArma);

    std::pair<int,int> getDanio();
    std::pair<int,int> getDanioFragmento();

    // -1 es equivalente a infinitas
    int getMuniciones();

    float getPotencia();
    void setPotencia(float potencia);

    int getFragmentos();

    int getCuentaRegresiva();
    void setCuentaRegresiva(int cuentaRegresiva);

    bool usar();

    // TODO: hacer todo el cpp y revisar la estructura
    // (esta horrible, lo se)

};

#endif
