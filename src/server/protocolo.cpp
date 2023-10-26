#include "protocolo.h"

Protocolo::Protocolo(Socket&& socket):
    socket(std::move(socket)){};


std::vector<int32_t> Protocolo::obtenerPartidas() {
    bool was_closed = false;
    int8_t codigo;
    socket.recvall(&codigo, sizeof(codigo), &was_closed);
    // TODO: verificar (was_closed + codigo)

    int16_t cant;
    socket.recvall(&cant, sizeof(cant), &was_closed);
    // TODO: verificar
    cant = ntohs(cant);

    std::vector<int32_t> partidas(cant, 0);
    socket.recvall(partidas.data(), cant*sizeof(int32_t), &was_closed);
    // TODO: verificar

    for (int i = 0; i < (int)cant; i++) {
        partidas.at(i) = ntohl(partidas[i]);
    }
    return partidas;
}


std::vector<int32_t> Protocolo::obtenerMapas() {

}

int32_t Protocolo::crearPartida(int32_t mapaSeleccionado) {
    int8_t codigo = CREAR;
    int32_t mapa = htonl(mapaSeleccionado);

    bool was_closed;
    socket.sendall((char*)&codigo, sizeof(codigo), &was_closed);
    socket.sendall((char*)&mapa, sizeof(mapa), &was_closed);
    // TODO: verificar

    socket.recvall(&codigo, sizeof(codigo), &was_closed);
    // TODO: verificar
    int32_t codigoPartida;
    if ((int)codigo != CREADA) {
        return codigoPartida; // no me convence
    }
    socket.recvall(&codigoPartida, sizeof(codigoPartida), &was_closed);
    // TODO: verificar
    
    codigoPartida = ntohl(codigoPartida);
    return codigoPartida;
}

bool Protocolo::unirseAPartida(int32_t id) {

}

void Protocolo::moverGusano(int32_t gusano, Direccion direccion) {

}




void Protocolo::enviarMapas(std::vector<int32_t> mapasDisponibles) {
    //Como diria sisop "Your code here"

}

void Protocolo::enviarPartidas(std::vector<int32_t> partidasDisponibles) {
    //Como diria sisop "Your code here"
    int8_t codigo = PARTIDAS;
    uint16_t cant = htons(partidasDisponibles.size());

    std::vector<int32_t> paraEnviar;
    for (auto &&partida : partidasDisponibles) {
        paraEnviar.push_back(htonl(partida));
    }
    bool was_closed = false;
    /* TODO: agregar ifs para ver si se cerro el socket
            mover estos sendall a otro metodo
            hacer que este metodo devuelva 1 string/algo con toda la info para hacer
                mas facil el testeo
    */
    socket.sendall((char*)&codigo, sizeof(codigo), &was_closed);
    socket.sendall((char*)&cant, sizeof(cant), &was_closed);
    socket.sendall(paraEnviar.data(), sizeof(int32_t)*paraEnviar.size(), &was_closed);
    
}

int32_t Protocolo::obtenerPartidaDeseada() {
    //Como diria sisop "Your code here"
    //Tenemos que ponernos de acuerdo en que devolver cuando el cliente
    //elige crear una partida nueva

    // return "Devuelvo esto solamente para que compile";
    return 0;
}

