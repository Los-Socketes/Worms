#ifndef ARMA_HEADER
#define ARMA_HEADER


#include "defs.h"

// struct MisArmas

// Gusano {
//     Arma armaEquipada; 

//     EstadoArma 
// }

struct CaracteristicasArma {
    bool tieneMira;
    bool esCuerpoACuerpo;
    bool tienePotenciaVariable;
    bool tieneCuentaRegresiva;
    bool esTeledirigido;
    bool esAfectadoPorViento;
};

struct Danio {
    int epicentro;
    int radio;
};

class Arma {
private: 
    ArmaProtocolo idArma;
    CaracteristicasArma caracteristicas;
    int municiones;
    int fragmentos;

    Danio danio;
    Danio danioFragmento;
    float potencia;
    int cuentaRegresiva;
    std::pair<coordX, coordY> coordenadasTeletransporte;

    float angulo;

public:
    Arma(ArmaProtocolo idArma);

    // TODO: devolver cosas a configurar
    CaracteristicasArma getCaracteristicas();

    Danio getDanio();
    Danio getDanioFragmento();

    // -1 es equivalente a infinitas
    int getMuniciones();

    float getPotencia();
    void setPotencia(float potencia);

    float getAngulo();
    void setAngulo(float angulo);

    int getFragmentos();

    int getCuentaRegresiva();
    void setCuentaRegresiva(int cuentaRegresiva);

    // Esto no es atacar, es un actualizar valores
    // bool usar();

    RepresentacionArma getRepresentacion();

    std::pair<coordX, coordY> getCoordenadasTeletransporte();
    void setCoordenadasTeletransporte(std::pair<coordX, coordY> coordenadas);
    // TODO: hacer todo el cpp y revisar la estructura
    // (esta horrible, lo se)

};


/* CHARLA DE GUIA: 
c: equipar X
s: ok, estado
c: arranco a atacar
s: estadoGusano = configurarAngulo | estadoArma: apuntando X angulo
c: cambio angulo
s: ok, seteo y mando estado
c: listo el angulo
s: ok, estadoGusano = configurarPotencia
c: cambio potencia
s: ok, seteo y mando estado
c: listo el potencia
s: ok, ataco :p + muchos estados de como pasan las cosas
*/

#endif
