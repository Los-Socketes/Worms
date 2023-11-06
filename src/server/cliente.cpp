#include "cliente.h"
#include "jugador.h"
#include "partida.h"
#include "protocolo.h"
#include "socket.h"
#include <string>


Cliente::Cliente(Socket&& socket, strings mapasDisponibles, MonitorPartida& monitorPartidas) 
    : protocolo(std::move(socket)), sender(std::ref(protocolo)), 
	reciever(std::ref(protocolo), mapasDisponibles, monitorPartidas) {
    // Jugador jugadorNuevo();
	this->sender.start();
    this->reciever.start();
};

void Cliente::obtenerAccesoAAcciones(Queue<Direccion>* acciones) {
    //TODO Sacar tal vez?
    // this->acciones = acciones;
    this->reciever.darAcceso(acciones);
}


void Cliente::enviarEstadoJuego(EstadoDelJuego estado) {
    this->sender.anadirEstado(estado);
}


