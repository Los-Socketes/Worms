#include "cliente.h"
#include "jugador.h"
#include "partida.h"
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
    //Paso 1: Envio los mapas
    this->protocolo.enviarMapas(this->mapasDisponibles);

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

    //Paso 3: Obtengo la partida deseada
    int partidaElejida = this->protocolo.obtenerPartidaDeseada();

    Partida *partidaElegida = this->avisar.at(partidaElejida);

    Jugador *jugadorNuevo = new Jugador(std::move(this->protocolo));

    partidaElegida->anadirJugador(jugadorNuevo);

    this->conectadoAPartida = true;
}
