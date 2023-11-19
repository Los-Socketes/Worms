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
// Mapa de 75 metros de ancho por 40 de alto.
const int MAPA_ANCHO = 75;
const int MAPA_ALTO = 40;

class Cliente {
 private:
    SDL sdl;
    SDLTTF ttf;
    Protocolo protocolo;
    std::shared_ptr<EstadoDelJuego> estado_juego;
    Camara camara;
    Dibujador dibujador;
    Menu menu;
    Queue<std::shared_ptr<EstadoDelJuego>> recepcion_estados;
    Queue<std::shared_ptr<AccionCliente>> envio_comandos;
    Queue<Comando> comandos_teclado;
    std::atomic<bool> es_turno;
    EntradaTeclado entrada_teclado;
    Recibidor recibidor;
    Enviador enviador;
    std::pair<int, int> pos_cursor;

    // Inicia los hilos.
    void iniciar();
 public:
    Cliente(Socket&& skt);
    
    // Ejecuta el menu.
    InformacionInicial ejecutar_menu();

    // Loop principal del cliente.
    void loop_principal(InformacionInicial& info_inicial);

    // Destructor.
    ~Cliente();
};

#endif // CLIENTE_H_