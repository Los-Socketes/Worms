#ifndef PROTOCOLO_HEADER
#define PROTOCOLO_HEADER

#include "socket.h"
#include <string>
#include <vector>

//TODO Poner todos los types defs en un header file juntos
typedef uint id;

class Protocolo {
private:
    Socket socket;
public:
    // Pongo estas para que compile nomas. Ponele el nombre/encaralas de
    // la forma que te parezca
    void mostrarMapas(std::vector<std::string> mapasDisponibles); 

    void mostrarPartidas(std::vector<std::string> partidasDisponibles); 

    [[nodiscard]] id obtenerPartidaDeseada();

    Protocolo(Socket&& socket);

};

#endif
