#include "protocolo.h"
#include <cstdint>
#include <vector>

#include <iostream>

Protocolo::Protocolo(Socket &&socket)
    : socket(std::move(socket))
      {
	this->maxYEnMapa = 100;
      };


void Protocolo::cerrarConexion(int forma) {
    this->socket.shutdown(forma);
    this->socket.close();
}


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
    if (was_closed) {
        return (int8_t)-1;
    }
    return codigo;
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
    int8_t codigo = obtenerCodigo();
    if (codigo == -1 || codigo == ERROR) {
        return INVAL_ID;
    }
    // TODO: quizas ver si no es exito -> tirar excepcion

    return obtenerId();
}


// Fuente: https://www.geeksforgeeks.org/rounding-floating-point-number-two-decimal-places-c-c/
float Protocolo::toFloat(int valor) {
    return (float)valor / 10000;
}


int Protocolo::toInt(float valor) {
    // redondeo y uso 2 decimales
    return (int)(valor * 10000 + .5);
}


//METODOS DEL CLIENTE
#ifdef CLIENT
bool Protocolo::pedirInformacion(tipoInfo infoAPedir) {
    int8_t pedidoAEnviar[2] = {PEDIDO, (int8_t)infoAPedir};

    bool was_closed = false;
    socket.sendall(pedidoAEnviar, sizeof(pedidoAEnviar), &was_closed);
    return !was_closed;
}


std::vector<RepresentacionMapa> Protocolo::obtenerMapas() {
    int8_t codigo = obtenerCodigo();
    std::vector<RepresentacionMapa> error;
    if (codigo != MAPAS) {
        return error;
    }

    bool was_closed = false;
    int16_t cant;
    socket.recvall(&cant, sizeof(cant), &was_closed);
    if (was_closed) {
        return error;
    }
    cant = ntohs(cant);

    std::vector<RepresentacionMapa> mapas(cant);
    for (int i = 0; i < (int)cant; i++) {
        uint16_t sz;
        socket.recvall(&sz, sizeof(sz), &was_closed);
        if (was_closed) {
            return error;
        }

        int tamanio = (int)ntohs(sz);
        std::vector<char> mapa(tamanio, 0x00);
        socket.recvall(mapa.data(), tamanio, &was_closed);
        if (was_closed) {
            return error;
        }
        RepresentacionMapa repMapa;
        repMapa.ID = (id)i;
        std::string nombre = std::string(mapa.begin(), mapa.end());
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
    bool is_open = enviarCodigo(CREAR);
    if (!is_open) {
        return INVAL_ID;
    }

    is_open = enviarId(mapaSeleccionado);
    if (!is_open) {
        return INVAL_ID;
    }

    return verificarConexion();
}


bool Protocolo::unirseAPartida(id idPartida) {
    bool is_open = enviarCodigo(UNIRSE);
    if (!is_open) {
        return false;
    }

    is_open = enviarId(idPartida);
    if (!is_open) {
        return false;
    }

    id idPartidaUnida = verificarConexion();
    return !(idPartidaUnida == INVAL_ID);
}


bool Protocolo::moverGusano(Direccion direccion) {
    bool is_open = enviarCodigo(MOV);
    if (!is_open) {
        return false;
    }

    int8_t dir = direccion;
    bool was_closed = false;
    socket.sendall(&dir, sizeof(dir), &was_closed);
    if (was_closed) {
        return false;
    }
    return !was_closed;
}


bool Protocolo::equiparArma(ArmaProtocolo arma) {
    bool is_open = enviarCodigo(EQUIPAR);
    if (!is_open) {
        return false;
    }

    int8_t armaAEnviar = arma;
    bool was_closed = false;
    socket.sendall(&armaAEnviar, sizeof(armaAEnviar), &was_closed);
    return !was_closed;
}


bool Protocolo::atacar() {
    return enviarCodigo(ATACAR);
}


bool Protocolo::configurarAngulo(float angulo) {
    bool is_open = enviarCodigo(CALIBRAR);
    if (!is_open) {
        return false;
    }

    int8_t tipoConfig = ANGULO; 
    bool was_closed = false;
    socket.sendall(&tipoConfig, sizeof(tipoConfig), &was_closed);
    if (was_closed) {
        return false;
    }

    int32_t valor = htonl(toInt(angulo));
    socket.sendall(&valor, sizeof(valor), &was_closed);
    return !was_closed;
}


bool Protocolo::configurarPotencia(float potencia) {
    bool is_open = enviarCodigo(CALIBRAR);
    if (!is_open) {
        return false;
    }

    int8_t tipoConfig = POTENCIA; 
    bool was_closed = false;
    socket.sendall(&tipoConfig, sizeof(tipoConfig), &was_closed);
    if (was_closed) {
        return false;
    }

    int32_t valor = htonl(toInt(potencia));
    socket.sendall(&valor, sizeof(valor), &was_closed);
    return !was_closed;
}


bool Protocolo::configurarCuentaRegresiva(int valor) {
    bool is_open = enviarCodigo(CALIBRAR);
    if (!is_open) {
        return false;
    }

    int8_t tipoConfig = CUENTA_REGRESIVA; 
    bool was_closed = false;
    socket.sendall(&tipoConfig, sizeof(tipoConfig), &was_closed);
    if (was_closed) {
        return false;
    }

    int32_t valorAEnviar = htonl(valor);
    socket.sendall(&valorAEnviar, sizeof(valorAEnviar), &was_closed);
    return !was_closed;
}


bool Protocolo::configurarCoordenadas(std::pair<coordX,coordY> coordenadas) {
    bool is_open = enviarCodigo(CALIBRAR);
    if (!is_open) {
        return false;
    }

    int8_t tipoConfig = COORDENADAS; 
    bool was_closed = false;
    socket.sendall(&tipoConfig, sizeof(tipoConfig), &was_closed);
    if (was_closed) {
        return false;
    }

    std::vector<int32_t> posAEnviar;
    posAEnviar.push_back(htonl((int32_t)toInt(coordenadas.enX)));
    posAEnviar.push_back(htonl((int32_t)toInt(coordenadas.enY)));

    socket.sendall(posAEnviar.data(), sizeof(int32_t)*posAEnviar.size(), &was_closed);
    return !was_closed;
}


std::shared_ptr<EstadoDelJuego> Protocolo::obtenerEstadoDelJuego() {
    int8_t codigo = obtenerCodigo();
    std::shared_ptr<EstadoDelJuego> estado(new EstadoDelJuego);
    if (codigo == -1 || codigo != ESTADO) {
        return estado;
    }

    int16_t cantJugadores;
    bool was_closed = false;
    socket.recvall(&cantJugadores, sizeof(cantJugadores), &was_closed);
    if (was_closed) {
        return estado;
    }
    cantJugadores = ntohs(cantJugadores);

    std::map<idJugador,std::vector<RepresentacionGusano>> gusanos;
    for (int16_t i = 0; i < cantJugadores; i++) {
        id idJugador = obtenerId();
        if (idJugador == INVAL_ID) {
            return estado;
        }

        int16_t cantGusanos;
        was_closed = false;
        socket.recvall(&cantGusanos, sizeof(cantGusanos), &was_closed);
        if (was_closed) {
            return estado;
        }
        cantGusanos = ntohs(cantGusanos);

        std::vector<RepresentacionGusano> listaGusanos(cantGusanos);
        for (int16_t j = 0; j < cantGusanos; j++) {
            id idGusano = obtenerId();
            if (idJugador == INVAL_ID) {
                return estado;
            }

            uint32_t vida;
            socket.recvall(&vida, sizeof(vida), &was_closed);
            if (was_closed) {
                return estado;
            }
            vida = ntohl(vida);

            std::vector<int32_t> posicion(2,0);
            socket.recvall(posicion.data(), sizeof(int32_t)*2, &was_closed);
            if (was_closed) {
                return estado;
            }

            std::pair<coordX, coordY> posicionRecibida;
            posicionRecibida.enX = toFloat(ntohl(posicion[0]));
            posicionRecibida.enY = toFloat(ntohl(posicion[1]));
            posicionRecibida.enY = maxYEnMapa - posicionRecibida.enY;

            int8_t estadoGusano;
            bool was_closed = false;
            socket.recvall(&estadoGusano, sizeof(estadoGusano), &was_closed);
            if (was_closed) {
                return estado;
            }

            int8_t dir;
            socket.recvall(&dir, sizeof(dir), &was_closed);
            if (was_closed) {
                return estado;
            }

            int8_t mira;
            socket.recvall(&mira, sizeof(mira), &was_closed);
            if (was_closed) {
                return estado;
            }
            int8_t potenciaVariable;
            socket.recvall(&potenciaVariable, sizeof(potenciaVariable), &was_closed);
            if (was_closed) {
                return estado;
            }
            int8_t tieneCuentaRegresiva;
            socket.recvall(&tieneCuentaRegresiva, sizeof(tieneCuentaRegresiva), &was_closed);
            if (was_closed) {
                return estado;
            }
            int32_t municiones;
            socket.recvall(&municiones, sizeof(municiones), &was_closed);
            if (was_closed) {
                return estado;
            }
            municiones = ntohl(municiones);

            int32_t fragmentos;
            socket.recvall(&fragmentos, sizeof(fragmentos), &was_closed);
            if (was_closed) {
                return estado;
            }
            fragmentos = ntohl(fragmentos);

            int32_t danioEpicentro;
            socket.recvall(&danioEpicentro, sizeof(danioEpicentro), &was_closed);
            if (was_closed) {
                return estado;
            }
            danioEpicentro = ntohl(danioEpicentro);

            int32_t danioRadio;
            socket.recvall(&danioRadio, sizeof(danioRadio), &was_closed);
            if (was_closed) {
                return estado;
            }
            danioRadio = ntohl(danioRadio);

            int32_t danioFragEpicentro;
            int32_t danioFragRadio;
            if (fragmentos > 0) {
                socket.recvall(&danioFragEpicentro, sizeof(danioFragEpicentro), &was_closed);
                if (was_closed) {
                    return estado;
                }
                danioFragEpicentro = ntohl(danioFragEpicentro);

                socket.recvall(&danioFragRadio, sizeof(danioFragRadio), &was_closed);
                if (was_closed) {
                    return estado;
                }
                danioFragRadio = ntohl(danioFragRadio);
            }
            int32_t angulo;
            socket.recvall(&angulo, sizeof(angulo), &was_closed);
            if (was_closed) {
                return estado;
            }
            angulo = toFloat(ntohl(angulo));

            int32_t potencia;
            socket.recvall(&potencia, sizeof(potencia), &was_closed);
            if (was_closed) {
                return estado;
            }
            potencia = toFloat(ntohl(potencia));

            int32_t cuentaRegresiva;
            socket.recvall(&cuentaRegresiva, sizeof(cuentaRegresiva), &was_closed);
            if (was_closed) {
                return estado;
            }
            cuentaRegresiva = ntohl(cuentaRegresiva);


            int8_t arma;
            socket.recvall(&arma, sizeof(arma), &was_closed);
            if (was_closed) {
                return estado;
            }


            RepresentacionArma armaEquipada;
            armaEquipada.tieneMira = mira;
            armaEquipada.tienePotenciaVariable = potenciaVariable;
            armaEquipada.tieneCuentaRegresiva = tieneCuentaRegresiva;
            armaEquipada.municiones = municiones;
            armaEquipada.fragmentos = fragmentos;

            armaEquipada.danio.epicentro = danioEpicentro;
            armaEquipada.danio.radio = danioRadio;

            if (fragmentos > 0) {
                armaEquipada.danioFragmento.epicentro = danioFragEpicentro;
                armaEquipada.danioFragmento.radio = danioFragRadio;
            }
            armaEquipada.anguloRad = angulo;
            armaEquipada.potencia = potencia;
            armaEquipada.cuentaRegresiva = cuentaRegresiva;
            armaEquipada.arma = (ArmaProtocolo)arma;

            RepresentacionGusano gusanoActual;
            gusanoActual.vida = vida;
            gusanoActual.idGusano = idGusano;
            gusanoActual.estado = (EstadoGusano)estadoGusano;
            gusanoActual.dir = (DireccionGusano)dir;
            gusanoActual.posicion = posicionRecibida;
            gusanoActual.armaEquipada = armaEquipada;

            listaGusanos[j] = gusanoActual;
        }   

        gusanos.insert({idJugador, listaGusanos});
        
    }
    
    estado->gusanos = gusanos;
    return estado;
}

void Protocolo::setMaxY(int y) {
    this->maxYEnMapa = y;
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

    bool is_open = enviarCodigo(MAPAS);
    if (!is_open) {
        return false;
    }

    is_open = enviarCantidad(cantMapas);
    if (!is_open) {
        return false;
    }

    for (auto &&mapa : mapasDisponibles){
        bool was_closed = false;
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

    bool is_open = enviarCodigo(PARTIDAS);
    if (!is_open) {
        return false;
    }

    is_open = enviarCantidad(cantPartidas);
    if (!is_open) {
        return false;
    }

    bool was_closed = false;
    socket.sendall(paraEnviar.data(), sizeof(int32_t)*cantPartidas, &was_closed);
    return !was_closed;    
}


id Protocolo::obtenerMapaDeseado() {
    int8_t codigo = obtenerCodigo();
    if (codigo != CREAR) {
        return INVAL_ID;
    }
    return obtenerId();
}


id Protocolo::obtenerPartidaDeseada() {
    int8_t codigo = obtenerCodigo();
    if (codigo != UNIRSE) {
        return INVAL_ID;
    }
    return obtenerId();
}


bool Protocolo::enviarConfirmacion(id idPartida) {
    bool is_open = enviarCodigo(EXITO);
    if (!is_open) {
        return false;
    }

    return enviarId(idPartida);
}


bool Protocolo::enviarError() {
    return enviarCodigo(ERROR);
}


Accion Protocolo::obtenerAccion() {
    int8_t codigo = obtenerCodigo();
    bool was_closed = false;
    Accion accion;
    if (codigo != MOV && codigo != ATACAR && 
        codigo != EQUIPAR && codigo != CALIBRAR) {
        return accion;
    }

    if (codigo == EQUIPAR) {
        int8_t arma;
        socket.recvall(&arma, sizeof(arma), &was_closed);
        if (was_closed) {
            return accion;
        }

        accion.accion = EQUIPARSE;
        accion.armaAEquipar = (ArmaProtocolo)arma;
        return accion;
    }

    if (codigo == ATACAR) {
        accion.accion = ATAQUE;
        return accion;
    }

    if (codigo == CALIBRAR) {
        int8_t valorAConfigurar;
        socket.recvall(&valorAConfigurar, sizeof(valorAConfigurar), &was_closed);
        if (was_closed) {
            return accion;
        }
        Configuracion config;
        config.caracteristica = (ValorAConfigurar)valorAConfigurar;
        if (config.caracteristica == COORDENADAS) {
            std::vector<int32_t> posicion(2,0);
            socket.recvall(posicion.data(), sizeof(int32_t)*2, &was_closed);
            if (was_closed) {
                return accion;
            }

            std::pair<coordX, coordY> posicionRecibida;
            posicionRecibida.enX = toFloat(ntohl(posicion[0]));
            posicionRecibida.enY = toFloat(ntohl(posicion[1]));
            config.coordenadas = posicionRecibida;
        } else {
            int32_t valor;
            socket.recvall(&valor, sizeof(valor), &was_closed);
            if (was_closed) {
                return accion;
            }
            valor = ntohl(valor);
            
            if (config.caracteristica == POTENCIA) {
                config.potencia = toFloat(valor);
            } else if (config.caracteristica == ANGULO) {
                config.angulo = toFloat(valor);
            } else {
                config.cuentaRegresiva = valor;
            }
        }

        accion.accion = PREPARAR;
        accion.configARealizar = config;
        return accion;
        
    }
    // caso de moverse
    
    int8_t dir;
    socket.recvall(&dir, sizeof(dir), &was_closed);
    if (was_closed) {
        return accion;
    }

    // TODO: ampliar a los otros tipos de accion
    accion.accion = MOVERSE;
    accion.dir = (Direccion)dir;
    return accion;
}


// se manda ESCENARIO+cantJugadores+[idJugador+cantGusanos+[id+vida+posX+posY+dir+arma]]
bool Protocolo::enviarEstadoDelJuego(std::shared_ptr<EstadoDelJuego> estado) {
    bool is_open = enviarCodigo(ESTADO);
    if (!is_open) {
        return false;
    }
    int cant = estado->gusanos.size();
    //envio cantJugadores
    is_open = enviarCantidad(cant);
    if (!is_open) {
        return false;
    }

    for (auto const& [idJugador, listaGusanos] : estado->gusanos) {
        // envio idJugador
        is_open = enviarId(idJugador);
        if (!is_open) {
            return false;
        }

        // envio cant de gusanos
        int cantGusanos = listaGusanos.size();
        is_open = enviarCantidad(cantGusanos);
        if (!is_open) {
            return false;
        }

        for (int32_t i = 0; i < cantGusanos; i++) {
            RepresentacionGusano gusano = listaGusanos[i];
            // envio el id del gusano
            is_open = enviarId(gusano.idGusano);
            if (!is_open) {
                return false;
            }
            
            // envio vida del gusano
            bool was_closed = false;
            uint32_t vida = htonl((uint32_t)gusano.vida);
            socket.sendall(&vida, sizeof(vida), &was_closed);
            if (was_closed) {
                return false;
            }
            
            // envio posicion
            std::vector<int32_t> posAEnviar;
            posAEnviar.push_back(htonl((int32_t)toInt(gusano.posicion.enX)));
            posAEnviar.push_back(htonl((int32_t)toInt(gusano.posicion.enY)));

            socket.sendall(posAEnviar.data(), sizeof(int32_t)*posAEnviar.size(), &was_closed);
            if (was_closed) {
                return false;
            }
            // enviar estado del gusano
            int8_t estadoGusano = gusano.estado;
            socket.sendall(&estadoGusano, sizeof(estadoGusano), &was_closed);
            if (was_closed) {
                return false;
            }


            // envio direccion
            int8_t dir = gusano.dir;
            socket.sendall(&dir, sizeof(dir), &was_closed);
            if (was_closed) {
                return false;
            }

            // envio datos de arma
            RepresentacionArma armaEquipada = gusano.armaEquipada;
            int8_t mira = armaEquipada.tieneMira;
            socket.sendall(&mira, sizeof(mira), &was_closed);
            if (was_closed) {
                return false;
            }
            int8_t potenciaVariable = armaEquipada.tienePotenciaVariable;
            socket.sendall(&potenciaVariable, sizeof(potenciaVariable), &was_closed);
            if (was_closed) {
                return false;
            }
            int8_t tieneCuentaRegresiva = armaEquipada.tieneCuentaRegresiva;
            socket.sendall(&tieneCuentaRegresiva, sizeof(tieneCuentaRegresiva), &was_closed);
            if (was_closed) {
                return false;
            }
            int32_t municiones = htonl(armaEquipada.municiones);
            socket.sendall(&municiones, sizeof(municiones), &was_closed);
            if (was_closed) {
                return false;
            }
            int32_t fragmentos = htonl(armaEquipada.fragmentos);
            socket.sendall(&fragmentos, sizeof(fragmentos), &was_closed);
            if (was_closed) {
                return false;
            }
            int32_t danioEpicentro = htonl(armaEquipada.danio.epicentro);
            socket.sendall(&danioEpicentro, sizeof(danioEpicentro), &was_closed);
            if (was_closed) {
                return false;
            }
            int32_t danioRadio = htonl(armaEquipada.danio.radio);
            socket.sendall(&danioRadio, sizeof(danioRadio), &was_closed);
            if (was_closed) {
                return false;
            }

            if (fragmentos > 0) {
                int32_t danioFragEpicentro = htonl(armaEquipada.danioFragmento.epicentro);
                socket.sendall(&danioFragEpicentro, sizeof(danioFragEpicentro), &was_closed);
                if (was_closed) {
                    return false;
                }
                int32_t danioFragRadio = htonl(armaEquipada.danioFragmento.radio);
                socket.sendall(&danioFragRadio, sizeof(danioFragRadio), &was_closed);
                if (was_closed) {
                    return false;
                }
            }

            int32_t angulo = htonl(toInt(armaEquipada.anguloRad));
            socket.sendall(&angulo, sizeof(angulo), &was_closed);
            if (was_closed) {
                return false;
            }
            int32_t potencia = htonl(toInt(armaEquipada.potencia));
            socket.sendall(&potencia, sizeof(potencia), &was_closed);
            if (was_closed) {
                return false;
            }

            int32_t cuentaRegresiva = htonl(armaEquipada.cuentaRegresiva);
            socket.sendall(&cuentaRegresiva, sizeof(cuentaRegresiva), &was_closed);
            if (was_closed) {
                return false;
            }

            int8_t arma = armaEquipada.arma;
            socket.sendall(&arma, sizeof(arma), &was_closed);
            if (was_closed) {
                return false;
            }
        }  
    }
    

    return true;
}

//Endif de la macro de SERVER
#endif
