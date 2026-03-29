#include "ArbolAvl.h"
#include "helpers/StringHelper.h"

NodoAvl* ArbolAvl::insertarNodoRecursivo(NodoAvl* nodo, Product* valor) {
    if (nodo == nullptr) {
        return new NodoAvl(valor);
    }

    if (!dato1MayorDato2(nodo->getDato()->getName(), valor->getName())) {
        nodo->setDerecho(insertarNodoRecursivo(nodo->getDerecho(), valor));
    } else {
        nodo->setIzquierdo(insertarNodoRecursivo(nodo->getIzquierdo(), valor));
    }
    return nodo;
}

void ArbolAvl::actualizarFe(NodoAvl* nodo) {
    if (nodo != nullptr) {
        nodo->setFe(obtenerAltura(nodo->getDerecho()) - obtenerAltura(nodo->getIzquierdo()));
    }
}

int ArbolAvl::obtenerAltura(NodoAvl* nodo) {
    if (nodo == nullptr) return 0;

    return 1 + max(obtenerAltura(nodo->getIzquierdo()), obtenerAltura(nodo->getDerecho()));
}

NodoAvl* ArbolAvl::buscarMaximo(NodoAvl* nodo) {
    NodoAvl* actual = nodo;
    while(actual && actual->getDerecho() != nullptr) {
        actual = actual->getDerecho();
    }
    return actual;
}

NodoAvl* ArbolAvl::eliminarNodoRecursivo(NodoAvl* nodo, Product* valor) {
    if(nodo == nullptr) return nullptr;

    if (dato1MayorDato2(nodo->getDato()->getName(), valor->getName())) {
        nodo->setIzquierdo(eliminarNodoRecursivo(nodo->getIzquierdo(), valor));
    } else if (dato1MayorDato2(valor->getName(), nodo->getDato()->getName())) {
        nodo->setDerecho(eliminarNodoRecursivo(nodo->getDerecho(), valor));
    } else {
        if (nodo->getIzquierdo() == nullptr) {
            NodoAvl* temp = nodo->getDerecho();
            delete nodo;
            return temp;
        } else if (nodo->getDerecho() == nullptr) {
            NodoAvl* temp = nodo->getIzquierdo();
            delete nodo;
            return temp;
        }

        NodoAvl* temp = buscarMaximo(nodo->getIzquierdo());
        nodo->setDato(temp->getDato());
        nodo->setIzquierdo(eliminarNodoRecursivo(nodo->getIzquierdo(), temp->getDato()));
    }
    return nodo;
}

bool ArbolAvl::buscarNodoRecursivo(NodoAvl* nodo, string& valor) {
    if(nodo == nullptr) return false;

    if (!dato1MayorDato2(nodo->getDato()->getName(), valor) &&
        !dato1MayorDato2(valor, nodo->getDato()->getName())) {
        return true;
    }

    if(dato1MayorDato2(nodo->getDato()->getName(), valor)) {
        return buscarNodoRecursivo(nodo->getIzquierdo(), valor);
    } else {
        return buscarNodoRecursivo(nodo->getDerecho(), valor);
    }
}

bool ArbolAvl::dato1MayorDato2(const string& dato1, const string& dato2) const {
    return StringHelper::toLowerCase(dato1) > StringHelper::toLowerCase(dato2);
}

ArbolAvl::ArbolAvl() : raiz(nullptr) {}

void ArbolAvl::insertar(Product* valor) {
    raiz = insertarNodoRecursivo(raiz, valor);
}

void ArbolAvl::eliminar(Product* valor) {
    raiz = eliminarNodoRecursivo(raiz, valor);
}

void ArbolAvl::buscar(string& nombre) {
    buscarNodoRecursivo(raiz, nombre);
}

