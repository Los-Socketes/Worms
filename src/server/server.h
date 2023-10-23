#ifndef SERVER_HEADER
#define SERVER_HEADER

// #include "recibidor.h"
#include "partida.h"
#include "socket.h"

#include "cliente.h"
#include <cstdint>
#include <string>
#include <vector>

struct RepresentacionPartida {
    std::string ID;
};

class Server {
private:
    // Recibidor recibidor;
    Socket aceptador;
    
    void recibirCliente();

    //Esto lo hago constante porque solo es fijo. Se tiene que leer de
    //un archivo
    const std::vector<std::string> escenariosDisponibles;

    // TODO: Preguntar si esto no es una race condition.
    // Entiendo que si lo es porque se va a modificar en tiempo de
    // corrida y va a haber threads leyendo y escribiendo en ella.
    std::vector<Partida> partidas;
    //Este counter va a hacer de ID para la partida
    uint partidaCounter;
    [[nodiscard]] std::vector<RepresentacionPartida> getRepresentacionPartidas();

public:
    Server(const char *puerto);
};



#endif
