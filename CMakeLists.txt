 # Pongo esta version simplemente porque es la ultima version estable
 # en Gentoo (y es la que tengo instalada).
 # Entiendo que se podria cambiar por otra si llega a haber drama
cmake_minimum_required(VERSION 3.25.1)

# Cuando este listo ponemos la version VERSION 1.0 B)
project(worms VERSION 0.1 HOMEPAGE_URL
  https://github.com/Los-Socketes/Worms LANGUAGES CXX)

# Usamos el estandar iso 17 porque lo dice la consigna
set(CMAKE_CXX_STANDARD 17)

# Catch2 tiene versiones relativamente distintas entre Gentoo y Debian
# Como solo usamos la libreria para correr tests, vamos a usar una
# version u otra dependiendo de la distro. Los includes van a ser
# reemplazados con macros
set (grep_cmd "grep")
set (grep_arg "^NAME=")
set (grep_file "/etc/os-release")
execute_process(COMMAND ${grep_cmd} ${grep_arg} ${grep_file}
    OUTPUT_VARIABLE infoOS
)
string(REPLACE NAME= "" infoOS ${infoOS})
string(REPLACE \n "" infoOS ${infoOS})

string(COMPARE EQUAL ${infoOS} "Gentoo" esGentoo)
string(COMPARE EQUAL ${infoOS} "\"Debian GNU/Linux\"" esDebian)

# Anadimos debug symbols
if(${esGentoo})
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Busco codigo para el sdl
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer 2.6.3 REQUIRED)
find_package(SDL2pp REQUIRED)

if(${esGentoo}) 
  message("Estas en " ${infoOS})
  find_package(Catch2 3.4.0 REQUIRED)
elseif(${esDebian})
  message("Estas en " ${infoOS})
  find_package(Catch2 2.13.10 REQUIRED)
endif()

#Incluimos codigo de box2d
find_package(box2d REQUIRED)

# Seteamos las flags de compilacion
add_compile_options(
  
  # Queremos todos las advertencias y errores posibles
  -Wall -Wextra -Wpedantic

  # Queremos que los warnings sean errors
  -Werror

  # Seteamos esta flag porque muchas veces dejamos algo "en camino"
  # TODO Sacar estos Wno ASAP
  -Wno-unused-variable
  -Wno-unused-parameter
  -Wno-unused-but-set-variable

 )


# Incluye CTest para poder correr tests
include(CTest)

# Hace que genere el archivo compile_commands.json. Este es usado para
# algunos editores de texto, llamese Emacs o programas como
# include-what-you-use. Ese archivo generado deberia estar en .gitignore
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Creamos unas variables para facilitar el uso de directorios en el
# cmake
set(SERVERDIR src/server)
set(COMMONDIR src/common)
set(CLIENTDIR src/client)

# Hacemos que el directorio common sea el unico directorio de donde
# se puede incluir (sin contar el directorio propio). 
include_directories("${COMMONDIR}")


# Creacion de los ejecutables -- Notas Incluir archivo a archivo.
## Ejecutable del server
add_executable(server
  "${SERVERDIR}/main.cpp"
  "${SERVERDIR}/server.cpp"
  "${SERVERDIR}/cliente.cpp"
  "${SERVERDIR}/jugador.cpp"
  "${SERVERDIR}/partida.cpp"
  "${SERVERDIR}/gusano.cpp"
  "${SERVERDIR}/sender.cpp"
  "${SERVERDIR}/reciever.cpp"
  "${SERVERDIR}/aceptador.cpp"
  "${SERVERDIR}/monitorPartida.cpp"

  # Protocolo
  "${COMMONDIR}/protocolo.cpp"

  # Socket
  "${COMMONDIR}/resolvererror.cpp"
  "${COMMONDIR}/liberror.cpp"
  "${COMMONDIR}/resolver.cpp"
  "${COMMONDIR}/socket.cpp"

  #Threads
  "${COMMONDIR}/thread.h"

  #Thread safe queue
  "${COMMONDIR}/queue.h"
  
  #Armas
)
## Macros para pasarle al server
target_compile_definitions(server PUBLIC
  SERVER
)

# Linkeo box2d
target_include_directories(server PRIVATE ${box2d_INCLUDE_DIRS})
target_link_libraries(server PRIVATE box2d::box2d)

#Ejecutable del cliente
add_executable(client
  "${CLIENTDIR}/main.cpp"
  "${CLIENTDIR}/entradateclado.cpp"
  "${CLIENTDIR}/enviador.cpp"
  "${CLIENTDIR}/recibidor.cpp"
  "${CLIENTDIR}/cliente.cpp"
  "${CLIENTDIR}/menu.cpp"
  "${CLIENTDIR}/animacion.cpp"
  "${CLIENTDIR}/camara.cpp"
  "${CLIENTDIR}/accioncliente.cpp"

  # Socket
  "${COMMONDIR}/resolvererror.cpp"
  "${COMMONDIR}/liberror.cpp"
  "${COMMONDIR}/resolver.cpp"
  "${COMMONDIR}/socket.cpp"

  #Threads
  "${COMMONDIR}/thread.h"

  #Thread safe queue
  "${COMMONDIR}/queue.h"

  # Protocolo
  "${COMMONDIR}/protocolo.cpp"
)
## Macros para pasarle al client
target_compile_definitions(client PUBLIC
  CLIENT
)

# Linkeo de las librerias SDL
target_include_directories(client PRIVATE ${SDL2_INCLUDE_DIRS})
target_include_directories(client PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS})
target_include_directories(client PRIVATE ${SDL2_MIXER_INCLUDE_DIRS})
target_include_directories(client PRIVATE ${SDL2PP_INCLUDE_DIRS})
target_link_libraries(client PRIVATE SDL2::SDL2)
target_link_libraries(client PRIVATE SDL2_image::SDL2_image)
target_link_libraries(client PRIVATE SDL2_mixer::SDL2_mixer)
target_link_libraries(client PRIVATE SDL2pp::SDL2pp)

# Copiamos los assets a la carpeta de build
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

## TESTS
# Creamos unas variables para facilitar el uso de directorios en el
# cmake
set(SERVERTESTDIR test/server)
set(COMMONTESTDIR test/common)
set(CLIENTTESTDIR test/client)

# Ejecutamos los tests
## TESTS DE PROTOCOLO
add_executable(testprotocolo
  "${COMMONTESTDIR}/protocolo.cpp"

  "${COMMONDIR}/protocolo.cpp"
  
  # Socket
  "${COMMONDIR}/resolvererror.cpp"
  "${COMMONDIR}/liberror.cpp"
  "${COMMONDIR}/resolver.cpp"
  "${COMMONDIR}/socket.cpp"
)
## Macros para pasarle al test
target_compile_definitions(testprotocolo PUBLIC
  CLIENT
)
target_compile_definitions(testprotocolo PUBLIC
  SERVER
)
if(${esGentoo})
  target_compile_definitions(testprotocolo PUBLIC
    DISTRO=1
  )
elseif(${esDebian})
  target_compile_definitions(testprotocolo PUBLIC
    DISTRO=2
  )
endif()

add_test(NAME protocolotest
  COMMAND testprotocolo
)
target_link_libraries(testprotocolo PRIVATE Catch2::Catch2WithMain)

## TESTS DE THREAD SAFE LIST

# TODO: Cambiar estos ejecutables por los tests de cmake
add_executable(testthreadsafelist
  "${COMMONTESTDIR}/tslist.cpp"
)
add_test(NAME threadsafelisttest
  COMMAND testthreadsafelist
)


# Notas a pie:
## Incluir archivo a archivo:
### 1: No soy FANATICO de que tengamos que poner cada uno de los
### archivos individualmente. Sin embargo, segun los creadores de
### cmake es la "mejor opcion". Fuente:
### https://cmake.org/cmake/help/latest/command/file.html#filesystem

