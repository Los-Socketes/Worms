#ifndef SERVER_HEADER
#define SERVER_HEADER

#include "aceptador.h"
#include <string>
#include "monitorPartida.h"

class Server {
private:
    Aceptador aceptador;

    //Esto lo hago constante porque es fijo. Se tiene que leer de
    //un archivo
    //TODO: Leer de un archivo
    const std::vector<std::string> escenariosDisponibles = {
        "El pico de la Viuda", "Battlesnax"
    };

    MonitorPartida partidas;


public:
    Server(const char *puerto);

    void esperarQueSeCierre();

};



#endif
