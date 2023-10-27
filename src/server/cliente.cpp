#include "cliente.h"
#include "jugador.h"
#include "partida.h"
#include "protocolo.h"
#include "socket.h"
#include <string>

// class Server {
// public:
//     void anadirJugadorAPartida(Protocolo&& socketDeJugador, id idPartida);
// };

Cliente::Cliente(Socket&& socket, std::vector<std::string> mapasDisponibles,
	       TSList<Partida*>& avisar) 
    : protocolo(std::move(socket)), avisar(avisar) {
    this->conectadoAPartida = false;
    this->mapasDisponibles = mapasDisponibles;
};

//Esto corre en un thread
/*
    obtener pedido
    if pedido == mapa:
        *enviarMapas
        *recibirMapaSeleccionado
        Creo partida (y lo uno)
        *envioConfirmacion con idPartida / envioError si no pude creala
    else:
        *enviarPartidas
        *recibirPartidaSeleccionada
        lo uno
        *envioConfirmacion con idPartida / envioError si no pude creala

    me vuelvo jugador
*/
void Cliente::elegirPartida() {
    tipoInfo pedido;
    pedido = this->protocolo.obtenerPedido();

    Partida *partidaElegida;
    id partida;

    //Paso 1: Envio los mapas
    switch (pedido) {
        case MAPA:
	  {
	  this->protocolo.enviarMapas(this->mapasDisponibles);

	  //Obtengo el nombre del mapa que me pidio
	  id mapaDeseado = this->protocolo.obtenerMapaDeseado();
	  std::string nombreMapaDeseado;
	  nombreMapaDeseado = this->mapasDisponibles.at(mapaDeseado);

	  //Creo la nueva partida y chequeo si se creo correctamente
	  //Si no, error; si si, digo que si :D
	  partidaElegida = new Partida(nombreMapaDeseado);
	  if (partidaElegida == nullptr)
	      this->protocolo.enviarError();

	  //La anado a la lista para que clientes subsecuentes puedan
	  //usarla
	  this->avisar.append(partidaElegida);
	  //TODO TAL VEZ hacer que append devuelva la posicion
	  //idk, maybe *sniff* maybe
	  partida = this->avisar.size() - 1;

	  break;
	  }

        case PARTIDA:
	  {
	  //Paso 2: Envio las partidas
	  std::vector<RepresentacionPartida> partidasAEnviar;
	  for (int i = 0; i < this->avisar.size() ; i++) {
	      Partida* partida = this->avisar.at(i);

	      if (partida == nullptr)
		continue;


	      RepresentacionPartida repreActual;
	      repreActual.ID = i;

	      partidasAEnviar.push_back(repreActual);
	  }
	  this->protocolo.enviarPartidas(partidasAEnviar);
	  partida = this->protocolo.obtenerPartidaDeseada();

	  partidaElegida = this->avisar.at(partida);

	  // partidaElejida->anadirJugador
	  break;
	  }
    };

    Jugador *jugadorNuevo = new Jugador(std::move(this->protocolo));

    //Uno al jugador
    partidaElegida->anadirJugador(jugadorNuevo);

    this->protocolo.enviarConfirmacion(partida);

    this->conectadoAPartida = true;
}
