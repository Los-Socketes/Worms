#ifndef ARMA_HEADER
#define ARMA_HEADER


#include "defs.h"

// struct MisArmas

// Gusano {
//     Arma armaEquipada; 

//     EstadoArma 
// }

class Arma {
private: 
    ArmaProtocolo idArma;
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

    // TODO: devolver cosas a configurar

    std::pair<int,int> getDanio();
    std::pair<int,int> getDanioFragmento();

    // -1 es equivalente a infinitas
    int getMuniciones();

    float getPotencia();
    void setPotencia(float potencia);

    int getFragmentos();

    int getCuentaRegresiva();
    void setCuentaRegresiva(int cuentaRegresiva);

    // Esto no es atacar, es un actualizar valores
    bool usar();

    RepresentacionArma getRepresentacion();

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
