#include "reciever.h"
#include "defs.h"
#include "protocolo.h"
#include <cstdlib>

class MonitorPartida {public: [[nodiscard]] id anadirPartida(std::string mapaNombre);

    [[nodiscard]] idJugador anadirJugadorAPartida(Cliente *nuevoJugador, id partidaEspecifica);

    [[nodiscard]] std::vector<RepresentacionPartida> partidasDisponibles();
};


Reciever::Reciever(Protocolo& protocol, strings mapasDisponibles, MonitorPartida& monitorPartidas, Cliente *cliente) 
    : protocolo(protocol), partidas(monitorPartidas), cliente(cliente) {
        this->mapasDisponibles = mapasDisponibles;
    }

void Reciever::lobby() {
    tipoInfo pedido;
    pedido = this->protocolo.obtenerPedido();
    id partidaElegida;
    switch (pedido) {
        case MAPA:
	  {
	      //Le mando los mapas que me dieron
	      this->protocolo.enviarMapas(mapasDisponibles);

	      //Me devuelve el indice del mapas deseado
	      id mapaDeseado = this->protocolo.obtenerMapaDeseado();
	      std::string nombreMapaDeseado;
	      nombreMapaDeseado = mapasDisponibles.at(mapaDeseado);

	      partidaElegida = partidas.anadirPartida(nombreMapaDeseado);

	  break;
	  }
        case PARTIDA:
	  {
	  std::vector<RepresentacionPartida> partidasAEnviar;
	  partidasAEnviar = partidas.partidasDisponibles();

	  this->protocolo.enviarPartidas(partidasAEnviar);

	  partidaElegida = this->protocolo.obtenerPartidaDeseada();
	  break;
	  }
        case INVAL_TIPO:
	  {
	  //TODO: _hacer algo_
	  abort();
	  break;
	  }
    }

    this->protocolo.enviarConfirmacion(partidaElegida);
    // TODO: cambiar a que sea de cliente o algo idk
    idJugador idDelNuevoJugador;
    idDelNuevoJugador = partidas.anadirJugadorAPartida(this->cliente, partidaElegida);
    this->miId = idDelNuevoJugador;
}


void Reciever::obtener(Queue<Accion> *accionesRecibidas) {
    //TODO Throw
    if (accionesRecibidas == nullptr)
        abort();

    this->acciones = accionesRecibidas;
}

void Reciever::run() {
    //TODO Cambiar a socket vivo o algo
    lobby();
    while (true) {
        Accion accionDeseada;
        accionDeseada = this->protocolo.obtenerAccion();
        accionDeseada.jugador = this->miId;

        this->acciones->push(accionDeseada);
    }
}
