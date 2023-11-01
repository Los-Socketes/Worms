#ifndef SERVER_HEADER
#define SERVER_HEADER

#include "aceptador.h"
#include "partida.h"
#include "socket.h"

#include "cliente.h"

#include <cstdint>
#include <string>
#include <vector>
#include "monitorPartida.h"

// struct RepresentacionPartida {
//     int ID;

//     operator std::string() const {
//         return std::to_string(this->ID);
//     }
// };

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
