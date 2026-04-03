#ifndef ARBOL_B_PLUS_H
#define ARBOL_B_PLUS_H

#include "NodoBPlus.h"
#include "listas/normal/ListaEnlazada.h"
#include "helpers/DotGenerator.h"
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <unordered_set>

using namespace std;

class ArbolBPlus {

    protected:
        int orden;
        PPagina raiz;
        PPagina hojaInicio;     // Apunta a la primera hoja para escaneo secuencial

    public:
        explicit ArbolBPlus(int m = 3);
        ~ArbolBPlus();

        // Operaciones principales
        void crear();
        
        // Búsqueda de productos por categoría
        tipoValor buscar(const string& categoria);
        
        // Inserción de producto
        void insertar(Product* producto);
        
        // Eliminación de producto por categoría
        void eliminar(const string& categoria);

        // Listar todos los productos ordenados por categoría
        void listarPorCategoria();

        // Listar productos de una categoría específica
        void listarCategoria(const string& categoria);

        // Generacion de DOT para visualizacion
        bool generarDot(const string& filepath);

    private:
        // Métodos privados para búsqueda
        tipoValor buscar(PPagina nodo, const string& categoria);

        // Métodos privados para inserción
        void insertar(PPagina& nodo, Product* producto, bool esRaiz = false);
        void dividirNodo(PPagina nodoPadre, int indice, PPagina nodoLleno);
        void insertatEnNodoNoLleno(PPagina nodo, Product* producto);

        // Métodos auxiliares para generar DOT
        void asignarIdsRecursivo(PPagina nodo, int& contador, std::map<PPagina, int>& mapIds);
        void agregarNodosAlGenerador(PPagina nodo, DotGenerator& gen, const std::map<PPagina, int>& mapIds, std::unordered_set<PPagina>& visitados);
        void agregarAristasAlGenerador(PPagina nodo, DotGenerator& gen, const std::map<PPagina, int>& mapIds, std::unordered_set<PPagina>& visitados);

        // Métodos privados para eliminación
        void eliminar(PPagina nodo, const string& categoria);
        void llenarNodo(PPagina nodo, int pos);
        void fusionarNodos(PPagina nodo, int pos);
        void tomarDelAnterior(PPagina nodo, int pos);
        void tomarDelSiguiente(PPagina nodo, int pos);
};

#endif
