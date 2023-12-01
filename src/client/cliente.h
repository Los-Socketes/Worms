#ifndef CLIENTE_H_
#define CLIENTE_H_

#include <SDL2pp/SDL2pp.hh>

#include "entradaTeclado.h"
#include "recibidor.h"
#include "enviador.h"
#include "camara.h"
#include "socket.h"
#include "protocolo.h"
#include "queue.h"
#include "menu.h"
#include "defs.h"
#include "accionCliente.h"
#include "controlEntidades.h"
#include "configuracionCliente.h"

using namespace SDL2pp;

#define TAM_QUEUE 500

// Clase principal del cliente. Se encarga de ejecutar el menu, iniciar los hilos,
// y ejecutar el loop principal del juego.
class Cliente {
 private:
    // Configuracion
    ConfiguracionCliente& config;
    // SDL
    SDL sdl;
    SDLTTF ttf;
    Window ventana;
    Renderer renderizador;
    Mixer mixer;
    Music music;
    // Conexion a servidor
    Protocolo protocolo;
    std::shared_ptr<EstadoDelJuego> estado_juego;
    bool es_host;
    // Juego
    Camara camara;
    std::pair<int, int> pos_cursor;
    int volumen;
    bool muteado;
    Font fuente1;
    Font fuente2;
    ControlEntidades control_entidades;
    Menu menu;
    // Hilos
    Queue<std::shared_ptr<EstadoDelJuego>> recepcion_estados;
    Queue<std::shared_ptr<AccionCliente>> envio_comandos;
    Queue<Comando> comandos_teclado;
    std::atomic<bool> es_turno;
    std::atomic<bool> municiones_agotadas;
    bool inicio;
    EntradaTeclado entrada_teclado;
    Recibidor recibidor;
    Enviador enviador;

    // Configura SDL, SDL_mixer.
    void configurarSDL(InformacionInicial& info_inicial);

    // Configura camara.
    void configurarCamara(InformacionInicial& info_inicial);

    // Configura el controlador de entidades.
    void configurarControlEntidades(InformacionInicial& info_inicial);

    // Inicia los hilos.
    void iniciarHilos(InformacionInicial& info_inicial);

    // Dado un comando local, ejecuta la accion correspondiente.
    // Setea los flags continuar y mover_camara.
    void ejecutarComandoTeclado(Comando& comando, bool& continuar, bool& mover_camara);

    // Busca el objetivo para enfocar la camara.
    void actualizarObjetivoCamara();

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