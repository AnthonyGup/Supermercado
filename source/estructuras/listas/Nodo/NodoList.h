#ifndef NODOLIST_H
#define NODOLIST_H

#include "pojo/Product.h"

class NodoList {
    private:
        NodoList *siguiente = nullptr;
        Product *dato = nullptr;
        int indice;

    public:
        NodoList(Product *dato);
        ~NodoList();

        // GETTERS
        NodoList* getSiguiente() const;
        Product* getDato() const;
        int getIndice() const;

        // SETTERS
        void setSiguiente(NodoList* siguiente);
        void setDato(Product* dato);
};

#endif