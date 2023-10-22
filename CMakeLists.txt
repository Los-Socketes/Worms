 # Pongo esta version simplemente porque es la ultima version estable
 # en Gentoo (y es la que tengo instalada).
 # Entiendo que se podria cambiar por otra si llega a haber drama
cmake_minimum_required(VERSION 3.26.5)

# Cuando este listo ponemos la version VERSION 1.0 B)
project(worms VERSION 0.1 HOMEPAGE_URL https://github.com/Los-Socketes/Worms LANGUAGES CXX)

# Usamos el estandar iso 11 porque lo dice la consigna
set(CMAKE_CXX_STANDARD 11)

# Incluimos los directorios con el codigo fuente
# include_directories(src/server)

add_executable(server src/server/main.cpp)
