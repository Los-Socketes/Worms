#ifndef PARTIDA_HEADER
#define PARTIDA_HEADER

#include "cliente.h"
#include "defs.h"
#include "thread.h"
#include <utility>
#include <vector>
#include "jugador.h"
#include "queue.h"
#include <condition_variable>
#include <mutex>
#include <box2d/box2d.h>
#include <set>

//El game loop ES nuestra funcion run
#define gameLoop run

#define MINJUGADORES 2

// Uso potencia de 2 para operaciones con bits (con quien colisiona cada clase)


// Este struct se usa para asociar facilmente un body de box2d a
// alguna de nuestras clases. En teoria se podria usar solo el puntero,
// pero esto nos evita casteos falopas y hace que todas los bodies tengan
// lo mismo. Aparte usamos un union, en memoria es casi el mismo tamano


// class ResolvedorDestruccion : public b2DestructionListener {
//     void SayGoodbye(b2Fixture *fixture); 	
//     void SayGoodbye(b2Joint *joint); 	
// };

class ResolvedorColisiones : public b2ContactListener {
public:
    std::atomic<bool> finPartida;
    void BeginContact(b2Contact* contact);
 
    void EndContact(b2Contact* contact);
 
    // void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
 
    // void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
};

class ResolvedorQuery : public b2QueryCallback {
public:
    std::vector<b2Body*> foundBodies;
      
    bool ReportFixture(b2Fixture* fixture);
};

class Partida : public Thread {
    std::atomic<bool> finPartida;
    Queue<Accion> acciones;

    bool termino;
    MomentoDePartida momento;

    //World de box2d de la partida
    b2World world;
    ResolvedorColisiones colisiones;
    ResolvedorQuery query;
    //WARNING Actualmente solo usado para sacar los cuerpos creados en
    //las explosiones
    std::vector<b2Body *> cuerposADestruir;
    // ResolvedorDestruccion destucciones;

    std::string mapa;
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

    bool enviarEstadoAJugadores();

    [[nodiscard]] Accion obtenerAccion(Accion accionObtenida, bool obtuvoNueva,
		     Gusano* gusanoActual);

    [[nodiscard]] Gusano *anadirGusano(std::pair<coordX, coordY> coords);

    void anadirViga(radianes angulo, int longitud, std::pair<coordX, coordY> posicionInicial);

    void anadirOceano(std::pair<coordX, coordY> posicionInicial);

    void borrarCuerpos(); 

    void crearProjectil(Gusano *gusano, Ataque ataque, Proyectil* proyectil);

    std::pair<Gusano *, Jugador *> cambiarDeJugador(Jugador *jugadorTurnoActual, Gusano *gusanoActual, time_t tiempoActual, Proyectil *proyectil);

public:
    Partida(const std::string mapa);

    [[nodiscard]] InformacionInicial obtenerInfoInicial();

    void anadirCliente(Cliente *clienteNuevo);

    void gameLoop() override;
    void stop() override;

    ~Partida();
};

#endif
