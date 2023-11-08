#include "cliente.h"


Cliente::Cliente(Socket&& socket, strings mapasDisponibles, MonitorPartida& monitorPartidas) 
    : protocolo(std::move(socket)), sender(std::ref(protocolo)), 
      reciever(std::ref(protocolo), mapasDisponibles, monitorPartidas, this) {
	this->sender.start();
    this->reciever.start();
};

void Cliente::obtenerAccesoAAcciones(Queue<Accion>* acciones) {
    this->reciever.obtener(acciones);
}


void Cliente::enviarEstadoJuego(EstadoDelJuego estado) {
    this->sender.anadirEstado(estado);
}
