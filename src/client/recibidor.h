#ifndef RECIBIDOR_H_
#define RECIBIDOR_H_

#include <atomic>
#include <syslog.h>
#include <SDL2/SDL.h>

#include "estadojuego.h"
#include "../common/thread.h"
#include "../common/queue.h"

class Recibidor: public Thread {
private:
    Queue<EstadoJuego>& recepcion_estados;
    std::atomic<bool> cont;

public:
    Recibidor(Queue<EstadoJuego>& recepcion_estados);

    // Hilo que se encarga de recibir los estados del juego.
    void run() override;

    // Para cerrar el hilo.
    void stop();

};

#endif  // RECIBIDOR_H_
