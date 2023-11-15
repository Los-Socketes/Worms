#ifndef CLIENTE_H_
#define CLIENTE_H_

#include <SDL2pp/SDL2pp.hh>

#include "entradateclado.h"
#include "recibidor.h"
#include "enviador.h"
#include "camara.h"
#include "socket.h"
#include "protocolo.h"
#include "queue.h"
#include "menu.h"
#include "animacion.h"
#include "defs.h"
#include "accioncliente.h"
#include "dibujador.h"

using namespace SDL2pp;

#define TAM_QUEUE 500
#define FPS 30

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// A reemplazar cuando reciba datos del mapa.
const int MAPA_ANCHO = 1500;
const int MAPA_ALTO = 700;

class Cliente {
 private:
    SDL sdl;
    SDLTTF ttf;
    Protocolo protocolo;
    EstadoDelJuego estado_juego;
    Camara camara;
    Dibujador dibujador;
    Menu menu;
    Queue<EstadoDelJuego> recepcion_estados;
    Queue<std::shared_ptr<AccionCliente>> envio_comandos;
    Queue<Comando> comandos_teclado;
    EntradaTeclado entrada_teclado;
    Recibidor recibidor;
    Enviador enviador;
    // Valor temporal, angulo global para probar.
    radianes angulo;

    // Inicia los hilos.
    void iniciar();

    // Renderiza el estado del juego.
    void renderizar(Renderer& renderizador, Animacion& caminar, Animacion& agua, int itq);
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