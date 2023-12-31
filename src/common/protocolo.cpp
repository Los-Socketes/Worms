#include "protocolo.h"
#include <cstdint>
#include <vector>

#include <iostream>

Protocolo::Protocolo(Socket &&socket) : socket(std::move(socket)) {};


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


InformacionInicial Protocolo::verificarConexion() {
    InformacionInicial info;
    info.jugador = INVAL_ID;
    int8_t codigo = obtenerCodigo();
    if (codigo == -1 || codigo == ERROR) {
        return info;
    }
    idJugador idEnviada = obtenerId();
    if (idEnviada == INVAL_ID) {
        return info;
    }

    id idPartida = obtenerId();
    if (idPartida == INVAL_ID) {
        return info;
    }

    int16_t cantVigas;
    bool was_closed = false;
    socket.recvall(&cantVigas, sizeof(cantVigas), &was_closed);
    if (was_closed) {
        return info;
    }
    cantVigas = ntohs(cantVigas);
    std::vector<RepresentacionViga> vigas;
    for (int16_t i = 0; i < cantVigas; i++) {
        int32_t angulo;
        socket.recvall(&angulo, sizeof(angulo), &was_closed);
        if (was_closed) {
            return info;
        }
        float anguloRecibido = toFloat(ntohl(angulo));

        int32_t largo;
        socket.recvall(&largo, sizeof(largo), &was_closed);
        if (was_closed) {
            return info;
        }
        largo = ntohl(largo);

        std::vector<int32_t> posicion(2,0);
        socket.recvall(posicion.data(), sizeof(int32_t)*2, &was_closed);
        if (was_closed) {
            return info;
        }

        std::pair<coordX, coordY> posicionRecibida;
        posicionRecibida.enX = toFloat(ntohl(posicion[0]));
        posicionRecibida.enY = toFloat(ntohl(posicion[1]));

        RepresentacionViga vigaActual;
        vigaActual.angulo = anguloRecibido;
        vigaActual.longitud = largo;
        vigaActual.posicionInicial = posicionRecibida;
        vigas.push_back(vigaActual);
    } 

    std::vector<int32_t> dimensiones(2,0);
    socket.recvall(dimensiones.data(), sizeof(int32_t)*2, &was_closed);
    if (was_closed) {
        return info;
    }

    std::pair<coordX, coordY> dimensionesRecibidas;
    dimensionesRecibidas.enX = toFloat(ntohl(dimensiones[0]));
    dimensionesRecibidas.enY = toFloat(ntohl(dimensiones[1]));

    info.jugador = idEnviada;
    info.vigas = vigas;
    info.dimensiones = dimensionesRecibidas;
    info.idPartida = idPartida;
    return info;

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


InformacionInicial Protocolo::crearPartida(id mapaSeleccionado) {
    InformacionInicial error;
    error.jugador = -1;
    bool is_open = enviarCodigo(CREAR);
    if (!is_open) {
        return error;
    }

    is_open = enviarId(mapaSeleccionado);
    if (!is_open) {
        return error;
    }

    return verificarConexion();
}


InformacionInicial Protocolo::unirseAPartida(id idPartida) {
    InformacionInicial error;
    bool is_open = enviarCodigo(UNIRSE);
    if (!is_open) {
        return error;
    }

    is_open = enviarId(idPartida);
    if (!is_open) {
        return error;
    }

    return verificarConexion();
}


bool Protocolo::iniciarPartida() {
    return enviarCodigo(EMPEZAR);
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


bool Protocolo::enviarCheat(TipoCheat cheat) {
    bool is_open = enviarCodigo(CHEATEAR);
    if (!is_open) {
        return false;
    }
    int8_t cheatAEnviar = cheat;
    bool was_closed = false;
    socket.sendall(&cheatAEnviar, sizeof(cheatAEnviar), &was_closed);
    return !was_closed;
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

std::map<idJugador, SituacionJugador> Protocolo::obtenerSituacion(int cantJugadores) {
    std::map<idJugador, SituacionJugador> situacionJugadores;
    std::map<idJugador, SituacionJugador> error;
    bool was_closed = false;
    for (int16_t i = 0; i < cantJugadores; i++) {
        id idJugador = obtenerId();
        if (idJugador == INVAL_ID) {
            return error;
        }
        int8_t situacion;
        socket.recvall(&situacion, sizeof(situacion), &was_closed);
        if (was_closed) {
            return error;
        }
        situacionJugadores.insert({idJugador, (SituacionJugador)situacion});
    }
    return situacionJugadores;
}

std::map<idJugador, std::map<id, RepresentacionGusano>> Protocolo::obtenerGusanos(int cantJugadores) {
    std::map<idJugador, std::map<id, RepresentacionGusano>> gusanos;
    std::map<idJugador, std::map<id, RepresentacionGusano>> error;
    bool was_closed = false;
    for (int16_t i = 0; i < cantJugadores; i++) {
        id idJugador = obtenerId();
        if (idJugador == INVAL_ID) {
            return error;
        }

        int16_t cantGusanos;
        was_closed = false;
        socket.recvall(&cantGusanos, sizeof(cantGusanos), &was_closed);
        if (was_closed) {
            return error;
        }
        cantGusanos = ntohs(cantGusanos);

        std::map<id, RepresentacionGusano> mapaGusanos;
        // std::vector<RepresentacionGusano> listaGusanos(cantGusanos);
        for (int16_t j = 0; j < cantGusanos; j++) {
            id idGusano = obtenerId();
            if (idJugador == INVAL_ID) {
                return error;
            }

            uint32_t vida;
            socket.recvall(&vida, sizeof(vida), &was_closed);
            if (was_closed) {
                return error;
            }
            vida = ntohl(vida);

            std::vector<int32_t> posicion(2,0);
            socket.recvall(posicion.data(), sizeof(int32_t)*2, &was_closed);
            if (was_closed) {
                return error;
            }

            std::pair<coordX, coordY> posicionRecibida;
            posicionRecibida.enX = toFloat(ntohl(posicion[0]));
            posicionRecibida.enY = toFloat(ntohl(posicion[1]));

            int8_t estadoGusano;
            socket.recvall(&estadoGusano, sizeof(estadoGusano), &was_closed);
            if (was_closed) {
                return error;
            }

            int8_t dir;
            socket.recvall(&dir, sizeof(dir), &was_closed);
            if (was_closed) {
                return error;
            }

            // caracteristicas de arma
            int8_t mira;
            socket.recvall(&mira, sizeof(mira), &was_closed);
            if (was_closed) {
                return error;
            }
            int8_t potenciaVariable;
            socket.recvall(&potenciaVariable, sizeof(potenciaVariable), &was_closed);
            if (was_closed) {
                return error;
            }
            int8_t tieneCuentaRegresiva;
            socket.recvall(&tieneCuentaRegresiva, sizeof(tieneCuentaRegresiva), &was_closed);
            if (was_closed) {
                return error;
            }
            int32_t municiones;
            socket.recvall(&municiones, sizeof(municiones), &was_closed);
            if (was_closed) {
                return error;
            }
            municiones = ntohl(municiones);

            int32_t fragmentos;
            socket.recvall(&fragmentos, sizeof(fragmentos), &was_closed);
            if (was_closed) {
                return error;
            }
            fragmentos = ntohl(fragmentos);

            int32_t danioEpicentro;
            socket.recvall(&danioEpicentro, sizeof(danioEpicentro), &was_closed);
            if (was_closed) {
                return error;
            }
            danioEpicentro = ntohl(danioEpicentro);

            int32_t danioRadio;
            socket.recvall(&danioRadio, sizeof(danioRadio), &was_closed);
            if (was_closed) {
                return error;
            }
            danioRadio = ntohl(danioRadio);

            int32_t danioFragEpicentro;
            int32_t danioFragRadio;
            if (fragmentos > 0) {
                socket.recvall(&danioFragEpicentro, sizeof(danioFragEpicentro), &was_closed);
                if (was_closed) {
                    return error;
                }
                danioFragEpicentro = ntohl(danioFragEpicentro);

                socket.recvall(&danioFragRadio, sizeof(danioFragRadio), &was_closed);
                if (was_closed) {
                    return error;
                }
                danioFragRadio = ntohl(danioFragRadio);
            }
            int32_t angulo;
            socket.recvall(&angulo, sizeof(angulo), &was_closed);
            if (was_closed) {
                return error;
            }
            float anguloRecibido = toFloat(ntohl(angulo));

            int32_t potencia;
            socket.recvall(&potencia, sizeof(potencia), &was_closed);
            if (was_closed) {
                return error;
            }
            potencia = toFloat(ntohl(potencia));

            int32_t cuentaRegresiva;
            socket.recvall(&cuentaRegresiva, sizeof(cuentaRegresiva), &was_closed);
            if (was_closed) {
                return error;
            }
            cuentaRegresiva = ntohl(cuentaRegresiva);


            int8_t arma;
            socket.recvall(&arma, sizeof(arma), &was_closed);
            if (was_closed) {
                return error;
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
            armaEquipada.anguloRad = anguloRecibido;
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

            mapaGusanos.insert({idGusano, gusanoActual});
        }   

        gusanos.insert({idJugador, mapaGusanos});
        
    }
    return gusanos;
}

std::vector<RepresentacionProyectil> Protocolo::obtenerProyectiles() {
    std::vector<RepresentacionProyectil> error;
    bool was_closed = false;
    int16_t cantProyectiles;
    socket.recvall(&cantProyectiles, sizeof(cantProyectiles), &was_closed);
    if (was_closed) {
        return error;
    }
    cantProyectiles = ntohs(cantProyectiles);

    std::vector<RepresentacionProyectil> proyectiles;
    for (int i = 0; i < cantProyectiles; i++) {
        idProyectil proyectilId = obtenerId();
        if (proyectilId == INVAL_ID) {
            return error;
        }
        int8_t tipoProyectil;
        socket.recvall(&tipoProyectil, sizeof(tipoProyectil), &was_closed);
        if (was_closed) {
            return error;
        }

        int8_t esFragmento;
        socket.recvall(&esFragmento, sizeof(esFragmento), &was_closed);
        if (was_closed) {
            return error;
        }

        std::vector<int32_t> posicionProyectiles(2,0);
        socket.recvall(posicionProyectiles.data(), sizeof(int32_t)*2, &was_closed);
        if (was_closed) {
            return error;
        }

        std::pair<coordX, coordY> posicionRecibidaProy;
        posicionRecibidaProy.enX = toFloat(ntohl(posicionProyectiles[0]));
        posicionRecibidaProy.enY = toFloat(ntohl(posicionProyectiles[1]));

        int32_t anguloProyectos;
        socket.recvall(&anguloProyectos, sizeof(anguloProyectos), &was_closed);
        if (was_closed) {
            return error;
        }
        float anguloRecibidoProy = toFloat(ntohl(anguloProyectos));

        int32_t cuentaRegresivaProy;
        socket.recvall(&cuentaRegresivaProy, sizeof(cuentaRegresivaProy), &was_closed);
        if (was_closed) {
            return error;
        }
        cuentaRegresivaProy = ntohl(cuentaRegresivaProy);

        int8_t exploto;
        socket.recvall(&exploto, sizeof(exploto), &was_closed);
        if (was_closed) {
            return error;
        }
        RepresentacionProyectil proyectil;
        proyectil.id = proyectilId;
        proyectil.proyectil = (ArmaProtocolo)tipoProyectil;
        proyectil.esFragmento = esFragmento;
        proyectil.posicion = posicionRecibidaProy;
        proyectil.angulo = anguloRecibidoProy;
        proyectil.cuentaRegresiva = cuentaRegresivaProy;
        proyectil.exploto = exploto;

        proyectiles.push_back(proyectil);
    }
    return proyectiles;
}

std::vector<RepresentacionProvisiones> Protocolo::obtenerProvisiones() {
    std::vector<RepresentacionProvisiones> error;
    bool was_closed = false;
    int16_t cantProvisiones;
    socket.recvall(&cantProvisiones, sizeof(cantProvisiones), &was_closed);
    if (was_closed) {
        return error;
    }
    cantProvisiones = ntohs(cantProvisiones);

    std::vector<RepresentacionProvisiones> provisiones;
    for (int i = 0; i < cantProvisiones; i++) {
        int id = obtenerId();
        if (id == INVAL_ID) {
            return error;
        }

        std::vector<int32_t> posicionProvisiones(2,0);
        socket.recvall(posicionProvisiones.data(), sizeof(int32_t)*2, &was_closed);
        if (was_closed) {
            return error;
        }

        std::pair<coordX, coordY> posicionRecibidaProv;
        posicionRecibidaProv.enX = toFloat(ntohl(posicionProvisiones[0]));
        posicionRecibidaProv.enY = toFloat(ntohl(posicionProvisiones[1]));

        int8_t estaEnElAire;
        socket.recvall(&estaEnElAire, sizeof(estaEnElAire), &was_closed);
        if (was_closed) {
            return error;
        }

        int8_t tipo;
        socket.recvall(&tipo, sizeof(tipo), &was_closed);
        if (was_closed) {
            return error;
        }

        int8_t armaMunicion;
        socket.recvall(&armaMunicion, sizeof(armaMunicion), &was_closed);
        if (was_closed) {
            return error;
        }

        int8_t fueAgarrada;
        socket.recvall(&fueAgarrada, sizeof(fueAgarrada), &was_closed);
        if (was_closed) {
            return error;
        }

        int8_t exploto;
        socket.recvall(&exploto, sizeof(exploto), &was_closed);
        if (was_closed) {
            return error;
        }

        RepresentacionProvisiones provision;
        provision.id = id;
        provision.posicion = posicionRecibidaProv;
        provision.estaEnElAire = estaEnElAire;
        provision.tipo = (tipoProvision)tipo;
        provision.armaMunicion = (ArmaProtocolo)armaMunicion;
        provision.fueAgarrada = fueAgarrada;
        provision.exploto = exploto;


        provisiones.push_back(provision);
    }

    return provisiones;
}


std::shared_ptr<EstadoDelJuego> Protocolo::obtenerEstadoDelJuego() {
    int8_t codigo = obtenerCodigo();
    std::shared_ptr<EstadoDelJuego> estado = std::make_shared<EstadoDelJuego>();
    estado->momento = ESTADO_INVALIDO;
    if (codigo == -1 || codigo != ESTADO) {
        return estado;
    }

    idJugador jugadorDeTurno = obtenerId();
    if (jugadorDeTurno == INVAL_ID) {
        return estado;
    }

    id gusanoDeTurno = obtenerId();
    if (gusanoDeTurno == INVAL_ID) {
        return estado;
    }

    bool was_closed = false;
    int32_t segundosRestantes;
    socket.recvall(&segundosRestantes, sizeof(segundosRestantes), &was_closed);
    if (was_closed) {
        return estado;
    }
    segundosRestantes = ntohl(segundosRestantes);

    int32_t viento;
    socket.recvall(&viento, sizeof(viento), &was_closed);
    if (was_closed) {
        return estado;
    }
    viento = ntohl(viento);

    int32_t ronda;
    socket.recvall(&ronda, sizeof(ronda), &was_closed);
    if (was_closed) {
        return estado;
    }
    ronda = ntohl(ronda);

    int8_t momento;
    socket.recvall(&momento, sizeof(momento), &was_closed);
    if (was_closed) {
        return estado;
    }

    int16_t cantJugadores;
    socket.recvall(&cantJugadores, sizeof(cantJugadores), &was_closed);
    if (was_closed) {
        return estado;
    }
    cantJugadores = ntohs(cantJugadores);

    std::map<idJugador, SituacionJugador> situacionJugadores = obtenerSituacion(cantJugadores);

    std::map<idJugador, std::map<id, RepresentacionGusano>> gusanos = obtenerGusanos(cantJugadores);

    std::vector<RepresentacionProyectil> proyectiles = obtenerProyectiles();
    
    std::vector<RepresentacionProvisiones> provisiones = obtenerProvisiones();

    estado->gusanos = gusanos;
    estado->proyectiles = proyectiles;
    estado->provisiones = provisiones;
    estado->jugadorDeTurno = jugadorDeTurno;
    estado->gusanoDeTurno = gusanoDeTurno;
    estado->segundosRestantes = segundosRestantes;
    estado->momento = (MomentoDePartida)momento;
    estado->situacionJugadores = situacionJugadores;
    estado->viento = viento;
    estado->ronda = ronda;

    return estado;
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


bool Protocolo::enviarConfirmacion(InformacionInicial informacion) {
    bool is_open = enviarCodigo(EXITO);
    if (!is_open) {
        return false;
    }

    is_open = enviarId(informacion.jugador);
    if (!is_open) {
        return false;
    }

    is_open = enviarId(informacion.idPartida);
    if (!is_open) {
        return false;
    }

    is_open = enviarCantidad(informacion.vigas.size());
    if (!is_open) {
        return false;
    }

    bool was_closed = false;
    for (auto &&viga : informacion.vigas) {
        // angulo
        int32_t angulo = htonl(toInt(viga.angulo));
        socket.sendall(&angulo, sizeof(angulo), &was_closed);
        if (was_closed) {
            return false;
        }
        // largo
        int32_t largo = htonl(viga.longitud);
        socket.sendall(&largo, sizeof(largo), &was_closed);
        if (was_closed) {
            return false;
        }
        //posicion
        std::vector<int32_t> posAEnviar;
        posAEnviar.push_back(htonl((int32_t)toInt(viga.posicionInicial.enX)));
        posAEnviar.push_back(htonl((int32_t)toInt(viga.posicionInicial.enY)));

        socket.sendall(posAEnviar.data(), sizeof(int32_t)*posAEnviar.size(), &was_closed);
        if (was_closed) {
            return false;
        }
    }

    std::vector<int32_t> dimensiones;
    dimensiones.push_back(htonl((int32_t)toInt(informacion.dimensiones.enX)));
    dimensiones.push_back(htonl((int32_t)toInt(informacion.dimensiones.enY)));

    socket.sendall(dimensiones.data(), sizeof(int32_t)*dimensiones.size(), &was_closed);
    return !was_closed;
}


bool Protocolo::enviarError() {
    return enviarCodigo(ERROR);
}


Accion Protocolo::obtenerAccion() {
    int8_t codigo = obtenerCodigo();
    bool was_closed = false;
    Accion accion;
    accion.accion = INVAL_ACCION;
    if (codigo != MOV && codigo != ATACAR && 
        codigo != EQUIPAR && codigo != CALIBRAR &&
        codigo != EMPEZAR && codigo != CHEATEAR) {
        return accion;
    }

    if (codigo == EMPEZAR) {
        accion.esEmpezar = true;
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
        accion.esEmpezar = false;
        return accion;
    }

    if (codigo == CHEATEAR) {
        int8_t cheat;
        socket.recvall(&cheat, sizeof(cheat), &was_closed);
        if (was_closed) {
            return accion;
        }

        accion.accion = CHEAT;
        accion.cheat = (TipoCheat)cheat;
        accion.esEmpezar = false;
        return accion;
    }

    if (codigo == ATACAR) {
        accion.accion = ATAQUE;
        accion.esEmpezar = false;
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
        accion.esEmpezar = false;
        return accion;
        
    }
    // caso de moverse
    
    int8_t dir;
    socket.recvall(&dir, sizeof(dir), &was_closed);
    if (was_closed) {
        return accion;
    }

    accion.accion = MOVERSE;
    accion.dir = (Direccion)dir;
    accion.esEmpezar = false;
    return accion;
}

bool Protocolo::enviarSituacion(std::map<idJugador, SituacionJugador> situacionJugadores) {
    bool was_closed = false;
    for (auto const& [idJugador, situacionJugador] : situacionJugadores) {
        bool is_open = enviarId(idJugador);
        if (!is_open) {
            return false;
        }

        int8_t situacion = situacionJugador;
        socket.sendall(&situacion, sizeof(situacion), &was_closed);
        if (was_closed) {
            return false;
        }
    }
    return true;
}

bool Protocolo::enviarGusanos(std::map<idJugador, std::map<id, RepresentacionGusano>> gusanos) {
    bool was_closed = false;
    for (auto const& [idJugador, mapaGusanos] : gusanos) {
        // envio idJugador
        bool is_open = enviarId(idJugador);
        if (!is_open) {
            return false;
        }

        // envio cant de gusanos
        int cantGusanos = mapaGusanos.size();
        is_open = enviarCantidad(cantGusanos);
        if (!is_open) {
            return false;
        }

        for (auto const& [idGusano, gusano] : mapaGusanos) {
        // for (int32_t i = 0; i < cantGusanos; i++) {
            // RepresentacionGusano gusano = listaGusanos[i];
            // envio el id del gusano
            is_open = enviarId(idGusano);
            if (!is_open) {
                return false;
            }
            
            // envio vida del gusano
            uint32_t vida = htonl((uint32_t)gusano.vida);
            socket.sendall(&vida, sizeof(vida), &was_closed);
            if (was_closed) {
                return false;
            }
            
            // envio posicion :D
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

bool Protocolo::enviarProyectiles(std::vector<RepresentacionProyectil> proyectiles) {
    bool is_open = enviarCantidad(proyectiles.size());
    if (!is_open) {
        return false;
    }

    bool was_closed = false;
    for (auto &&proyectil : proyectiles) {
        is_open = enviarId(proyectil.id);
        if (!is_open) {
            return false;
        }
        int8_t tipoProyectil = proyectil.proyectil;
        socket.sendall(&tipoProyectil, sizeof(tipoProyectil), &was_closed);
        if (was_closed) {
            return false;
        }

        int8_t esFragmento = proyectil.esFragmento;
        socket.sendall(&esFragmento, sizeof(esFragmento), &was_closed);
        if (was_closed) {
            return false;
        }

        std::vector<int32_t> posProyectil;
        posProyectil.push_back(htonl((int32_t)toInt(proyectil.posicion.enX)));
        posProyectil.push_back(htonl((int32_t)toInt(proyectil.posicion.enY)));

        socket.sendall(posProyectil.data(), sizeof(int32_t)*posProyectil.size(), &was_closed);
        if (was_closed) {
            return false;
        }

        int32_t anguloProyectil = htonl(toInt(proyectil.angulo));
        socket.sendall(&anguloProyectil, sizeof(anguloProyectil), &was_closed);
        if (was_closed) {
            return false;
        }

        int32_t cuentaRegProyectil = htonl(proyectil.cuentaRegresiva);
        socket.sendall(&cuentaRegProyectil, sizeof(cuentaRegProyectil), &was_closed);
        if (was_closed) {
            return false;
        }

        int8_t exploto = proyectil.exploto;
        socket.sendall(&exploto, sizeof(exploto), &was_closed);
        if (was_closed) {
            return false;
        }
    }
    return true;
}

bool Protocolo::enviarProvisiones(std::vector<RepresentacionProvisiones> provisiones) {
    bool was_closed = false;
    bool is_open = enviarCantidad(provisiones.size());
    if (!is_open) {
        return false;
    }
    for (auto &&provision : provisiones) {
        is_open = enviarId(provision.id);
        if (!is_open) {
            return false;
        }
        std::vector<int32_t> posProvision;
        posProvision.push_back(htonl((int32_t)toInt(provision.posicion.enX)));
        posProvision.push_back(htonl((int32_t)toInt(provision.posicion.enY)));

        socket.sendall(posProvision.data(), sizeof(int32_t)*posProvision.size(), &was_closed);
        if (was_closed) {
            return false;
        }

        int8_t estaEnElAire = provision.estaEnElAire;
        socket.sendall(&estaEnElAire, sizeof(estaEnElAire), &was_closed);
        if (was_closed) {
            return false;
        }

        int8_t tipo = provision.tipo;
        socket.sendall(&tipo, sizeof(tipo), &was_closed);
        if (was_closed) {
            return false;
        }

        int8_t armaMunicion = provision.armaMunicion;
        socket.sendall(&armaMunicion, sizeof(armaMunicion), &was_closed);
        if (was_closed) {
            return false;
        }

        int8_t fueAgarrada = provision.fueAgarrada;
        socket.sendall(&fueAgarrada, sizeof(fueAgarrada), &was_closed);
        if (was_closed) {
            return false;
        }

        int8_t exploto = provision.exploto;
        socket.sendall(&exploto, sizeof(exploto), &was_closed);
        if (was_closed) {
            return false;
        }
    }
    
    return true;
}

// se manda ESCENARIO+jugadorDeTurno+gusanoDeTurno+tiempoRestante+cantJugadores+[idJugador+cantGusanos+[id+vida+posX+posY+dir+arma]]
bool Protocolo::enviarEstadoDelJuego(std::shared_ptr<EstadoDelJuego> estado) {
    bool is_open = enviarCodigo(ESTADO);
    if (!is_open) {
        return false;
    }

    is_open = enviarId(estado->jugadorDeTurno);
    if (!is_open) {
        return false;
    }

    is_open = enviarId(estado->gusanoDeTurno);
    if (!is_open) {
        return false;
    }

    bool was_closed = false;
    int32_t tiempoRestante = htonl(estado->segundosRestantes);
    socket.sendall(&tiempoRestante, sizeof(tiempoRestante), &was_closed);
    if (was_closed) {
        return false;
    }

    int32_t viento = htonl(estado->viento);
    socket.sendall(&viento, sizeof(viento), &was_closed);
    if (was_closed) {
        return false;
    }

    int32_t ronda = htonl(estado->ronda);
    socket.sendall(&ronda, sizeof(ronda), &was_closed);
    if (was_closed) {
        return false;
    }

    int8_t momento = estado->momento;
    socket.sendall(&momento, sizeof(momento), &was_closed);
    if (was_closed) {
        return false;
    }
    
    //envio cantJugadores
    int cant = estado->gusanos.size();
    is_open = enviarCantidad(cant);
    if (!is_open) {
        return false;
    }

    is_open = enviarSituacion(estado->situacionJugadores);
    if (!is_open) {
        return false;
    }

    is_open = enviarGusanos(estado->gusanos);
    if (!is_open) {
        return false;
    }
    

    is_open = enviarProyectiles(estado->proyectiles);
    if (!is_open) {
        return false;
    }
    
    return enviarProvisiones(estado->provisiones);
}

//Endif de la macro de SERVER
#endif
