#ifndef MENU_H_
#define MENU_H_

#include <iostream>
#include <string>
#include <vector>

#include "protocolo.h"
#include "defs.h"

class Menu {
 private:
    Protocolo& protocolo;
 public:
    Menu(Protocolo& protocolo);

    InformacionInicial ejecutar(int argc, char* argv[]);
};

#endif // MENU_H_
