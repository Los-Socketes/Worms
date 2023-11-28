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
#include "controliteracion.h"
#include "configuracionCliente.h"

using namespace SDL2pp;

#define TAM_QUEUE 500
#define FPS 30

// Clase principal del cliente. Se encarga de ejecutar el menu, iniciar los hilos,
// y ejecutar el loop principal del juego.
class Cliente {
 private:
    SDL sdl;
    SDLTTF ttf;
    Protocolo protocolo;
    ConfiguracionCliente& config;
    std::shared_ptr<EstadoDelJuego> estado_juego;
    Camara camara;
    Dibujador dibujador;
    ControlIteracion control_iteracion;
    Menu menu;
    Queue<std::shared_ptr<EstadoDelJuego>> recepcion_estados;
    Queue<std::shared_ptr<AccionCliente>> envio_comandos;
    Queue<Comando> comandos_teclado;
    std::atomic<bool> es_turno;
    std::atomic<bool> municiones_agotadas;
    bool inicio;
    EntradaTeclado entrada_teclado;
    Recibidor recibidor;
    Enviador enviador;
    std::pair<int, int> pos_cursor;
    int volumen;
    bool muteado;

    // Inicia los hilos.
    void iniciar();

 public:
    Cliente(Socket&& skt, ConfiguracionCliente& config);
    
    // Ejecuta el menu.
    InformacionInicial ejecutar_menu(int argc, char* argv[]);

    // Loop principal del cliente.
    void loop_principal(InformacionInicial& info_inicial);

    // Indica si se salio desde el menu.
    bool salioDesdeMenu();

    // Destructor.
    ~Cliente();
};

#endif // CLIENTE_H_