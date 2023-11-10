#include "cliente.h"


Cliente::Cliente(Socket&& socket, strings mapasDisponibles, MonitorPartida& monitorPartidas) 
    : protocolo(std::move(socket)), sender(std::ref(protocolo)), 
      reciever(std::ref(protocolo), mapasDisponibles, monitorPartidas, this) {
    this->reciever.start();
	this->sender.start();
};

void Cliente::obtenerAccesoAAcciones(Queue<Accion>* acciones) {
    this->reciever.obtener(acciones);
}


inline void Cliente::enviarEstadoJuego(EstadoDelJuego estado) {
    this->sender.anadirEstado(estado);
}


void Cliente::cerrarSocket(int forma) { 
    this->protocolo.cerrarConexion(forma); 
}

Cliente::~Cliente() {
    this->reciever.join();
    std::cout << "despues reciever\n";
    this->sender.stop();
    this->sender.join();
    std::cout << "despues sender\n";
}


bool Cliente::estaMuerto() {
    return !this->reciever.is_alive();
}
