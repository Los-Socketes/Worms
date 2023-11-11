# Como compilar con CLANG y sobrevivir en el intento
EN TEORIA, lo *unico* que hay que hacer para compilar el tp es:
En este directorio van se deberia poder realizar la compilacion con Zig c++ (uuuuh, shiny!) en vez de Gcc.
La idea de esto es tener mas compiladores que nos puedan tirar mas warnings para poder atrapar los errores en tiempo de compilacion.
Zig propone cosas muy interesantes, vamos a ver que onda! Aparte, Andrew Kelley es un capo.

No necesitamos que compile para zig c++, con gcc sirve. Pero cuantas mas warnings tengamos mejor. Siempre se pueden ignorar.
Esto tambien implica que en el Cmakelists.txt vamos a tener que tener ifs dependiendo de si es clang o gcc o zig c++ (tal vez aplican los de clang). Eso es lo de menos.
Por ahora con algo facilon como esto anda!

0. Instalar Zig
Esto depende de distribucion a distribucion. 
A gusto de consumidor.

1. Ir al directorio buildClang
```console
cd buildZig/
```
2. Configurar el ambiente de cmake en el directorio actual
Esto es en dos pasos.
a. Obtener el path de Zig
Hay varias maneras de hacer esto. La tipica es:
```console
which zig
```
Esto escupe el path al ejecutable de clang++. Copiar para el paso b

b. Configurar especificando el compilador
```console
CXX=/path/que/dio/el/comando/which/zig c++ cmake ../
```
c. Chequear que quedo bien configurado el compilador
Si se configuro correctamente, deberia aparecer un mensaje que diga algo similar a:
> The CXX compiler identification is Clang X.X.X
(el nombre de la version puede cambiar)
Si sale GCC en vez de Clang, salio mal y hay que borrar el directorio buildZig y arrancar de 0

3. Correr el siguiente comando (compila el tp)
a. Compilar todo el tp
```console
cmake --build .
```
b. Compilar solo un target (reemplazar \<executable\> server o client)
```console
cmake --build . --target <executable>
```

4. Rezar
```
cat rezo
```
