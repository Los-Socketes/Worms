# Como compilar y sobrevivir en el intento
EN TEORIA, lo *unico* que hay que hacer para compilar el tp es:

1. Ir al directorio buil
```console
cd build/
```
2. Correr el siguiente comando (magia de cmake)
```console
cmake ../
```
3. Correr el siguiente comando (compila el tp)
a. Compilar todo el tp
```console
cmake --build . -- -k
```
b. Compilar solo un target (reemplazar \<executable\> server o client)
```console
cmake --build . --target <executable>
```

4. Rezar
```
cat rezo
```
