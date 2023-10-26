#include "protocolo.h"

Protocolo::Protocolo(Socket&& socket):
    socket(std::move(socket)){};


std::vector<id> Protocolo::obtenerVector() {
    bool was_closed = false;
    int16_t cant;
    socket.recvall(&cant, sizeof(cant), &was_closed);
    // TODO: verificar
    cant = ntohs(cant);

    std::vector<id> partidas(cant, 0);
    socket.recvall(partidas.data(), cant*sizeof(int32_t), &was_closed);
    // TODO: verificar

    for (int i = 0; i < (int)cant; i++) {
        partidas.at(i) = ntohl(partidas[i]);
    }
    return partidas;
}

std::vector<id> Protocolo::obtenerPartidas() {
    bool was_closed = false;
    int8_t codigo;
    socket.recvall(&codigo, sizeof(codigo), &was_closed);
    // TODO: verificar (was_closed + codigo)

    return obtenerVector();
}


std::vector<id> Protocolo::obtenerMapas() {
    bool was_closed = false;
    int8_t codigo;
    socket.recvall(&codigo, sizeof(codigo), &was_closed);
    // TODO: verificar (was_closed + codigo)

    return obtenerVector();
}


id Protocolo::crearPartida(id mapaSeleccionado) {
    int8_t codigo = CREAR;
    int32_t mapa = htonl(mapaSeleccionado);

    bool was_closed = false;
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


bool Protocolo::unirseAPartida(id id) {
    int8_t codigo = UNIRSE;
    int32_t idPartida = htonl(id);

    bool was_closed = false;
    socket.sendall((char*)&codigo, sizeof(codigo), &was_closed);
    // TODO: verificar
    socket.sendall((char*)idPartida, sizeof(idPartida), &was_closed);
    // TODO: verificar

    // TODO: ver que la respuesta sea afirmativa y devolver
    return true; // para que funcione por ahora
}

void Protocolo::moverGusano(id gusano, Direccion direccion) {

}



std::vector<char*> Protocolo::vectorListoParaEnviar(std::vector<id> vectorAEnviar) {
    std::vector<char*> paraEnviar;
    for (auto &&elemento : vectorAEnviar) {
        int32_t valorAEnviar = htonl(elemento);
        paraEnviar.push_back((char*)&valorAEnviar);
    }
    return paraEnviar;
}

void Protocolo::enviarMapas(std::vector<id> mapasDisponibles) {
    int8_t codigo = MAPAS;
    int cantMapas = mapasDisponibles.size();
    uint16_t cant = htons(cantMapas);

    std::vector<char*> paraEnviar = vectorListoParaEnviar(mapasDisponibles);

    bool was_closed = false;
    socket.sendall((char*)&codigo, sizeof(codigo), &was_closed);
    socket.sendall((char*)&cant, sizeof(cant), &was_closed);
    socket.sendall(paraEnviar.data(), sizeof(int32_t)*cantMapas, &was_closed);
    

}

void Protocolo::enviarPartidas(std::vector<id> partidasDisponibles) {
    int8_t codigo = PARTIDAS;
    int cantPartidas = partidasDisponibles.size();
    uint16_t cant = htons(cantPartidas);

    std::vector<char*> paraEnviar = vectorListoParaEnviar(partidasDisponibles);
    bool was_closed = false;
    /* TODO: agregar ifs para ver si se cerro el socket
            mover estos sendall a otro metodo
            hacer que este metodo devuelva 1 string/algo con toda la info para hacer
                mas facil el testeo
    */
    socket.sendall((char*)&codigo, sizeof(codigo), &was_closed);
    socket.sendall((char*)&cant, sizeof(cant), &was_closed);
    socket.sendall(paraEnviar.data(), sizeof(int32_t)*cantPartidas, &was_closed);
    
}

id Protocolo::obtenerMapaDeseado() {
    bool was_closed = false;
    int8_t codigo;
    socket.recvall(&codigo, sizeof(codigo), &was_closed);
    // TODO: verificar
    int32_t idMapa;
    socket.recvall(&idMapa, sizeof(idMapa), &was_closed);


}

id Protocolo::obtenerPartidaDeseada() {
    //Como diria sisop "Your code here"
    //Tenemos que ponernos de acuerdo en que devolver cuando el cliente
    //elige crear una partida nueva

    // return "Devuelvo esto solamente para que compile";
    return 0;
}

