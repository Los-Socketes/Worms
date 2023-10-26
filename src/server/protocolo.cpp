#include "protocolo.h"

Protocolo::Protocolo(Socket&& socket):
    socket(std::move(socket)){};


id_t Protocolo::crearPartida(std::string mapaSeleccionado) {

}

bool Protocolo::unirseAPartida(id_t id) {

}

void Protocolo::moverGusano(id_t gusano, Direccion direccion) {

}




void Protocolo::enviarMapas(std::vector<std::string> mapasDisponibles) {
    //Como diria sisop "Your code here"

}

void Protocolo::enviarMapas(std::vector<std::string> partidasDisponibles) {
    //Como diria sisop "Your code here"

}
int Protocolo::obtenerPartidaDeseada() {
    //Como diria sisop "Your code here"
    //Tenemos que ponernos de acuerdo en que devolver cuando el cliente
    //elige crear una partida nueva

    // return "Devuelvo esto solamente para que compile";
    return 0;
}

