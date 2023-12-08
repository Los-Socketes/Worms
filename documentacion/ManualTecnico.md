# Cliente

# Protocolo

# Server
El server contiene una clase "Server" que inicia al aceptador y espera a que se escriba por la terminal "q" para cerrar el programa. Cada una de estas partidas se corre en un hilo propio. El conjunto de las partidas es manejado a través de un "Manejador de Partidas"; este se encarga de que crear nuevas partidas y destruir el hilo/ partida si la partida finalizó.\
Cada partida de worms es manejada por una clase "Partida". Esta corre un método principal llamada "game loop" (en realidad, el método se llama "run", pero a través de macros se lo renombro para que fuese algo mas representativo). Cada persona que se conecta a una partida es asignada una clase Cliente, la cual tiene una clase Reciever y Sender. Estas ultimas son las que usa la partida para recibir y enviar información en el struct de "EstadoDelJuego".\
Cada partida tiene además un conjunto de Jugadores y Gusanos (los jugadores tienen gusanos asociados para saber su estado en la partida). El gusano de turno actual realiza acciones que le llegan a través de una cola de acciones. Dicha cola recibe las acciones a través de los recievers de cada cliente.\
Los gusanos interactúan fundamentalmente con la clase Arma. Esta es una clase genérica que encapsula el comportamiento de todas las armas la cual cambia sus parámetros dependiendo del arma elegida. La partida accede a dicha arma para determinar que tipo de disparo/ataque realizo el gusano y características de cada arma.

## Clases implementadas
- Server: Inicia el programa y espera una "q" para terminar.
- Partida: 
- Sender:
- Reciever:
- Provision:
- Gusano:
- Jugador:
- Cliente:
- Monitor de partida:
- Configuración server:
- Aceptador:
- Arma:
- Mapa:

