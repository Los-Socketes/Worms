# Worms - Grupo 5

## Instrucciones de instalación:

### Requisitos previos

Instalación de librerías necesarias para la ejecución del programa:

```bash	
sudo apt-update
sudo apt-get install libjpeg-dev libpng-dev libfreetype-dev libopusfile-dev libflac-dev libxmp-dev libfluidsynth-dev libwavpack-dev cmake libmodplug-dev libsdl2-dev qt5base-dev
```

#### Instalación de SDL2

```bash
git clone git@github.com:libsdl-org/SDL.git --branch release-2.26.5
cd SDL
mkdir build
cd build
cmake ..
make -j4
sudo make install
cd ../..
```

#### Instalación de SDL2_image

```bash
git clone git@github.com:libsdl-org/SDL_image.git --branch SDL2
cd SDL_image
mkdir build
cd build
cmake ..
make -j4
sudo make install
cd ../..
```

#### Instalación de SDL2_mixer

```bash
git clone git@github.com:libsdl-org/SDL_mixer.git --branch SDL2
cd SDL_mixer
mkdir build
cd build
cmake ..
make -j4
sudo make install
cd ../..
```

#### Instalación de SDL2_ttf

```bash
git clone git@github.com:libsdl-org/SDL_ttf.git --branch SDL2
cd SDL_ttf
mkdir build
cd build
cmake ..
make -j4
sudo make install
cd ../..
```

#### Instalación de libSDL2pp

```bash
git clone git@github.com:libSDL2pp/libSDL2pp.git --branch 0.18.1
cd libSDL2pp
mkdir build
cd build
cmake ..
make -j4
sudo make install
cd ../..
```

#### Instalación de box2d

```bash
git clone git@github.com:erincatto/box2d.git
cd box2d
mkdir build
cd build
cmake ..
make -j4
sudo make install
cd ../..
```

#### Instalación de yaml-cpp

```bash
git clone git@github.com:jbeder/yaml-cpp.git
cd yaml-cpp
mkdir build
cd build
cmake ..
make -j4
sudo make install
cd ../..
```

#### Instalación de catch2

```bash
git clone git@github.com:catchorg/Catch2.git
cd Catch2
mkdir build
cd build
cmake ..
make -j4
sudo make install
cd ../..
```

### Instalación del juego

```bash
git clone git@github.com:Los-Socketes/Worms.git
cd Worms
mkdir build
cd build
cmake ..
make -j4
```

## Ejecución del juego

Sobre la carpeta build:

Ejecutar el servidor:

```bash
./server <puerto>
```

Ejecutar el cliente:

```bash
./client <ip> <puerto>
```





