#include "sonido.h"

Sonido::Sonido(Mixer& mixer, const std::string& ruta) :
    mixer(mixer),
    sonido(ruta) {}

void Sonido::reproducir() {
    mixer.PlayChannel(-1, sonido, 0);
}