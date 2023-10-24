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

    operator std::string() const {
        return std::to_string(this->ID);
    }
};

class Server {
private:
    // Recibidor recibidor;
    Socket aceptador;
    
    //Esta funcion se tiene que correr en un thread
    void recibirCliente();
    std::vector<Cliente *> lobby;

    //Esto lo hago constante porque solo es fijo. Se tiene que leer de
    //un archivo
    const std::vector<std::string> escenariosDisponibles = {
        "El pico de la Viuda", "Battlesnax"
    };

    // En teoria, un vector es mas ineficiente que una lista porque
    // tiene que realocar TODOS los elementos cada vez que se saca
    // un elemento. Sin embargo, el realocar constantemente
    // nos asegura que no va a haber espacios vacios en el
    //medio, lo cual facilita el obtener y modificar sus id
    std::vector<Partida> partidas;

    [[nodiscard]] std::vector<RepresentacionPartida> getRepresentacionPartidas();

public:
    Server(const char *puerto);

    //TODO Preguntar que queda mas bonito a mis compis UwU
    // void anadirJugadorAPartida(
    void anadirJugadorAPartida(Protocolo&& protocoloJugador, id idPartida);

};



#endif
