#include "NodoBPlus.h"
#include <cstring>
#include <iostream>

NodoBPlus::NodoBPlus(int orden, bool hoja) : isHoja(hoja), cuenta(0), m(orden), siguiente(nullptr) {
    claves = new tipoClave[m - 1];
    ramas = new PPagina[m];
    valores = new tipoValor[m - 1];

    // Inicializar claves
    for (int i = 0; i < m - 1; i++) {
        claves[i] = "";
        valores[i] = nullptr;
    }

    // Inicializar ramas
    for (int i = 0; i < m; i++) {
        ramas[i] = nullptr;
    }
}

NodoBPlus::~NodoBPlus() {
    if (claves) delete[] claves;
    if (ramas) delete[] ramas;
    if (valores) {
        for (int i = 0; i < cuenta; i++) {
            if (valores[i]) delete valores[i];
        }
        delete[] valores;
    }
}

tipoClave NodoBPlus::Oclave(int i) const {
    if (i >= 0 && i < cuenta) {
        return claves[i];
    }
    return "";
}

PPagina NodoBPlus::Orama(int i) const {
    if (i >= 0 && i <= cuenta) {
        return ramas[i];
    }
    return nullptr;
}

tipoValor NodoBPlus::Ovalor(int i) const {
    if (i >= 0 && i < cuenta && esHoja) {
        return valores[i];
    }
    return nullptr;
}

PPagina NodoBPlus::OramaSiguiente() const {
    return siguiente;
}

bool NodoBPlus::esHoja() const {
    return esHoja;
}

int NodoBPlus::Ocuenta() const {
    return cuenta;
}

int NodoBPlus::Oorden() const {
    return m;
}

void NodoBPlus::Pclave(int i, tipoClave clave) {
    if (i >= 0 && i < m - 1) {
        claves[i] = clave;
    }
}

void NodoBPlus::Prama(int i, PPagina p) {
    if (i >= 0 && i < m) {
        ramas[i] = p;
    }
}

void NodoBPlus::Pvalor(int i, tipoValor lista) {
    if (i >= 0 && i < m - 1 && esHoja) {
        valores[i] = lista;
    }
}

void NodoBPlus::PramaSiguiente(PPagina p) {
    siguiente = p;
}

void NodoBPlus::Pcuenta(int valor) {
    cuenta = valor;
}

bool NodoBPlus::nodoLLeno() const {
    return cuenta == m - 1;
}

bool NodoBPlus::nodoSemiVacio() const {
    return cuenta <= (m / 2);
}

int NodoBPlus::buscarPosicion(tipoClave clave) const {
    int pos = 0;
    while (pos < cuenta && claves[pos] < clave) {
        pos++;
    }
    return pos;
}

void NodoBPlus::agregarProductoEnHoja(tipoValor lista, Product* producto) {
    if (lista) {
        lista->insertar(producto);
    }
}
