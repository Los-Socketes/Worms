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
    if (was_closed) {
        return INVAL_ID;
    }

    idEnviada = ntohl(idEnviada);
    return idEnviada;
}


bool Protocolo::enviarId(id ID) {
    id idAEnviar = htonl(ID);
    bool was_closed = false;
    socket.sendall(&idAEnviar, sizeof(idAEnviar), &was_closed);
    return !was_closed;
}


int8_t Protocolo::obtenerCodigo() {
    bool was_closed = false;
    int8_t codigo;
    socket.recvall(&codigo, sizeof(codigo), &was_closed);
    return (was_closed)? (int8_t)-1 : codigo;
}


bool Protocolo::enviarCodigo(int codigo) {
    int8_t codigoAEnviar = codigo;
    bool was_closed = false;
    socket.sendall(&codigoAEnviar, sizeof(codigoAEnviar), &was_closed);
    return !was_closed;
}


bool Protocolo::enviarCantidad(int cant) {
    int16_t cantAEnviar = htons(cant);
    bool was_closed = false;
    socket.sendall(&cantAEnviar, sizeof(cantAEnviar), &was_closed);
    return !was_closed;
}


std::vector<id> Protocolo::obtenerVector() {
    bool was_closed = false;
    int16_t cant;
    std::vector<id> error;
    socket.recvall(&cant, sizeof(cant), &was_closed);
    if (was_closed) {
        return error;
    }
    cant = ntohs(cant);

    std::vector<id> partidas(cant, 0);
    socket.recvall(partidas.data(), cant*sizeof(id), &was_closed);
    if (was_closed) {
        return error;
    }

    for (int i = 0; i < (int)cant; i++) {
        partidas.at(i) = ntohl(partidas[i]);
    }
    return partidas;
}


id Protocolo::verificarConexion() {
    bool was_closed = false;
    int8_t codigo = obtenerCodigo();
    if ((int)codigo == -1 || (int)codigo == ERROR) {
        return INVAL_ID;
    }
    // TODO: quizas ver si no es exito -> tirar excepcion

    return obtenerId();
}


//METODOS DEL CLIENTE
#ifdef CLIENT
bool Protocolo::pedirInformacion(tipoInfo infoAPedir) {
    int8_t pedidoAEnviar[2] = {PEDIDO, (int8_t)infoAPedir};

    bool was_closed = false;
    socket.sendall(pedidoAEnviar, sizeof(pedidoAEnviar), &was_closed);
    return !was_closed;
}


std::vector<id> Protocolo::obtenerMapas() {
    int8_t codigo = obtenerCodigo();
    if ((int)codigo != MAPAS) {
        std::vector<id> error;
        return error;
    }

    bool was_closed = false;
    int16_t cant;
    std::vector<RepresentacionMapa> error;
    socket.recvall(&cant, sizeof(cant), &was_closed);
    if (was_closed) {
        return error;
    }
    cant = ntohs(cant);

    std::vector<RepresentacionMapa> mapas(cant, 0);
    for (int i = 0; i < (int)cant; i++) {
        uint16_t sz;
        skt.recvall(&sz, sizeof(sz), &was_closed);
        if (was_closed) {
            return error;
        }

        int tamanio = (int)ntohs(sz[0]);
        std::vector<char> mapa(tamanio, 0x00);
        skt.recvall(mapa.data(), tamanio, &was_closed);
        if (was_closed) {
            return error;
        }
        RepresentacionMapa repMapa;
        repMapa.id = (id)i;
        nombre = std::string(msg.begin(), msg.end());
        repMapa.nombre = nombre;
        
        mapas[i] = repMapa;
    }
    return mapas;
}


std::vector<id> Protocolo::obtenerPartidas() {
    int8_t codigo = obtenerCodigo();
    if (codigo != PARTIDAS) {
        std::vector<id> error;
        return error;
    }

    return obtenerVector();
}


id Protocolo::crearPartida(id mapaSeleccionado) {
    bool was_closed = enviarCodigo(CREAR);
    if (was_closed) {
        return INVAL_ID;
    }

    was_closed = enviarId(mapaSeleccionado);
    if (was_closed) {
        return INVAL_ID;
    }

    return verificarConexion();
}


bool Protocolo::unirseAPartida(id id) {
    bool was_closed = enviarCodigo(UNIRSE);
    if (was_closed) {
        return false;
    }

    was_closed = enviarId(id);
    if (was_closed) {
        return false;
    }

    return verificarConexion();
}


bool Protocolo::moverGusano(id gusano, Direccion direccion) {
    bool was_closed = enviarCodigo(MOV);
    if (was_closed) {
        return false;
    }

    was_closed = enviarId(gusano);
    if (was_closed) {
        return false;
    }

    int8_t dir = direccion;
    socket.sendall(&dir, sizeof(dir), &was_closed);
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
    if (was_closed) {
        return INVAL_TIPO;
    }
    return (tipoInfo)pedidoARecibir[1];
}


bool Protocolo::enviarMapas(std::vector<std::string> mapasDisponibles) {
    int cantMapas = mapasDisponibles.size();

    bool was_closed = enviarCodigo(MAPAS);
    if (was_closed) {
        return false;
    }

    was_closed = enviarCantidad(cantMapas);
    if (was_closed) {
        return false;
    }

    for (auto &&mapa : mapasDisponibles){
        int16_t sz = mapa.size();
        int16_t tam = htons(sz);
        socket.sendall(&tam, sizeof(tam), &was_closed);
        if (was_closed) {
            return false;
        }
        socket.sendall(mapa.c_str(), (int)sz, &was_closed);
        if (was_closed) {
            return false;
        }
    }
    
    return true;
}

bool Protocolo::enviarPartidas(std::vector<RepresentacionPartida> partidasDisponibles) {
    int cantPartidas = partidasDisponibles.size();

    std::vector<id> paraEnviar;
    for (auto &&partida : partidasDisponibles) {
        id idMapa = htonl(partida.ID);
        paraEnviar.push_back(idMapa);
    }

    /* TODO: mover estos sendall a otro metodo
            hacer que este metodo devuelva 1 string/algo con toda la info para hacer
                mas facil el testeo
    */
    bool was_closed = enviarCodigo(PARTIDAS);
    if (was_closed) {
        return false;
    }

    was_closed = enviarCantidad(cantPartidas);
    if (was_closed) {
        return false;
    }

    socket.sendall(paraEnviar.data(), sizeof(int32_t)*cantPartidas, &was_closed);
    return !was_closed;    
}


id Protocolo::obtenerMapaDeseado() {
    int8_t codigo = obtenerCodigo();
    if ((int)codigo != MAPAS) {
        return INVAL_ID;
    }
    return obtenerId();
}


id Protocolo::obtenerPartidaDeseada() {
    int8_t codigo = obtenerCodigo();
    if ((int)codigo != PARTIDAS) {
        return INVAL_ID;
    }
    return obtenerId();
}


bool Protocolo::enviarConfirmacion(id idPartida) {
    bool was_closed = enviarCodigo(EXITO);
    if (was_closed) {
        return false;
    }

    return enviarId(idPartida);
}


bool Protocolo::enviarError() {
    return enviarCodigo(ERROR);
}


Direccion Protocolo::obtenerAccion() {
    bool was_closed = obtenerCodigo();
    if (was_closed) {
        return INVAL_DIR;
    }

    id idGusano = obtenerId();
    if (idGusano == INVAL_ID) {
        return INVAL_DIR;
    }

    int8_t dir;
    socket.recvall(&dir, sizeof(dir), &was_closed);
    return (was_closed) ? INVAL_DIR : (Direccion)dir;
}
//Endif de la macro de SERVER
#endif
