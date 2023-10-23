#ifndef CLIENTE_HEADER
#define CLIENTE_HEADER

#include "socket.h"
#include <string>
#include <vector>

#define strings std::vector<std::string> //-- Notas Macros para los contenedores

class Cliente {
private:
    /* ServerProtocol protocolo; */

public:
    bool enPartida; //-- Notas Funciones para la conexion inicial


public:
    Cliente(Socket&& socket);

    void mostrar(strings mapasDisponibles); //-- Notas Funciones para la conexion inicial





};



#endif
/*
Notas a pie:
  Funciones para la conexion inicial
    Las funciones de aca abajo solo se usan cuando el cliete se
    conecta por primera vez. 
    La variable "enPartida" es simplemente usada para atrapar macanas
    en tiempo de corrida. No tiene mucha razon "logica" de ser mas
    alla de eso. Por eso no le hago getter y setter. En teoria, cuando
    el tp este listo, podriamos sacarla.
    Idealmente deberia ser una macro.
    TODO: Hacerla macro
  Macros para los contenedores
    Pongo esto en una macro por dos motivos:
    1: Es *demasiado* largo y hace la funcion menos legible
    2: No se si vamos a cambiar vector por list o alguna otra
    estructura de la STL. Si lo hacemos, solo lo tenemos que cambiar
    aca y no en cada una de las apariciones de la funcion
    Protip: La idea de esta notacion es que sea "a la haskell".
    Si termina con "s", es que son varios aka es un ""array"" 
     
*/
