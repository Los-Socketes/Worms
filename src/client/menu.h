#ifndef MENU_H_
#define MENU_H_

#include <iostream>
#include <string>
#include <vector>

#include "protocolo.h"
#include "defs.h"

// Clase que se encarga de mostrar el menu y de recibir la informacion inicial
// del juego desde el servidor una vez que se conecta a una partida.
class Menu {
 private:
    Protocolo& protocolo;
    bool salio;
 public:
    Menu(Protocolo& protocolo);

    InformacionInicial ejecutar(int argc, char* argv[], bool& es_host);

    // Indica si se salio desde el menu.
    bool salioDelMenu();
};

#endif // MENU_H_
