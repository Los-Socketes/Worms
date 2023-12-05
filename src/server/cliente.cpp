#include "cliente.h"

Cliente::Cliente(Socket&& socket, Mapas mapasDisponibles, MonitorPartida& monitorPartidas) 
    : protocolo(std::move(socket)), sender(std::ref(protocolo)), 
      reciever(std::ref(protocolo), mapasDisponibles, monitorPartidas, this) {
    this->reciever.start();
	this->sender.start();
};

void Cliente::obtenerAccesoAAcciones(Queue<Accion>* acciones) {
    this->reciever.obtener(acciones);
}


void Cliente::enviarEstadoJuego(std::shared_ptr<EstadoDelJuego> estado) {
    this->sender.anadirEstado(estado);
}


void Cliente::cerrarSocket(int forma) { 
    this->protocolo.cerrarConexion(forma); 
}

Cliente::~Cliente() {
    this->reciever.join();
    this->sender.stop();
    this->sender.join();
}

bool Cliente::estaMuerto() {
    return !this->reciever.is_alive();
}
