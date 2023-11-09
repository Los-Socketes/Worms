#include "TSList.h"
#include <iostream>

int main() {
    std::cout << "Inicio test de TSList\n";

    TSList<int> lista;

    #define numero 5
    lista.append(numero);
    int valor = lista.at(0);
    if (valor != numero)
        throw "ERROR: No accede al primer elemento correctamente\n";

    lista.remove(0);
    lista.append(numero);
    valor = lista.at(0);
    if (valor != numero)
        throw "ERROR: Borrar el elemento no actualiza las posiciones\n";
}
