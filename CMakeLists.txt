cmake_minimum_required(VERSION 3.16)

project(supermercado LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Para generadores de una sola configuracion (Makefiles/Ninja),
# usar Release por defecto si el usuario no define otra.
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING "Build type" FORCE)
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release RelWithDebInfo MinSizeRel)

option(SUPERMERCADO_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

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

set(SUPERMERCADO_INCLUDE_DIRS
    source
    source/estructuras/listas
    source/estructuras
    source/helpers
)

target_include_directories(supermercado PUBLIC
    ${SUPERMERCADO_INCLUDE_DIRS}
)

target_include_directories(supermercado_app PRIVATE
    ${SUPERMERCADO_INCLUDE_DIRS}
)

function(supermercado_enable_warnings target_name)
    target_compile_options(${target_name} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall;-Wextra;-Wpedantic>
    )

    if(SUPERMERCADO_WARNINGS_AS_ERRORS)
        target_compile_options(${target_name} PRIVATE
            $<$<CXX_COMPILER_ID:MSVC>:/WX>
            $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Werror>
        )
    endif()
endfunction()

supermercado_enable_warnings(supermercado)
supermercado_enable_warnings(supermercado_app)
