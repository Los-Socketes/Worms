#ifndef SONIDO_H_
#define SONIDO_H_

#include <SDL2pp/SDL2pp.hh>

using namespace SDL2pp;

class Sonido {
 private:
    Mixer& mixer;
    Chunk sonido;
 public:
    Sonido(Mixer& mixer, const std::string& ruta);
    void reproducir();
};

#endif // SONIDO_H_