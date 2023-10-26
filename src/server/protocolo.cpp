#include "protocolo.h"

Protocolo::Protocolo(Socket&& socket):
    socket(std::move(socket)){};

void Protocolo::mostrarMapas(std::vector<std::string> mapasDisponibles) {
    //Como diria sisop "Your code here"

}

void Protocolo::mostrarPartidas(std::vector<std::string> partidasDisponibles) {
    //Como diria sisop "Your code here"

}
int Protocolo::obtenerPartidaDeseada() {
    //Como diria sisop "Your code here"
    //Tenemos que ponernos de acuerdo en que devolver cuando el cliente
    //elige crear una partida nueva

    // return "Devuelvo esto solamente para que compile";
    return 0;
}
