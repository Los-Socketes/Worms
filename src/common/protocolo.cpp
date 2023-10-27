#include "protocolo.h"
#include <cstdint>
#include <vector>

Protocolo::Protocolo(Socket&& socket):
    socket(std::move(socket)){};


// METODOS PRIVADOS

id Protocolo::obtenerId() {
    bool was_closed = false;
    id idEnviada;
    socket.recvall(&idEnviada, sizeof(idEnviada), &was_closed);
    // TODO: verificar
    idEnviada = ntohl(idEnviada);
    return idEnviada;
}


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


id Protocolo::verificarConexion() {
    bool was_closed = false;
    int8_t codigo;
    socket.recvall(&codigo, sizeof(codigo), &was_closed);
    // TODO: verificar

    id idPartida = -1;
    if ((int)codigo == ERROR) {
        return idPartida;
    }
    // TODO: quizas ver si no es exito -> tirar excepcion
    socket.recvall(&idPartida, sizeof(idPartida), &was_closed);
    // TODO: verificar
    
    idPartida = ntohl(idPartida);
    return idPartida;
}


//METODOS DEL CLIENTE

#ifdef CLIENT
bool Protocolo::pedirInformacion(tipoInfo infoAPedir) {
    int8_t pedidoAEnviar[2] = {PEDIDO, (int8_t)infoAPedir};

    bool was_closed = false;
    socket.sendall((char*)pedidoAEnviar, sizeof(pedidoAEnviar), &was_closed);
    return !was_closed;
}


std::vector<id> Protocolo::obtenerMapas() {
    int8_t codigo = obtenerCodigo();
    // TODO: verificar (codigo)

    return obtenerVector();
}


std::vector<id> Protocolo::obtenerPartidas() {
    int8_t codigo = obtenerCodigo();
    // TODO: verificar (codigo)

    return obtenerVector();
}


id Protocolo::crearPartida(id mapaSeleccionado) {
    int8_t codigo = CREAR;
    int32_t mapa = htonl(mapaSeleccionado);

    bool was_closed = false;
    socket.sendall((char*)&codigo, sizeof(codigo), &was_closed);
    if (was_closed) {
        return false;
    }
    socket.sendall((char*)&mapa, sizeof(mapa), &was_closed);
    if (was_closed) {
        return false;
    }

    return verificarConexion();
}


bool Protocolo::unirseAPartida(id id) {
    int8_t codigo = UNIRSE;
    int32_t idPartida = htonl(id);

    bool was_closed = false;
    socket.sendall((char*)&codigo, sizeof(codigo), &was_closed);
    if (was_closed) {
        return false;
    }
    socket.sendall((char*)&idPartida, sizeof(idPartida), &was_closed);
    if (was_closed) {
        return false;
    }

    return verificarConexion();
}


bool Protocolo::moverGusano(id gusano, Direccion direccion) {
    int8_t codigo = MOV;
    id idGusano = htonl(gusano);
    int8_t dir = direccion;

    bool was_closed = false;
    socket.sendall((char*)&codigo, sizeof(codigo), &was_closed);
    if (was_closed) {
        return false;
    }
    socket.sendall((char*)&idGusano, sizeof(idGusano), &was_closed);
    if (was_closed) {
        return false;
    }
    socket.sendall((char*)&dir, sizeof(dir), &was_closed);
    if (was_closed) {
        return false;
    }
    return !was_closed;
}

//Endif de la macro de CLIENT
#endif


// METODOS DEL SERVER

#ifdef SERVER
tipoInfo Protocolo::obtenerPedido() {
    int8_t pedidoARecibir[2] = {0};
    bool was_closed = false;
    socket.recvall(pedidoARecibir, sizeof(pedidoARecibir), &was_closed);
    // TODO: verificar
    return (tipoInfo)pedidoARecibir[1];
}


bool Protocolo::enviarMapas(std::vector<std::string> mapasDisponibles) {
    int8_t codigo = MAPAS;
    int cantMapas = mapasDisponibles.size();
    uint16_t cant = htons(cantMapas);

    // por ahora solo enviamos ids, despues vemos de mandar mas info (nombre)
    std::vector<id> paraEnviar;
    for (int32_t i = 0; i < (int32_t)cantMapas; i++) {
        id idMapa = htonl(i);
        paraEnviar.push_back(idMapa);
    }
    

    bool was_closed = false;
    socket.sendall((char*)&codigo, sizeof(codigo), &was_closed);
    if (was_closed) {
        return false;
    }
    socket.sendall((char*)&cant, sizeof(cant), &was_closed);
    if (was_closed) {
        return false;
    }
    socket.sendall(paraEnviar.data(), sizeof(id)*cantMapas, &was_closed);
    return !was_closed;
}

bool Protocolo::enviarPartidas(std::vector<RepresentacionPartida> partidasDisponibles) {
    int8_t codigo = PARTIDAS;
    int cantPartidas = partidasDisponibles.size();
    uint16_t cant = htons(cantPartidas);

    std::vector<id> paraEnviar;
    for (auto &&partida : partidasDisponibles) {
        id idMapa = htonl(partida.ID);
        paraEnviar.push_back(idMapa);
    }

    bool was_closed = false;
    /* TODO: mover estos sendall a otro metodo
            hacer que este metodo devuelva 1 string/algo con toda la info para hacer
                mas facil el testeo
    */
    socket.sendall((char*)&codigo, sizeof(codigo), &was_closed);
    if (was_closed) {
        return false;
    }
    socket.sendall((char*)&cant, sizeof(cant), &was_closed);
    if (was_closed) {
        return false;
    }
    socket.sendall(paraEnviar.data(), sizeof(int32_t)*cantPartidas, &was_closed);
    return !was_closed;    
}


id Protocolo::obtenerMapaDeseado() {
    int8_t codigo = obtenerCodigo();
    // TODO: verificar
    return obtenerId();
}


id Protocolo::obtenerPartidaDeseada() {
    int8_t codigo = obtenerCodigo();
    // TODO: verificar
    return obtenerId();
}


bool Protocolo::enviarConfirmacion(id idPartida) {
    int8_t codigo = EXITO;
    int32_t idAEnviar = htonl(idPartida);

    bool was_closed = false;
    socket.sendall((char*)&codigo, sizeof(codigo), &was_closed);
    if (was_closed) {
        return false;
    }
    socket.sendall((char*)&idAEnviar, sizeof(idAEnviar), &was_closed);
    return !was_closed;
}


bool Protocolo::enviarError() {
    int8_t codigo = ERROR;

    bool was_closed = false;
    socket.sendall((char*)&codigo, sizeof(codigo), &was_closed);
    return !was_closed; // si sigue abierto (false) -> devuelve true
}


Direccion Protocolo::obtenerAccion() {
    int8_t codigo;
    bool was_closed = false;

    socket.recvall(&codigo, sizeof(codigo), &was_closed);
    id idGusano;
    socket.recvall(&idGusano, sizeof(idGusano), &was_closed);
    idGusano = ntohl(idGusano);

    int8_t dir;
    socket.recvall(&dir, sizeof(dir), &was_closed);
    // TODO: verificar

    return (Direccion)dir;
}
//Endif de la macro de SERVER
#endif
