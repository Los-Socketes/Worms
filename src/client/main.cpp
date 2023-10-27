#include <iostream>
#include <string>
#include <syslog.h>
#include <SDL2/SDL.h>

#include "estadojuego.h"
#include "entradateclado.h"
#include "recibidor.h"
#include "enviador.h"
#include "../common/thread.h"
#include "../common/queue.h"

#define HOST 1
#define PUERTO 2
#define TAM_QUEUE 500

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void renderizar(SDL_Window* ventana, EstadoJuego& estado_juego) {
    // Pantalla en blanco.
    SDL_Surface* pantalla = SDL_GetWindowSurface(ventana);
    SDL_FillRect(pantalla, NULL, SDL_MapRGB(pantalla->format, 0xFF, 0xFF, 0xFF));

    // Dibujo un punto rojo en la posición actual.
    int puntoX = estado_juego.coord_x;
    int puntoY = estado_juego.coord_y;

    SDL_Rect punto = {puntoX, puntoY, 10, 10};
    SDL_FillRect(pantalla, &punto, SDL_MapRGB(pantalla->format, 0xFF, 0x00, 0x00));

    // Actualizo ventana.
    SDL_UpdateWindowSurface(ventana);
}
int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Usar como: client <ip/hostname server> ";
        std::cout << "<puerto/servicename>" << std::endl;
        return 1;
    }
    try {
        bool continuar = true;
        EstadoJuego estado_juego(0, 0);

        // Inicializar SDL.        
        if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
            printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
            return 1;
        }

        SDL_Window* ventana = SDL_CreateWindow( "Worms", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

        if (ventana == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            return 1;
        }

        // TODO: Instanciar Socket(argv[HOST], argv[PUERTO]) y
        // pasarselo al protocolo.

        // Colas de comunicación.
        Queue<EstadoJuego> recepcion_estados(TAM_QUEUE);
        Queue<std::string> envio_comandos(TAM_QUEUE);
        Queue<std::string> comandos_teclado(TAM_QUEUE);

        // Hilo de entrada de teclado.
        EntradaTeclado entrada_teclado(envio_comandos, comandos_teclado);

        // Hilos de envío y recepción.
        Enviador enviador(envio_comandos);
        Recibidor recibidor(recepcion_estados);

        // Iniciar hilos.
        entrada_teclado.start();
        enviador.start();
        recibidor.start();

        // Loop principal.
        while (continuar) {

            // Eventos de SDL.
            // Por ahora solo chequeo el evento de cerrar la ventana.
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    continuar = false;
                }
            }

            // Actualizo el estado del juego.
            recepcion_estados.try_pop(estado_juego);

            // Renderizo.
            renderizar(ventana, estado_juego);

            // Chequeo comandos de teclado.
            std::string comando;
            if (comandos_teclado.try_pop(comando)) {
                if (comando == "q") {
                    continuar = false;
                }
            }
            
            // Delay para controlar el framerate.
            // TODO: implementar constant rate loop.
            SDL_Delay(1000 / 60);
        }
        
        // Cerrar todo.
        comandos_teclado.close();
        entrada_teclado.stop();
        entrada_teclado.join();
        recepcion_estados.close();
        envio_comandos.close();
        enviador.stop();
        recibidor.stop();
        enviador.join();
        recibidor.join();

        SDL_DestroyWindow(ventana);
        SDL_Quit();
    } catch (const std::exception& e) {
        syslog(LOG_CRIT, "[Crit] Error: %s\n", e.what());
    } catch (...) {
        syslog(LOG_CRIT, "[Crit] Unknown error.\n");
    }
    return 0;
}
