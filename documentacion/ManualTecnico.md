# Cliente

# Protocolo

# Server
El server contiene una clase "Server" que maneja todas las partidas de worms. Cada una de estas partidas se corre en un hilo propio (este hilo a su vez interactúa con otros hilos "hijos"). El conjunto de las partidas es manejado a través de un "Manejador de Partidas"; este se encarga de que destruir el hilo si la partida finalizo.
Cada partida de worms es manejada por una clase "Partida". Esta corre un método principal llamada "game loop" (en realidad, el método se llama "run", pero a través de macros se lo renombro para que fuese algo mas representativo). Cada persona que se conecta a una partida es asignada una clase Cliente, la cual tiene una clase Reciever y Sender. Estas ultimas son las que usa la partida para recibir y enviar información.
Cada partida tiene un conjunto de Jugadores y Gusanos. El gusano de turno actual realiza acciones que le llegan a través de una cola de acciones. Dicha cola recibe las acciones a través de los recievers de cada cliente. 
Los gusanos interactúan fundamentalmente con la clase Arma. Esta es una clase genérica que encapsula el comportamiento de todas las armas. Esta cambia sus parámetros dependiendo del arma elegida. La partida accede a dicha arma para determinar que tipo de disparo/ataque realizo el gusano.
