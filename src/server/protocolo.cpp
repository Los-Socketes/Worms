#include "protocolo.h"

Protocolo::Protocolo(Socket&& socket):
    socket(std::move(socket)){};


int8_t Protocolo::obtenerCodigo() {
    bool was_closed = false;
    int8_t codigo;
    socket.recvall(&codigo, sizeof(codigo), &was_closed);
    return codigo;
}

std::vector<id> Protocolo::obtenerVector() {
    bool was_closed = false;
    int16_t cant;
    socket.recvall(&cant, sizeof(cant), &was_closed);
    // TODO: verificar
    cant = ntohs(cant);

    std::vector<id> partidas(cant, 0);
    socket.recvall(partidas.data(), cant*sizeof(id), &was_closed);
    // TODO: verificar

    for (int i = 0; i < (int)cant; i++) {
        partidas.at(i) = ntohl(partidas[i]);
    }
    return partidas;
}

std::vector<id> Protocolo::obtenerPartidas() {
    int8_t codigo = obtenerCodigo();
    // TODO: verificar (codigo)

    return obtenerVector();
}


std::vector<id> Protocolo::obtenerMapas() {
    int8_t codigo = obtenerCodigo();
    // TODO: verificar (codigo)

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

    id idPartida;
    if ((int)codigo != CREADA) {
        return idPartida; // no me convence
    }
    socket.recvall(&idPartida, sizeof(idPartida), &was_closed);
    // TODO: verificar
    
    idPartida = ntohl(idPartida);
    return idPartida;
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
    int8_t codigo = MOV;
    id idGusano = htonl(gusano);
    int8_t dir = direccion;

    bool was_closed = false;
    socket.sendall((char*)&codigo, sizeof(codigo), &was_closed);
    socket.sendall((char*)&idGusano, sizeof(idGusano), &was_closed);
    socket.sendall((char*)&dir, sizeof(dir), &was_closed);
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
    socket.sendall(paraEnviar.data(), sizeof(id)*cantMapas, &was_closed);
    

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

id Protocolo::obtenerId() {
    bool was_closed = false;
    id idEnviada;
    socket.recvall(&idEnviada, sizeof(idEnviada), &was_closed);
    // TODO: verificar
    idEnviada = ntohl(idEnviada);
    return idEnviada;
}

id Protocolo::obtenerMapaDeseado() {
    int8_t codigo = obtenerCodigo();
    // TODO: verificar
    return obtenerId();
}

id Protocolo::obtenerPartidaDeseada() {
    //Tenemos que ponernos de acuerdo en que devolver cuando el cliente
    //elige crear una partida nueva
    int8_t codigo = obtenerCodigo();
    // TODO: verificar
    return obtenerId();
}


