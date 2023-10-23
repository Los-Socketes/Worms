#ifndef CLIENTE_HEADER
#define CLIENTE_HEADER

#include "socket.h"
#include <string>
#include <vector>
#include "protocolo.h"

#define strings std::vector<std::string> //-- Notas Macros para los contenedores

class Cliente { //-- Notas Relacion Cliente - Jugador
private:
    /* ServerProtocol protocolo; */
    Protocolo protocolo;


public:
    Cliente(Socket&& socket);

    void mostrar(strings mapasDisponibles); 

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
