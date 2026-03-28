#include "NodoList.h"

NodoList::NodoList(Product *dato) {
    this->dato = dato;
}

NodoList::~NodoList() {
    if (dato != nullptr) {
        delete dato;
        dato = nullptr;
    }
    siguiente = nullptr;
}

// GETTERS
NodoList* NodoList::getSiguiente() const {
    return siguiente;
}

Product* NodoList::getDato() const {
    return dato;
}

int NodoList::getIndice() const {
    return this->indice;
}

// SETTERS
void NodoList::setSiguiente(NodoList* siguiente) {
    this->siguiente = siguiente;
}

void NodoList::setDato(Product* dato) {
    this->dato = dato;
}
