#include "sonido.h"

Sonido::Sonido(Mixer& mixer, const std::string& ruta) :
    mixer(mixer),
    sonido(ruta) {}

void Sonido::reproducir() {
    int disponible = mixer.GetGroupAvailableChannel(0);
    if (disponible > 0) {
        mixer.PlayChannel(disponible, sonido, 0);
    }
}