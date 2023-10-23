#include "cliente.h"
#include "socket.h"

Cliente::Cliente(Socket &&socket)
    // :protocolo(socket)
{
};


void Cliente::mostrar(std::vector<std::string> mapasDisponibles) {
    this->protocolo.mostrar(mapasDisponibles);
}
