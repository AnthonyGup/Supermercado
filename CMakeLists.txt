cmake_minimum_required(VERSION 3.16)

project(supermercado LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(supermercado STATIC
    source/pojo/Producto.cpp
    source/excepciones/ProductoNoEncontradoException.cpp
    source/helpers/StringHelper.cpp
    source/estructuras/avl/NodoAvl.cpp
    source/estructuras/avl/ArbolAvl.cpp
    source/estructuras/listas/Nodo/NodoList.cpp
    source/estructuras/listas/normal/ListaEnlazada.cpp
    source/estructuras/listas/ordenada/ListaOrdenada.cpp
)

add_executable(supermercado_app
    source/Supermercado.cpp
)

target_link_libraries(supermercado_app PRIVATE supermercado)

target_include_directories(supermercado PUBLIC
    source
    source/estructuras/listas
)

if(MSVC)
    target_compile_options(supermercado PRIVATE /W4)
else()
    target_compile_options(supermercado PRIVATE -Wall -Wextra)
endif()
