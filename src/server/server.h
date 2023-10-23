#ifndef SERVER_HEADER
#define SERVER_HEADER

// #include "recibidor.h"
#include "partida.h"
#include "socket.h"

#include "cliente.h"
#include <cstdint>
#include <string>
#include <vector>

typedef uint id;

struct RepresentacionPartida {
    id ID;
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
    // En teoria, un vector es mas ineficiente que una lista porque
    // tiene que realocar TODOS los elementos cada vez que se saca
    // un elemento. Sin embargo, el realocar constantemente
    // nos asegura que no va a haber espacios vacios en el
    //medio, lo cual facilita el obtener y modificar sus id
    std::vector<Partida> partidas;

    [[nodiscard]] std::vector<RepresentacionPartida> getRepresentacionPartidas();

public:
    Server(const char *puerto);
};



#endif
