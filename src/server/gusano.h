#ifndef GUSANO_HEADER
#define GUSANO_HEADER

#include <box2d/box2d.h>
#include "defs.h"
#include "arma.h"
#include "box2dDefs.h"
#include <ctime>
#include <atomic>
//Arma que el gusano quiere crear
#define ArmaDeseada ArmaProtocolo

struct Ataque {
    b2Vec2 posicion;
    int potencia;
    int tiempoEspera;
    ArmaDeseada arma;
    Proyectil *proyectilAsociado;
};

struct condicionDeTurno {
    time_t cuandoArranco;
    bool usoSuArma;
    bool recibioDano;
};

class Gusano {
private:
    hp vida;
    DireccionGusano direccion;

    int idGusano;
    int tiempoQueMeQueda;


    //Guardo la ultima accion que realizo el gusano
    Accion ultimaAccion;

    condicionDeTurno turno;

    //Arma armaequipada;
    std::vector<Arma> armas;
    ArmaProtocolo armaEquipada;
    Arma* armaSeleccionada;

    EstadoGusano estado;

    //Conceptos de b2box
    b2Body* cuerpo;
    // int tiempoEspera;

    void setDireccion(DireccionGusano nuevaDireccion);

    //TODO Cambiar nombre
    void realizarMovimiento(Direccion direccionDeseada);

public:

    std::atomic<bool> golpeado;

    Gusano();

    Accion getUltimaAccion();

    [[nodiscard]] bool estaMuerto();

    //Es medio tetrica esta funcion si miras la implementacion
    [[nodiscard]] bool estaQuieto();

    /*
     * Le pregunta al gusano si su turno esta terminado o no.
     */
    [[nodiscard]] bool hayQueCambiarDeTurno(time_t tiempoActual);

    [[nodiscard]] int getTiempoQueMeQueda();

    void esMiTurno(time_t cuandoArranco);

    void giveId(int idGusano);

    void giveGun(ArmaProtocolo arma);

    void teletransportarse();

    void preparar(Accion& accion);

    //TODO: Esta funcion recibe el arma o tipo de dano
    void recibirDano(b2Vec2 golpe, Entidad *entidad);
    void recibirDanioCaida(b2Vec2 velocidad);
    // [[nodiscard]] std::pair<cambioX, cambioY> cambio(Accion accion);

    //WARNING: Esta funcion tiene que se ser llamada CERCA, sino AL
    //AL LADO del constructor. Gracias por nada c++, null pointers para
    //todos >:(
    void setCuerpo(b2Body* nuevoCuerpo);

    void setEstado(EstadoGusano nuevoEstado);
    EstadoGusano getEstado();

    int getId();

    [[nodiscard]] Ataque ejecutar(Accion accion, Proyectil *proyectil);

    [[nodiscard]] std::pair<coordX, coordY> getCoords();

    [[nodiscard]] RepresentacionGusano getRepresentacion();

    [[nodiscard]] std::pair<
        b2Vec2,
        std::pair<inicioCaja, finCaja>
        > ejecutarGolpe();
};

#endif
