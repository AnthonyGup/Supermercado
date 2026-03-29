#include "ArbolAvl.h"
#include "helpers/StringHelper.h"
#include "excepciones/ProductoNoEncontradoException.h"
#include <algorithm>

/**
 * Metodo que inserta nuevos productos en el arbol, lo hace recursivamente hasta encontrar el lugar para el producto agregado,
 * se comprueba luego el fe de cada nodo y si esta desbalanceado se llama a las rotaciones para corregir el desbalanceo,
 * tambien actualiza el factor de equilibro de cada nodo evaluado.
 * @param nodo es el nodo que se va a evaluar, si es vacio entonces se crea uno se setea el producto y se devuelve
 * @param valor es el producto que se quiere insertar
 * @return el nodo creado si era null y si no, el nodo con sus hijos ya seteados
 */
NodoAvl* ArbolAvl::insertarNodoRecursivo(NodoAvl* nodo, Product* valor) {
    if (nodo == nullptr) {
        NodoAvl* nuevoNodo = new NodoAvl(valor);
        actualizarFe(nuevoNodo);
        return nuevoNodo;
    }

    if (dato1MayorDato2(valor->getName(), nodo->getDato()->getName())) {
        nodo->setDerecho(insertarNodoRecursivo(nodo->getDerecho(), valor));
    } else {
        nodo->setIzquierdo(insertarNodoRecursivo(nodo->getIzquierdo(), valor));
    }
    actualizarFe(nodo);
    int fe = nodo->getFe();
    if(fe > 1 || fe < (-1)) {
        nodo = evaluarRotacion(nodo);
    }
    return nodo;
}

NodoAvl* ArbolAvl::evaluarRotacion(NodoAvl* nodo) {
    NodoAvl* nodo2 = evaluarDesbalance(nodo);
    if (nodo2 == nullptr) {
        return nodo;
    }

    if(nodo->getFe() > 0) {
        if (nodo2->getFe() > 0) {
            //DD hacer rotacion I
            return rotarI(nodo, nodo2);
        } else {
            //DI hacer rotacion ID
            throw ProductoNoEncontradoException("insertar", "rotacion DI no implementada en AVL");
        }
    } else if(nodo->getFe() < (-1)) {
        if (nodo2->getFe() > 0) {
            //ID hacer rotacion DI
            throw ProductoNoEncontradoException("insertar", "rotacion ID no implementada en AVL");
        } else {
            //II hacer rotacion D
            return rotarD(nodo, nodo2);
        }
    }

    return nodo;
}

NodoAvl* ArbolAvl::rotarI(NodoAvl* nodo1, NodoAvl* nodo2) {
    NodoAvl* temp = nodo2->getIzquierdo();

    nodo2->setIzquierdo(nodo1);
    nodo1->setDerecho(temp);

    actualizarFe(nodo1);
    actualizarFe(nodo2);

    return nodo2;
}

NodoAvl* ArbolAvl::rotarD(NodoAvl* nodo1, NodoAvl* nodo2) {
    NodoAvl* temp = nodo2->getDerecho();

    nodo2->setDerecho(nodo1);
    nodo1->setIzquierdo(temp);

    actualizarFe(nodo1);
    actualizarFe(nodo2);

    return nodo2;
}

NodoAvl* ArbolAvl::evaluarDesbalance(NodoAvl* nodo) {
    if(nodo->getFe() > 0) { //cargado a la derecha
        return nodo->getDerecho();
    } else if (nodo->getFe() < 0) { // cargado a la izquierda
        return nodo->getIzquierdo();
    }
    return nullptr;
}

void ArbolAvl::actualizarFe(NodoAvl* nodo) {
    if (nodo != nullptr) {
        int alturaIzquierda = obtenerAltura(nodo->getIzquierdo());
        int alturaDerecha = obtenerAltura(nodo->getDerecho());

        nodo->setAltura(1 + std::max(alturaIzquierda, alturaDerecha));
        nodo->setFe(alturaDerecha - alturaIzquierda);
    }
}

int ArbolAvl::obtenerAltura(NodoAvl* nodo) {
    return nodo == nullptr ? 0 : nodo->getAltura();
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
    if (valor == nullptr) {
        throw ProductoNoEncontradoException("insertar", "dato nulo en AVL");
    }

    if (valor->getName().empty()) {
        throw ProductoNoEncontradoException("insertar", "nombre vacio en AVL");
    }

    string nombreBuscado = valor->getName();
    if (buscarNodoRecursivo(raiz, nombreBuscado)) {
        throw ProductoNoEncontradoException("insertar", "ya existe un producto con nombre: " + valor->getName());
    }

    raiz = insertarNodoRecursivo(raiz, valor);
}

void ArbolAvl::eliminar(Product* valor) {
    raiz = eliminarNodoRecursivo(raiz, valor);
}

void ArbolAvl::buscar(string& nombre) {
    buscarNodoRecursivo(raiz, nombre);
}

