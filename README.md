WavePool
========

First project for the 2014 Infografia course. A 'pool' of sine wave emitters.

Building
========

Standard CMake procedure. You'll need SDL2 and OpenGL. CMake config script for
SDL2 is already provided.

```bash
mkdir build
cd build
cmake-gui .. #now configure your variables
make
```

The only installed file is the executable so you really don't need to specify an installation directory.
The executable is under src/WavePool in the build directory.

Build tested with clang++ on FreeBSD, OS X and with g++ on GNU/Linux, but it probably works with MSVC++,
Cygwin and MinGW.
