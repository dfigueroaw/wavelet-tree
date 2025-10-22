# Wavelet Tree

Una implementación del Wavelet Tree en C.

# Compilación y uso

El proyecto se puede compilar y testear con los siguientes comandos:

```bash
cmake -S . -B build                          # Generar archivos de compilación
cmake --build build                          # Compilar
ctest --test-dir build --output-on-failure   # Ejecutar tests
```

Si tu instalación de CMake no viene con `ctest`, puedes ejecutar directamente el
ejecutable de los tests:

```bash
build/tests/test_wavelet
```
