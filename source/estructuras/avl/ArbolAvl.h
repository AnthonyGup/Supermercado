#ifndef ARBOL_AVL_H
#define ARBOL_AVL_H

#include "NodoAvl.h"
#include "pojo/Product.h"
#include "helpers/DotGenerator.h"
#include <string>
#include <sstream>
#include <map>

using namespace std;

class ArbolAvl {
    private:
        NodoAvl* raiz = nullptr;

        void liberarRecursivo(NodoAvl* nodo);
        NodoAvl* insertarNodoRecursivo(NodoAvl* nodo, Product* valor);
        void actualizarFe(NodoAvl* nodo);
        int obtenerAltura(NodoAvl* nodo);
        NodoAvl* buscarMaximo(NodoAvl* nodo);
        NodoAvl* eliminarNodoRecursivo(NodoAvl*, Product* valor);
        bool buscarNodoRecursivo(NodoAvl* nodo, const string& valor) const;

        //AVL
        NodoAvl* evaluarRotacion(NodoAvl* nodo);
        NodoAvl* evaluarDesbalance(NodoAvl* nodo);
        NodoAvl* rotarD(NodoAvl* nodo1, NodoAvl* nodo2);
        NodoAvl* rotarI(NodoAvl* nodo1, NodoAvl* nodo2);
        NodoAvl* rotarDI(NodoAvl* nodo1, NodoAvl* nodo2, NodoAvl* nodo3);
        NodoAvl* rotarID(NodoAvl* nodo1, NodoAvl* nodo2, NodoAvl* nodo3);

        bool dato1MayorDato2(const string& dato1, const string& dato2) const;
    public:
        ArbolAvl();
        ~ArbolAvl();
        
        void insertar(Product* valor);
        void eliminar(Product* valor);
        bool buscar(const string& nombre) const;
        Product* obtenerProducto(const string& nombre) const;
        
        // Generación de DOT para visualización
        bool generarDot(const string& filepath);

    private:
        // Método auxiliar recursivo para generar DOT
        void asignarIdsRecursivo(NodoAvl* nodo, int& contador, map<NodoAvl*, int>& mapIds);
        void agregarNodosAlGenerador(NodoAvl* nodo, DotGenerator& gen, const map<NodoAvl*, int>& mapIds);
        void agregarAristasAlGenerador(NodoAvl* nodo, DotGenerator& gen, const map<NodoAvl*, int>& mapIds);
};

#endif