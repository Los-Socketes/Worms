#ifndef PARTIDA_HEADER
#define PARTIDA_HEADER

#include <box2d/box2d.h>

#include <mutex>
#include <utility>
#include <vector>
#include <condition_variable>
#include <set>

#include "cliente.h"
#include "mapa.h"
#include "defs.h"
#include "thread.h"
#include "jugador.h"
#include "queue.h"
#include "provision.h"
#include "resolvedores.h"

//El game loop ES nuestra funcion run
#define gameLoop run

#define MINJUGADORES 1

class Partida : public Thread {
    std::atomic<bool> finPartida;
    Queue<Accion> acciones;

    bool termino;
    MomentoDePartida momento;

    //World de box2d de la partida
    b2World world;
    b2Vec2 viento;
    ResolvedorColisiones colisiones;
    ResolvedorQuery query;
    //WARNING Actualmente solo usado para sacar los cuerpos creados en
    //las explosiones
    std::vector<b2Body *> cuerposADestruir;
    // ResolvedorDestruccion destucciones;

    // std::string mapa;
    Mapa mapaUsado;
    std::vector<RepresentacionViga> vigasEnMapa;
    std::pair<coordX, coordY> dimensiones;
    std::map<int, std::pair<coordX, coordY>> posicionesGusanos;
    int cantidad_gusanos_insertados;

    std::vector<Cliente *> clientes;
    std::mutex mtx;
    std::condition_variable seUnioJugador;

    std::vector<Gusano *> gusanos;

    int posJugadorActual = 0;
    std::vector<Jugador *> jugadores;
    Jugador *siguienteJugador(Jugador *viejoJugador);

    std::vector<Proyectil *> proyectiles;

    time_t ultimaProvision;
    void generarProvision(time_t horaActual);
    std::vector<Provision *> provisiones;
    void anadirProvision(coordX coordEnX);
    //Uso este valor para que se no se pisen los valores
    int cantidadProvisionesGeneradas = 0;

    time_t ultimoCambioViento;
    void cambiarElViento(time_t tiempoActual);

    bool enviarEstadoAJugadores();

    void procesarCheats(Accion cheat, Gusano *gusanoActual);

    [[nodiscard]] Accion obtenerAccion(Accion accionObtenida, bool obtuvoNueva,
		     Gusano* gusanoActual);

    [[nodiscard]] Gusano *anadirGusano(std::pair<coordX, coordY> coords);

    void anadirViga(radianes angulo, int longitud, std::pair<coordX, coordY> posicionInicial);


    void anadirOceano(std::pair<coordX, coordY> posicionInicial);

    void borrarCuerpos(); 

    bool sePuedeCambiarDeJugador(Gusano *gusanoActual, time_t tiempoActual);

    std::pair<Gusano *, Jugador *> cambiarDeJugador(Jugador *jugadorTurnoActual, Gusano *gusanoActual, time_t tiempoActual);

    //Bueno que lo construye
    Proyectil *proyectilConstructor();

    void generarExplosion(Proyectil *proyectil);

    void crearProyectiles(Gusano *gusano, Ataque ataque);
    void crearFragmentos(Proyectil* proyectil, int cantFragmentos);

public:
    Partida(Mapa mapa);

    [[nodiscard]] InformacionInicial obtenerInfoInicial();

    void anadirCliente(Cliente *clienteNuevo);

    void gameLoop() override;
    void stop() override;

    ~Partida();
};

#endif
