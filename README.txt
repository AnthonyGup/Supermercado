Sistema de gestion de supermercado

Este proyecto implementa un sistema de gestion de productos con varias estructuras de datos para comparar su comportamiento en busqueda, insercion, eliminacion y exportacion visual.

Estructura del proyecto

source/
  Codigo fuente principal del sistema.

build/
  Carpeta generada por CMake para compilacion.

output/
  Directorio donde se generan los archivos .dot.

Archivos de datos

productos_ejemplo.csv
  Archivo de prueba para validacion rapida.

productos_1000.csv
  Archivo de carga masiva con 1000 productos o mas, segun la version incluida.

Compilacion con CMake

Compilar en Windows

1. Abrir una terminal en la carpeta raiz del proyecto.
2. Configurar el proyecto:

cmake -S . -B build

3. Compilar el ejecutable:

cmake --build build --config Release

4. Ejecutar el programa (Visual Studio y generadores multi-config):

build\Release\supermercado_app.exe

Si tu generador produce otra ruta, busca supermercado_app.exe dentro de build.

Compilar en Linux

1. Instalar herramientas basicas (ejemplo Ubuntu/Debian):

sudo apt update
sudo apt install -y build-essential cmake

2. Entrar a la carpeta del proyecto.

3. Configurar en modo Release:

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

4. Compilar:

cmake --build build -j

5. Ejecutar:

./build/supermercado_app

Notas para Linux

- En Linux normalmente se usa generador single-config (Makefiles o Ninja), por eso se pasa -DCMAKE_BUILD_TYPE=Release en configuracion.
- Si usas Ninja, tambien funciona:

cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/supermercado_app

Ejecucion

Al iniciar, el programa muestra un menu con estas opciones:

1. Insertar producto manualmente.
2. Cargar productos desde un CSV.
3. Eliminar producto.
4. Buscar producto.
5. Mostrar listas.
6. Exportar arboles a .dot.
7. Medir latencias de busqueda.
8. Salir.

Notas importantes

- El CSV debe incluir nombre, codigo de barras, categoria, fecha de vencimiento, marca, precio y stock.
- La fecha se valida con formato YYYY-MM-DD.
- La exportacion a .dot permite generar imagenes con Graphviz.
- El programa registra errores de carga en error.log.

Salida visual

Los arboles AVL, B y B+ pueden exportarse a archivos .dot dentro de la carpeta output. Luego esos archivos se convierten a PNG con Graphviz.

Ejemplo de conversion a imagen:

dot -Tpng output\arbol_avl.dot -o output\arbol_avl.png

Objetivo del proyecto

Comparar estructuras de datos clasicas para administracion de productos y observar diferencias practicas entre listas y arboles en operaciones de busqueda, insercion y mantenimiento de orden.