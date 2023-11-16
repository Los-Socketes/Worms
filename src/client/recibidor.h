#ifndef RECIBIDOR_H_
#define RECIBIDOR_H_

#include <atomic>
#include <syslog.h>
#include <SDL2pp/SDL2pp.hh>

#include "thread.h"
#include "queue.h"
#include "protocolo.h"

class Recibidor: public Thread {
private:
    Protocolo& protocolo;
    Queue<std::shared_ptr<EstadoDelJuego>>& recepcion_estados;
    std::shared_ptr<EstadoDelJuego> estado_juego;
    std::atomic<bool> cont;

public:
    Recibidor(Protocolo& protocolo, Queue<std::shared_ptr<EstadoDelJuego>>& recepcion_estados);

    // Hilo que se encarga de recibir los estados del juego.
    void run() override;

    // Para cerrar el hilo.
    void stop() override;

};

#endif  // RECIBIDOR_H_
