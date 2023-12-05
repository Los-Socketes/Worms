#include "aceptador.h"
#include "liberror.h"


Aceptador::Aceptador(const char *puerto) : socket(puerto) {};

void Aceptador::aceptarClientes() {
    while (!this->socket.is_closed()) {
        try {
            Socket conexionEntrante = this->socket.accept();

            Cliente* cliente = new Cliente(std::move(conexionEntrante), this->escenariosDisponibles, std::ref(this->partidas));

            // Primero el reap_dead para que la lista tenga un elemento menos
            reap_dead();
            this->listaClientes.push_back(cliente);
        }
        // catch de que se cerro el socket
        catch(const LibError& e) {
            std::cerr << "[ACEPTADOR] " << e.what() << '\n';
            break;
        }
        catch(const std::runtime_error& e) {
            std::cerr << "[ACEPTADOR] " << e.what() << '\n';
            break;
        }
        
    }

}


void Aceptador::reap_dead() {
    this->listaClientes.remove_if([](Cliente* c) {
        if (c->estaMuerto()) {
            delete c;
            return true;
        }
        return false;
    });
    this->partidas.reapDead();
}

void Aceptador::kill() {
    this->partidas.kill();
    reap_dead();
    this->socket.shutdown(SHUT_RDWR);
    this->socket.close();
    for (auto &&cliente : this->listaClientes) {
        if (!cliente->estaMuerto()) {
            cliente->cerrarSocket(SHUT_RDWR);
            std::cout << "delete\n";
            delete cliente;
        }
    }
}
