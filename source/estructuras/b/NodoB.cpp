#include "NodoB.h"

NodoB::NodoB(int orden) {
    this->m = orden;
    this->cuenta = 0;
    
    claves = new tipoClave[orden];
    ramas = new PPagina[orden + 1];
    
    for(int k = 0; k < orden; k++) {
        claves[k] = nullptr;
    }
    
    for(int k = 0; k <= orden; k++) {
        ramas[k] = nullptr;
    }
}

NodoB::~NodoB() {
    delete[] claves;
    delete[] ramas;
}

bool NodoB::nodoLLeno() const {
    return cuenta == m - 1;
}

bool NodoB::nodoSemiVacio() const {
    return cuenta < (m / 2);
}

NodoB::tipoClave NodoB::Oclave(int i) const {
    return claves[i];
}

void NodoB::Pclave(int i, tipoClave clave) {
    claves[i] = clave;
}

NodoB::PPagina NodoB::Orama(int i) const {
    return ramas[i];
}

void NodoB::Prama(int i, NodoB* p) {
    ramas[i] = p;
}

int NodoB::Ocuenta() const {
    return cuenta;
}

void NodoB::Pcuenta(int valor) {
    cuenta = valor;
}