#include "reciever.h"
#include "protocolo.h"
#include <cstdlib>


Reciever::Reciever(Protocolo& protocol, strings mapasDisponibles, MonitorPartida& monitorPartidas) 
    : protocolo(protocol), partidasDisponibles(monitorPartidas) {
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

	      partidaElegida = partidasDisponibles.anadirPartida(nombreMapaDeseado);

	  break;
	  }
        case PARTIDA:
	  {
	  std::vector<RepresentacionPartida> partidasAEnviar;
	  partidasAEnviar = partidasDisponibles.partidasDisponibles();

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
    // partidasDisponibles.anadirJugadorAPartida(this, partidaElegida);
}


void Reciever::darAcceso(Queue<Direccion> *accionesRecibidas) {
    //TODO Throw
    if (accionesRecibidas == nullptr)
        abort();

    this->acciones = accionesRecibidas;
}

void Reciever::run() {
    //TODO Cambiar a socket vivo o algo
    lobby();
    while (true) {
        Direccion accionDeseada;
        accionDeseada = this->protocolo.obtenerAccion();

        this->acciones->push(accionDeseada);
    }
}
