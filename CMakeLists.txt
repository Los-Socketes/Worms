 # Pongo esta version simplemente porque es la ultima version estable
 # en Gentoo (y es la que tengo instalada).
 # Entiendo que se podria cambiar por otra si llega a haber drama
cmake_minimum_required(VERSION 3.26.5)

# Cuando este listo ponemos la version VERSION 1.0 B)
project(worms VERSION 0.1 HOMEPAGE_URL
  https://github.com/Los-Socketes/Worms LANGUAGES CXX)

# Usamos el estandar iso 11 porque lo dice la consigna
set(CMAKE_CXX_STANDARD 11)

# Hace que genere el archivo compile_commands.json. Este es usado para
# algunos editores de texto, llamese Emacs. Ese archivo generado
# deberia estar en .gitignore
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
  # "${SERVERDIR}/recibidor.cpp"
  "${SERVERDIR}/server.cpp"
  "${SERVERDIR}/cliente.cpp"

  # Socket
  "${COMMONDIR}/resolvererror.cpp"
  "${COMMONDIR}/liberror.cpp"
  "${COMMONDIR}/resolver.cpp"
  "${COMMONDIR}/socket.cpp"

  #Threads
  "${COMMONDIR}/thread.h"

  #Thread safe queue
  "${COMMONDIR}/queue.h"
)

#Ejecutable del cliente
add_executable(client
  "${CLIENTDIR}/main.cpp"
)






# Notas a pie:
## Incluir archivo a archivo:
### 1: No soy FANATICO de que tengamos que poner cada uno de los
### archivos individualmente. Sin embargo, segun los creadores de
### cmake es la "mejor opcion". Fuente:
### https://cmake.org/cmake/help/latest/command/file.html#filesystem
