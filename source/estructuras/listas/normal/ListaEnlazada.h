#ifndef LISTA_ENLAZADA_H
#define LISTA_ENLAZADA_H

#include "Nodo/NodoList.h"
#include "excepciones/ProductoNoEncontradoException.h"

class ListaEnlazada {

    private:
        NodoList *cabeza = nullptr;
        NodoList *cola = nullptr;
        int size = 0;

        void desvincularNodo(NodoList *anterior, NodoList *actual);
    public:
        ListaEnlazada() = default;
        ~ListaEnlazada();

        bool estaVacia() const;
        int getSize() const;

        NodoList* buscarNodo(const string &nombre);
        void insertar(Product *dato);
        void eliminar(const string &nombre);
        void mostrar() const;
        void limpiar();

};

#endif