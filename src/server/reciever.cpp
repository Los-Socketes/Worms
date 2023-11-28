#include "reciever.h"
#include "defs.h"
#include "protocolo.h"
#include <cstdlib>

class MonitorPartida {public: [[nodiscard]] id anadirPartida(std::string mapaNombre);

    void anadirJugadorAPartida(Cliente *nuevoJugador, id partidaEspecifica);

    [[nodiscard]] InformacionInicial obtenerInfoInicialDePartida(id partidaEspecifica);

    [[nodiscard]] std::vector<RepresentacionPartida> partidasDisponibles();
};


Reciever::Reciever(Protocolo& protocol, strings mapasDisponibles, MonitorPartida& monitorPartidas, Cliente *cliente) 
    : protocolo(protocol), partidas(monitorPartidas), cliente(cliente) {
        this->mapasDisponibles = mapasDisponibles;
        this->acciones = nullptr;
    }

bool Reciever::lobby() {
    bool envio = true;
    tipoInfo pedido;
    pedido = this->protocolo.obtenerPedido();
    id partidaElegida;
    switch (pedido) {
        case MAPA:
	    {
        //Le mando los mapas que me dieron
        envio = this->protocolo.enviarMapas(mapasDisponibles);
        if (!envio) {
            return false;
        }
        //Me devuelve el indice del mapas deseado
        id mapaDeseado = this->protocolo.obtenerMapaDeseado();
        if (mapaDeseado == INVAL_ID) {
            return false;
        }
        std::string nombreMapaDeseado;
        nombreMapaDeseado = mapasDisponibles.at(mapaDeseado);

        partidaElegida = partidas.anadirPartida(nombreMapaDeseado);

        break;
        }
        case PARTIDA:
        {
        std::vector<RepresentacionPartida> partidasAEnviar;
        partidasAEnviar = partidas.partidasDisponibles();

        envio = this->protocolo.enviarPartidas(partidasAEnviar);
        if (!envio) {
            return false;
        }
        partidaElegida = this->protocolo.obtenerPartidaDeseada();
        if (partidaElegida == INVAL_ID) {
            return false;
        }
        break;
        }
        case INVAL_TIPO:
        {
        //TODO: _hacer algo_
        //   abort();
        //   break;
        return false;
        }
    }

    InformacionInicial infoInicial;
    infoInicial = partidas.obtenerInfoInicialDePartida(partidaElegida);
    // infoInicial = partidas.anadirJugadorAPartida(this->cliente, partidaElegida);

    envio = this->protocolo.enviarConfirmacion(infoInicial);
    if (!envio) {
        return false;
    }
    partidas.anadirJugadorAPartida(this->cliente, partidaElegida);
    // TODO: cambiar a que sea de cliente o algo idk
    this->miId = infoInicial.jugador;
    return true;
}


void Reciever::obtener(Queue<Accion> *accionesRecibidas) {
    //TODO Throw
    if (accionesRecibidas == nullptr)
        abort();

    this->acciones = accionesRecibidas;
}

void Reciever::run() {
    //TODO Cambiar a socket vivo o algo
    try {
        if (!lobby()) {
            return;
        }
        while (true) {
            Accion accionDeseada;
            accionDeseada = this->protocolo.obtenerAccion();
            if (accionDeseada.accion == INVAL_ACCION && !accionDeseada.esEmpezar) {
                break;
            }
            accionDeseada.jugador = this->miId;

            this->acciones->push(accionDeseada);
            // std::cout << "PUSHEO: " << accionDeseada.esEmpezar << "\n";
        }
    }
    catch( ... ) {
        return;
    }
    
}

