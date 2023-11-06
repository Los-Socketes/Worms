#include "jugador.h"
#include "partida.h"
#include <cstdlib>

// class MonitorPartida {
// public:
//     [[nodiscard]] id anadirPartida(std::string mapaNombre);

//     void anadirJugadorAPartida(Jugador *nuevoJugador, id partidaEspecifica);

//     [[nodiscard]] std::vector<RepresentacionPartida> partidasDisponibles();
// };

Jugador::Jugador() {
    this->gusanoActualPos = 0;

    // MOVIMOS A RECIEVER + CLIENTE
    //A partir de aca hasta el final esta la logica de "lobby".
    //No usamos los threads enviador y recibidor porque la comunicacion
    //es sincronica
    

    //Ahora que el jugador se unio a una partida, creo los dos threads
    //comunicadores
    // this->enviador.start();
    // this->recibidor.start();
}


void Jugador::obtenerGusanosIniciales(std::vector<Gusano*> gusanos) {
    this->gusanos = gusanos;
}


// bool Jugador::obtenerAccion(Direccion& primeraAccion) {
//     bool pudeObtenerla;
//     pudeObtenerla = this->acciones->try_pop(primeraAccion);

//     return pudeObtenerla;
// }

Gusano *Jugador::getGusanoActual() {
    Gusano *gusanoActual = nullptr;

    //Busco el primer gusano no muerto
    while (gusanoActual == nullptr) {
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

// void Jugador::obtenerAccesoAAcciones(Queue<Direccion>* acciones){
//     //TODO Sacar tal vez?
//     this->acciones = acciones;
//     this->recibidor.darAcceso(acciones);
// }


// void Jugador::enviarEstadoJuego(EstadoDelJuego estado) {
//     this->enviador.anadirEstado(estado);
// }
