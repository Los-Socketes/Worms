#include "aceptador.h"
#include "liberror.h"


Aceptador::Aceptador(const char *puerto)
    : socket(puerto) {};

void Aceptador::aceptarClientes() {
    //TODO: Hacer que en vez de true sea socket cerrado
    while (!this->socket.is_closed()) {
        try {
            Socket conexionEntrante = this->socket.accept();

            //TODO Anadir un reap dead ACA que busque en una lista de jugadores
            Cliente* cliente = new Cliente(std::move(conexionEntrante), this->escenariosDisponibles, std::ref(this->partidas));

            // Primero el reap_dead para que la lista tenga un elemento menos
            reap_dead();
            this->listaClientes.push_back(cliente);
        }
        // catch de que se cerro el socket
        catch(const LibError& e) {
            // std::cerr << e.what() << '\n';
            break;
        }
        
    }

}

void Aceptador::asignar(std::vector<std::string> escenariosDisponibles) {
    this->escenariosDisponibles = escenariosDisponibles;
}

void Aceptador::reap_dead() {
    this->listaClientes.remove_if([](Cliente* c) {
        if (c->estaMuerto()) {
            delete c;
            return true;
        }
        return false;
    });
}

void Aceptador::kill() {
    for (auto &&cliente : this->listaClientes) {
        if (!cliente->estaMuerto()) {
            cliente->cerrarSocket(SHUT_RDWR);
            std::cout << "delete\n";
            delete cliente;
        }
    }
    reap_dead();
    this->socket.shutdown(SHUT_RDWR);
    this->socket.close();
}
