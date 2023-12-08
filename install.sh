#!/bin/sh

# Dar permisos de ejecución antes de ejecutar!!
# $ chmod +x install.sh

# Verificar si se proporciona la ruta de instalación como parámetro
if [ $# -ne 1 ]; then
    echo "Uso: $0 <ruta_de_instalacion>"
    exit 1
fi

# Directorio para instalar las dependencias
DEPS_DIR="deps"
if [ -d "$DEPS_DIR" ]; then
    echo "La carpeta '$DEPS_DIR' ya existe. Eliminando contenido previo..."
    rm -rf "$DEPS_DIR" || {
        echo "Error al borrar el contenido de '$DEPS_DIR'. Por favor, verifica los permisos."
        exit 1
    }
fi

mkdir -p "$DEPS_DIR"
cd "$DEPS_DIR" || exit 1

# Función para mensajes informativos
print_status() {
    echo "=============================="
    echo "$1"
    echo "=============================="
}

# Función para manejar errores
handle_error() {
    echo "Error: $1"
    exit 1
}

print_status "Instalando dependencias del sistema..."
# Instalar dependencias del sistema usando el gestor de paquetes
if ! sudo apt-get update; then
    handle_error "Fallo al actualizar apt"
fi

if ! sudo apt-get install -y libjpeg-dev libpng-dev libfreetype-dev libopusfile-dev libflac-dev libxmp-dev libfluidsynth-dev libwavpack-dev cmake libmodplug-dev libsdl2-dev qtbase5-dev git; then
    handle_error "Fallo al instalar dependencias del sistema"
fi

# Instalación de SDL2
print_status "Instalando SDL2..."
git clone https://github.com/libsdl-org/SDL.git --branch release-2.26.5 || handle_error "Fallo al clonar SDL2"
cd SDL || handle_error "Fallo al acceder al directorio de SDL"
mkdir -p build && cd build || handle_error "Fallo al crear/construir el directorio de SDL2"
cmake .. || handle_error "Fallo al ejecutar CMake para SDL2"
make -j "$(nproc)" || handle_error "Fallo al construir SDL2"
sudo make install || handle_error "Fallo al instalar SDL2"
cd ../..

# Instalación de SDL2_image
print_status "Instalando SDL2_image..."
git clone git@github.com:libsdl-org/SDL_image.git --branch SDL2 || handle_error "Fallo al clonar SDL2_image"
cd SDL_image || handle_error "Fallo al acceder al directorio de SDL2_image"
mkdir -p build && cd build || handle_error "Fallo al crear/construir el directorio de SDL2_image"
cmake .. || handle_error "Fallo al ejecutar CMake para SDL2_image"
make -j $(nproc) || handle_error "Fallo al construir SDL2_image"
sudo make install || handle_error "Fallo al instalar SDL2_image"
cd ../..

# Instalación de SDL2_mixer
print_status "Instalando SDL2_mixer..."
git clone git@github.com:libsdl-org/SDL_mixer.git --branch SDL2 || handle_error "Fallo al clonar SDL2_mixer"
cd SDL_mixer || handle_error "Fallo al acceder al directorio de SDL2_mixer"
mkdir -p build && cd build || handle_error "Fallo al crear/construir el directorio de SDL2_mixer"
cmake .. || handle_error "Fallo al ejecutar CMake para SDL2_mixer"
make -j $(nproc) || handle_error "Fallo al construir SDL2_mixer"
sudo make install || handle_error "Fallo al instalar SDL2_mixer"
cd ../..

# Instalación de SDL2_ttf
print_status "Instalando SDL2_ttf..."
git clone git@github.com:libsdl-org/SDL_ttf.git --branch SDL2 || handle_error "Fallo al clonar SDL2_ttf"
cd SDL_ttf || handle_error "Fallo al acceder al directorio de SDL2_ttf"
mkdir -p build && cd build || handle_error "Fallo al crear/construir el directorio de SDL2_ttf"
cmake .. || handle_error "Fallo al ejecutar CMake para SDL2_ttf"
make -j $(nproc) || handle_error "Fallo al construir SDL2_ttf"
sudo make install || handle_error "Fallo al instalar SDL2_ttf"
cd ../..

# Instalación de libSDL2pp
print_status "Instalando libSDL2pp..."
git clone git@github.com:libSDL2pp/libSDL2pp.git --branch 0.18.1 || handle_error "Fallo al clonar libSDL2pp"
cd libSDL2pp || handle_error "Fallo al acceder al directorio de libSDL2pp"
mkdir -p build && cd build || handle_error "Fallo al crear/construir el directorio de libSDL2pp"
cmake .. || handle_error "Fallo al ejecutar CMake para libSDL2pp"
make -j $(nproc) || handle_error "Fallo al construir libSDL2pp"
sudo make install || handle_error "Fallo al instalar libSDL2pp"
cd ../..


# Instalación de box2d
print_status "Instalando box2d..."
git clone git@github.com:erincatto/box2d.git || handle_error "Fallo al clonar box2d"
cd box2d || handle_error "Fallo al acceder al directorio de box2d"
mkdir -p build && cd build || handle_error "Fallo al crear/construir el directorio de box2d"
cmake .. || handle_error "Fallo al ejecutar CMake para box2d"
make -j $(nproc) || handle_error "Fallo al construir box2d"
sudo make install || handle_error "Fallo al instalar box2d"
cd ../..

# Instalación de yaml-cpp
print_status "Instalando yaml-cpp..."
git clone git@github.com:jbeder/yaml-cpp.git || handle_error "Fallo al clonar yaml-cpp"
cd yaml-cpp || handle_error "Fallo al acceder al directorio de yaml-cpp"
mkdir build && cd build || handle_error "Fallo al crear/construir el directorio de yaml-cpp"
cmake .. || handle_error "Fallo al ejecutar CMake para yaml-cpp"
make -j $(nproc) || handle_error "Fallo al construir yaml-cpp"
sudo make install || handle_error "Fallo al instalar yaml-cpp"
cd ../..

# Instalación de catch2
print_status "Instalando catch2..."
git clone git@github.com:catchorg/Catch2.git || handle_error "Fallo al clonar Catch2"
cd Catch2 || handle_error "Fallo al acceder al directorio de Catch2"
mkdir -p build && cd build || handle_error "Fallo al crear/construir el directorio de Catch2"
cmake .. || handle_error "Fallo al ejecutar CMake para Catch2"
make -j $(nproc) || handle_error "Fallo al construir Catch2"
sudo make install || handle_error "Fallo al instalar Catch2"
cd ../..

# Instalación del juego
print_status "Instalando el juego..."
RUTA_INSTALACION="$1"
git clone git@github.com:Los-Socketes/Worms.git || handle_error "Fallo al clonar el repositorio del juego"
cd Worms || handle_error "Fallo al acceder al directorio del juego"
mkdir -p build && cd build || handle_error "Fallo al crear/construir el directorio del juego"
cmake .. || handle_error "Fallo al ejecutar CMake para el juego"
cmake --build . -j "$(nproc)" || handle_error "Fallo al construir el juego"
cd ../..

# Copiar la carpeta build compilada a la ruta de instalación
print_status "Copiando el juego a la ruta de instalación..."
cp -r Worms/build "$RUTA_INSTALACION" || handle_error "Fallo al copiar el juego a la ruta de instalación"

# Borrar directorio de dependencias
print_status "Borrando directorio de dependencias..."
cd ..
rm -rf "$DEPS_DIR" || handle_error "Fallo al borrar el directorio de dependencias"

# Actualizar la cache de librerías
sudo ldconfig

# Mensaje de éxito
echo "¡Instalación completada con éxito!"
