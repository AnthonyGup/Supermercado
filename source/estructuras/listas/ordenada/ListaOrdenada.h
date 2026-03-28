#ifndef LISTA_ORDENADA_H
#define LISTA_ORDENADA_H

#include "Nodo/NodoList.h"
#include "excepciones/ProductoNoEncontradoException.h"

class ListaOrdenada {

    private:
        NodoList *cabeza = nullptr;
        NodoList *cola = nullptr;
        int size = 0;

        void limpiar();
        void desvincularNodo(NodoList *anterior, NodoList *actual);

    public:
        ListaOrdenada() = default;
        ~ListaOrdenada();

        bool estaVacia() const;
        int getSize() const;

        NodoList* buscarNodo(const string &nombre);
        void insertar(Product *dato);
        void eliminar(const string &nombre);
        void mostrar() const;

};

#endif