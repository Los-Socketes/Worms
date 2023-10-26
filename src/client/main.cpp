#include <iostream>
#include <string>
#include <syslog.h>

#include "estadojuego.h"
#include "entradateclado.h"
#include "recibidor.h"
#include "enviador.h"
#include "../common/thread.h"
#include "../common/queue.h"

#define HOST 1
#define PUERTO 2
#define TAM_QUEUE 100

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Usar como: client <ip/hostname server> ";
        std::cout << "<puerto/servicename>" << std::endl;
        return 1;
    }
    try {
        bool continuar = true;
        EstadoJuego estado_juego(0, 0);

        // Instanciar Socket(argv[HOST], argv[PUERTO]) y
        // pasarselo al protocolo.

        // Colas de comunicación.
        Queue<EstadoJuego> recepcion_estados(TAM_QUEUE);
        Queue<std::string> envio_comandos(TAM_QUEUE);
        // Queue<std::string> comandos_teclado(TAM_QUEUE);

        // Hilo de entrada de teclado.
        // EntradaTeclado entrada_teclado(comandos_teclado, envio_comandos);

        // Hilos de envío y recepción.
        Enviador enviador(envio_comandos);
        Recibidor recibidor(recepcion_estados);

        // Iniciar hilos.
        // entrada_teclado.start();
        enviador.start();
        recibidor.start();

        // Loop principal.
        while (continuar) {
            // Recibir estado del juego (no bloqueante) y actualizar.
            recepcion_estados.try_pop(estado_juego);

            // TODO: Dibujar y renderizar.

            // TODO: Recibir comandos de la cola no bloqueante de comandos locales.
            // Por ahora solo leo del teclado.
            std::string comando;
            std::getline(std::cin, comando);
            if (comando == "estado") {
                std::cout << "Coord X: " << std::to_string(estado_juego.coord_x);
                std::cout << " Coord Y: " << std::to_string(estado_juego.coord_y) << std::endl;
            }
            else if (comando == "salir") {
                continuar = false;
            }
        }
        
        // comandos_teclado.close();
        // entrada_teclado.stop();
        // entrada_teclado.join();
        recepcion_estados.close();
        envio_comandos.close();
        enviador.stop();
        recibidor.stop();
        enviador.join();
        recibidor.join();
    } catch (const std::exception& e) {
        syslog(LOG_CRIT, "[Crit] Error: %s\n", e.what());
    } catch (...) {
        syslog(LOG_CRIT, "[Crit] Unknown error.\n");
    }
    return 0;
}
