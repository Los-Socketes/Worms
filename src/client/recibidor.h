#ifndef RECIBIDOR_H_
#define RECIBIDOR_H_

#include <atomic>
#include <syslog.h>
#include <SDL2pp/SDL2pp.hh>

#include "thread.h"
#include "queue.h"
#include "protocolo.h"
#include "defs.h"
#include "accionCliente.h"

 // Hilo que se encarga de recibir los estados del juego.
class Recibidor: public Thread {
private:
    Protocolo& protocolo;
    Queue<std::shared_ptr<EstadoDelJuego>>& recepcion_estados;
    // Queue<std::shared_ptr<AccionCliente>>& envio_comandos;
    std::shared_ptr<EstadoDelJuego> estado_juego;
    std::atomic<bool> cont;
    idJugador id_jugador;
    std::atomic<bool>& es_turno;
    std::atomic<bool>& municiones_agotadas;

public:
    Recibidor(Protocolo& protocolo, Queue<std::shared_ptr<EstadoDelJuego>>& recepcion_estados, std::atomic<bool>& es_turno, std::atomic<bool>& municiones_agotadas);
    // Recibidor(Protocolo& protocolo, Queue<std::shared_ptr<EstadoDelJuego>>& recepcion_estados, Queue<std::shared_ptr<AccionCliente>>& envio_comandos, std::atomic<bool>& es_turno);

    void run() override;

    // Setea el id del jugador.
    void setIdJugador(id id_jugador);

    // Para cerrar el hilo.
    void stop() override;

};

#endif  // RECIBIDOR_H_
