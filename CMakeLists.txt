cmake_minimum_required(VERSION 3.16)

project(supermercado LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(supermercado STATIC
    source/pojo/Producto.cpp
    source/excepciones/ProductoNoEncontradoException.cpp
    source/helpers/StringHelper.cpp
    source/helpers/CSVLoader.cpp
    source/estructuras/avl/NodoAvl.cpp
    source/estructuras/avl/ArbolAvl.cpp
    source/estructuras/listas/Nodo/NodoList.cpp
    source/estructuras/listas/normal/ListaEnlazada.cpp
    source/estructuras/listas/ordenada/ListaOrdenada.cpp
    source/estructuras/b/NodoB.cpp
    source/estructuras/b/ArbolB.cpp
    source/estructuras/bplus/NodoBPlus.cpp
    source/estructuras/bplus/ArbolBPlus.cpp
    source/frontend/Frontend.cpp
)

add_executable(supermercado_app
    source/Supermercado.cpp
)

target_link_libraries(supermercado_app PRIVATE supermercado)

target_include_directories(supermercado PUBLIC
    source
    source/estructuras/listas
    source/estructuras
    source/helpers
)

target_include_directories(supermercado_app PRIVATE
    source
    source/estructuras/listas
    source/estructuras
    source/helpers
)

if(MSVC)
    target_compile_options(supermercado PRIVATE /W4)
    target_compile_options(supermercado_app PRIVATE /W4)
else()
    target_compile_options(supermercado PRIVATE -Wall -Wextra)
    target_compile_options(supermercado_app PRIVATE -Wall -Wextra)
endif()
