#include "jugador.h"
#include "partida.h"
#include <cstdlib>

class MonitorPartida {
public:
    [[nodiscard]] id anadirPartida(std::string mapaNombre);

    void anadirJugadorAPartida(Jugador *nuevoJugador, id partidaEspecifica);

    [[nodiscard]] std::vector<RepresentacionPartida> partidasDisponibles();
};

Jugador::Jugador(Socket &&socket, std::vector<std::string> mapasDisponibles,
	       MonitorPartida& partidasDisponibles):
      protocolo(std::move(socket)),
      enviador(this->protocolo),
      recibidor(this->protocolo, this->acciones)
{
    this->gusanoActualPos = 0;

    //A partir de aca hasta el final esta la logica de "lobby".
    //No usamos los threads enviador y recibidor porque la comunicacion
    //es sincronica
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
    partidasDisponibles.anadirJugadorAPartida(this, partidaElegida);

    //Ahora que el jugador se unio a una partida, creo los dos threads
    //comunicadores
    this->enviador.start();
    this->recibidor.start();
}


void Jugador::obtenerGusanosIniciales(std::vector<Gusano*> gusanos) {
    this->gusanos = gusanos;
}


bool Jugador::obtenerAccion(Direccion& primeraAccion) {
    bool pudeObtenerla;
    pudeObtenerla = this->acciones->try_pop(primeraAccion);

    return pudeObtenerla;
}

Gusano *Jugador::getGusanoActual() {
    Gusano *gusanoActual = nullptr;

    //Busco el primer gusano no muerto
    while (gusanoActual != nullptr) {
        gusanoActual = this->gusanos.at(gusanoActualPos);
        this->gusanoActualPos += 1;

        //WARNING Casteo falopa. En teoria nada deberia explotar
        if (this->gusanoActualPos > (int) this->gusanos.size())
	  this->gusanoActualPos = 0;
    }

    //Una vez encontrado dicho gusano, la proxima iteracion ya arranca
    //con el proximo

    return gusanoActual;
}

void Jugador::obtenerAccesoAAcciones(Queue<Direccion>* acciones){
    this->acciones = acciones;
}


void Jugador::enviarEstadoJuego(EstadoDelJuego estado) {
    this->enviador.anadirEstado(estado);
}
