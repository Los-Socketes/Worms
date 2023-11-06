#ifndef SERVER_HEADER
#define SERVER_HEADER

#include "aceptador.h"
#include <string>

class Server {
private:
    Aceptador aceptador;

    //Esto lo hago constante porque es fijo. Se tiene que leer de
    //un archivo
    //TODO: Leer de un archivo
    const std::vector<std::string> escenariosDisponibles = {
        "El pico de la Viuda", "Battlesnax"
    };



public:
    Server(const char *puerto);

    void esperarQueSeCierre();

};



#endif
