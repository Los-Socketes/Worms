#ifndef CLIENTE_HEADER
#define CLIENTE_HEADER

#include <string>
#include <vector>
#include "partida.h"
#include "protocolo.h"
#include "thread.h"
#include "threadSafeList.h"

#define strings std::vector<std::string> //-- Notas Macros para los contenedores

#define elegirPartida run

// Forward declaration para que el cliente le comunique al server
// No soy FAN de esto, pero mepa que es la mejor manera de estructurar
// el codigo para protejer los recursos
// class Server;

class Cliente : public Thread { //-- Notas Relacion Cliente - Jugador
// class Cliente {
private:
    Protocolo protocolo;

    bool conectadoAPartida;

    std::vector<std::string> mapasDisponibles;

    //Este nombre es una mierda. Cambiar
    TSList<Partida>& avisar;

public:
    Cliente(Socket&& socket, std::vector<std::string> mapasDisponibles,
	  TSList<Partida>& avisar);

    void run();


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
