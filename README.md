Adri� Garriga Alonso (adria.garriga01@estudiant.upf.edu) 158636

WavePool
========

Primer proyecto para el curso de Infograf�a 2014. Una "piscina" de ondas senoidales.

Controles
---------

A 				- A�adir emisor bajo el cursor

D				- Borrar emisor seleccionado

Click			- Seleccionar un emisor

Arrastrar rat�n	- Mover el emisor seleccionado

Q / Z			- Longitud de onda + / -

W / X			- Amplitud + / - (puede haber overflow de color)

E / C			- Velocidad de propagaci�n + / -

P				- Pausar / continuar tiempo del emisor seleccionado

Compilaci�n
-----------

Procedimiento est�ndar de CMake. Se necesitan SDL2 y OpenGL. Script de configuraci�n
para SDL2 de CMake en el repositorio.

```bash
mkdir build
cd build
cmake-gui .. #now configure your variables
make
```

El ejecutable est� en "<build_dir>/src/Wavepool", o en el directorio base si se corre
"make install".

Build comprobada con clang++ en FreeBSD y OS X y con g++ en GNU/Linux. Deber�a funcionar
en MSVC++ ya que CMake puede generar proyectos, pero no lo he probado. A�n as�,
funciona mejor en compiladores con soporte para OpenMP (principalmente g++) ya que se
a�ade multithreading.

Problemas encontrados durante el desarrollo
-------------------------------------------

El problema principal de �ste programa es la lentitud. Calcular la distancia a todos
los p�xeles para cada onda senoidal es muy costoso, y calcular la onda senoidal no tanto
pero tambi�n. As� que lo cache� en memoria, se utilizan un par de MB m�s de memoria pero
el procesador est� bastante menos cargado. En cada frame s�lo se debe comprobar el tiempo,
mirar la distancia en una tabla, y mirar el resultado en otra. Para cada p�xel.

Por alguna raz�n, dibujar s�lo el fondo sin ning�n emisor tambi�n es bastante costoso.

Tambi�n obtuve mejor rendimiento volviendo inline el m�todo setPixel de Image, y poniendo
threads OpenMP en algunos fors particularmente largos (el de renderizar y el de recachear
ondas senoidales cuando se cambia el tama�o de la ventana).

Cambios en la �ltima versi�n
----------------------------

- El overflow ya no suele ser un problema (se puede llegar al menos de -2^15 a 2^15-1, en
la mayor�a de m�quinas a�n m�s)
- Puedes elegir el color de las ondas con el selector en la parte superior de la pantalla
- Puedes arrastrar y mover con un s�lo bot�n, el esquema anterior era confuso.
- Los controles se ponen por pantalla al arrancar

- Hab�a muchas conversiones de Uint a Float, han sido eliminadas
- C�digo compilado con MSVC++

