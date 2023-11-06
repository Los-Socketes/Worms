#ifndef CLIENTE_HEADER
#define CLIENTE_HEADER

#include <string>
#include <vector>
#include "protocolo.h"
#include "sender.h"
#include "reciever.h"
#include "jugador.h"

#define elegirPartida run

class MonitorPartida;

class Cliente {
private:
    Protocolo protocolo;
    Sender sender;
    Reciever reciever;
    Jugador jugador;


public:
    Cliente(Socket&& socket, strings mapasDisponibles, MonitorPartida& monitorPartidas);

    void obtenerAccesoAAcciones(Queue<Direccion>* acciones);

    void enviarEstadoJuego(EstadoDelJuego estado);

    Jugador *getJugador();
};

#endif
/*
Notas a pie:
  Macros para los contenedores
    Pongo esto en una macro por dos motivos:
    1: El tip es *demasiado* largo y hace la funcion menos legible
    2: No se si vamos a cambiar vector por list o alguna otra
    estructura de la STL. Si lo hacemos, solo lo tenemos que cambiar
    aca y no en cada una de las apariciones de la funcion
    Protip: La idea de esta notacion es que sea "a la haskell".
    Si termina con "s", es que son varios aka es un ""array"" 
  Relacion Cliente - Jugador
    La clase cliente representa a alguien que se esta conectando al
    servidor. En el momento en el que el esa persona se conecta a una
    partida, se convierte en una clase Jugador. Esto lo hago para
    manterner separado el codigo de conexion inicial con el codigo de
    juego en si mismo
*/
