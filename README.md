Adrià Garriga Alonso (adria.garriga01@estudiant.upf.edu) 158636

WavePool
========

Primer proyecto para el curso de Infografía 2014. Una "piscina" de ondas senoidales.

Controles
---------

A 				- Añadir emisor bajo el cursor

D				- Borrar emisor seleccionado

Click			- Seleccionar un emisor

Arrastrar ratón	- Mover el emisor seleccionado

Q / Z			- Longitud de onda + / -

W / X			- Amplitud + / - (puede haber overflow de color)

E / C			- Velocidad de propagación + / -

P				- Pausar / continuar tiempo del emisor seleccionado

Compilación
-----------

Procedimiento estándar de CMake. Se necesitan SDL2 y OpenGL. Script de configuración
para SDL2 de CMake en el repositorio.

```bash
mkdir build
cd build
cmake-gui .. #now configure your variables
make
```

El ejecutable está en "<build_dir>/src/Wavepool", o en el directorio base si se corre
"make install".

Build comprobada con clang++ en FreeBSD y OS X y con g++ en GNU/Linux. Debería funcionar
en MSVC++ ya que CMake puede generar proyectos, pero no lo he probado. Aún así,
funciona mejor en compiladores con soporte para OpenMP (principalmente g++) ya que se
añade multithreading.

Problemas encontrados durante el desarrollo
-------------------------------------------

El problema principal de éste programa es la lentitud. Calcular la distancia a todos
los píxeles para cada onda senoidal es muy costoso, y calcular la onda senoidal no tanto
pero también. Así que lo cacheé en memoria, se utilizan un par de MB más de memoria pero
el procesador está bastante menos cargado. En cada frame sólo se debe comprobar el tiempo,
mirar la distancia en una tabla, y mirar el resultado en otra. Para cada píxel.

Por alguna razón, dibujar sólo el fondo sin ningún emisor también es bastante costoso.

También obtuve mejor rendimiento volviendo inline el método setPixel de Image, y poniendo
threads OpenMP en algunos fors particularmente largos (el de renderizar y el de recachear
ondas senoidales cuando se cambia el tamaño de la ventana).

Cambios en la última versión
----------------------------

- El overflow ya no suele ser un problema (se puede llegar al menos de -2^15 a 2^15-1, en
la mayoría de máquinas aún más)
- Puedes elegir el color de las ondas con el selector en la parte superior de la pantalla
- Puedes arrastrar y mover con un sólo botón, el esquema anterior era confuso.
- Los controles se ponen por pantalla al arrancar

- Había muchas conversiones de Uint a Float, han sido eliminadas
- Código compilado con MSVC++

