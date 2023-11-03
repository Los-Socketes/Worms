#ifndef CLIENTE_H_
#define CLIENTE_H_

#include <SDL2pp/SDL2pp.hh>

#include "entradateclado.h"
#include "recibidor.h"
#include "enviador.h"
#include "socket.h"
#include "protocolo.h"
#include "queue.h"
#include "menu.h"
#include "animacion.h"
#include "defs.h"

using namespace SDL2pp;

#define TAM_QUEUE 500
#define FPS 30

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class Cliente {
 private:
    Protocolo protocolo;
    EstadoDelJuego estado_juego;
    Menu menu;
    Queue<EstadoDelJuego> recepcion_estados;
    Queue<std::string> envio_comandos;
    Queue<std::string> comandos_teclado;
    EntradaTeclado entrada_teclado;
    Recibidor recibidor;
    Enviador enviador;

    // Inicia los hilos.
    void iniciar();

    // Renderiza el estado del juego.
    void renderizar(Renderer& renderizador, Animacion& caminar, int itq);
 public:
    Cliente(Socket&& skt);
    
    // Ejecuta el menu.
    bool ejecutar_menu();

    // Loop principal del cliente.
    void loop_principal();

    // Destructor.
    ~Cliente();
};

#endif // CLIENTE_H_