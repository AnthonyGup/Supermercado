#include "ArbolBPlus.h"
#include <iostream>

ArbolBPlus::ArbolBPlus(int m) : orden(m), raiz(nullptr), hojaInicio(nullptr) {
    crear();
}

ArbolBPlus::~ArbolBPlus() {
    // Recorrer todas las hojas usando el puntero siguiente y eliminar
    PPagina hoja = hojaInicio;
    while (hoja) {
        PPagina siguiente = hoja->OramaSiguiente();
        delete hoja;
        hoja = siguiente;
    }
}

bool ArbolBPlus::arbolBPlusvacio() const {
    return raiz == nullptr || raiz->Ocuenta() == 0;
}

PPagina ArbolBPlus::Oraiz() const {
    return raiz;
}

int ArbolBPlus::Oorden() const {
    return orden;
}

void ArbolBPlus::Praiz(PPagina r) {
    raiz = r;
}

void ArbolBPlus::Porden(int ord) {
    orden = ord;
}

void ArbolBPlus::crear() {
    if (raiz) delete raiz;
    raiz = new NodoBPlus(orden, true);
    hojaInicio = raiz;
}

ListaEnlazada* ArbolBPlus::buscar(const string& categoria) {
    if (!raiz) return nullptr;
    return buscar(raiz, categoria);
}

ListaEnlazada* ArbolBPlus::buscar(PPagina nodo, const string& categoria) {
    if (!nodo) return nullptr;

    int i = 0;
    while (i < nodo->Ocuenta() && categoria > nodo->Oclave(i)) {
        i++;
    }

    // Verificar si encontramos la categoría
    if (i < nodo->Ocuenta() && categoria == nodo->Oclave(i)) {
        if (nodo->esHoja()) {
            return nodo->Ovalor(i);
        }
    }

    // Si es hoja y no encontramos, retornar nullptr
    if (nodo->esHoja()) {
        return nullptr;
    }

    // Si no es hoja, descer a la rama apropiada
    return buscar(nodo->Orama(i), categoria);
}

void ArbolBPlus::insertar(Product* producto) {
    if (!raiz) crear();

    string categoria = producto->getCategory();

    // Si la raíz está llena, dividir
    if (raiz->nodoLLeno()) {
        PPagina nuevoNodo = new NodoBPlus(orden, false);
        nuevoNodo->Prama(0, raiz);

        // Dividir la raíz
        dividirNodo(nuevoNodo, 0, raiz);
        raiz = nuevoNodo;
    }

    insertatEnNodoNoLleno(raiz, producto);
}

void ArbolBPlus::insertatEnNodoNoLleno(PPagina nodo, Product* producto) {
    string categoria = producto->getCategory();
    int i = nodo->Ocuenta() - 1;

    if (nodo->esHoja()) {
        // Desplazar claves y valores
        while (i >= 0 && categoria < nodo->Oclave(i)) {
            nodo->Pclave(i + 1, nodo->Oclave(i));
            nodo->Pvalor(i + 1, nodo->Ovalor(i));
            i--;
        }

        // Si ya existe la categoría
        if (i >= 0 && categoria == nodo->Oclave(i)) {
            nodo->agregarProductoEnHoja(nodo->Ovalor(i), producto);
        } else {
            // Nueva categoría
            nodo->Pclave(i + 1, categoria);
            ListaEnlazada* nuevaLista = new ListaEnlazada();
            nuevaLista->insertar(producto);
            nodo->Pvalor(i + 1, nuevaLista);
            nodo->Pcuenta(nodo->Ocuenta() + 1);
        }
    } else {
        // Encontrar el hijo donde descender
        while (i >= 0 && categoria < nodo->Oclave(i)) {
            i--;
        }
        i++;

        PPagina hijo = nodo->Orama(i);
        if (hijo && hijo->nodoLLeno()) {
            dividirNodo(nodo, i, hijo);
            if (categoria > nodo->Oclave(i)) {
                i++;
            }
        }

        insertatEnNodoNoLleno(nodo->Orama(i), producto);
    }
}

void ArbolBPlus::dividirNodo(PPagina nodoPadre, int indice, PPagina nodoLleno) {
    int t = (orden + 1) / 2;
    PPagina nodoNuevo = new NodoBPlus(orden, nodoLleno->esHoja());
    
    const int MITAD = (orden - 1) / 2;

    // Copiar la segunda mitad de claves a nodoNuevo
    for (int i = 0; i < MITAD; i++) {
        nodoNuevo->Pclave(i, nodoLleno->Oclave(i + MITAD + 1));
        
        if (nodoLleno->esHoja()) {
            nodoNuevo->Pvalor(i, nodoLleno->Ovalor(i + MITAD + 1));
        }
    }

    // Si no es hoja, copiar ramas
    if (!nodoLleno->esHoja()) {
        for (int i = 0; i <= MITAD; i++) {
            nodoNuevo->Prama(i, nodoLleno->Orama(i + MITAD + 1));
        }
    } else {
        // Enlazar hojas
        nodoNuevo->PramaSiguiente(nodoLleno->OramaSiguiente());
        nodoLleno->PramaSiguiente(nodoNuevo);
    }

    nodoNuevo->Pcuenta(MITAD);
    nodoLleno->Pcuenta(MITAD);

    // Desplazar ramas en el padre
    for (int i = nodoPadre->Ocuenta(); i > indice; i--) {
        nodoPadre->Prama(i + 1, nodoPadre->Orama(i));
    }

    nodoPadre->Prama(indice + 1, nodoNuevo);

    // Desplazar claves en el padre
    for (int i = nodoPadre->Ocuenta() - 1; i >= indice; i--) {
        nodoPadre->Pclave(i + 1, nodoPadre->Oclave(i));
    }

    nodoPadre->Pclave(indice, nodoLleno->Oclave(MITAD));
    nodoPadre->Pcuenta(nodoPadre->Ocuenta() + 1);
}

void ArbolBPlus::eliminar(const string& categoria) {
    if (!raiz) return;

    eliminar(raiz, categoria);

    // Si la raíz quedó vacía, su único hijo se convierte en raíz
    if (raiz->Ocuenta() == 0) {
        if (!raiz->esHoja() && raiz->Orama(0)) {
            PPagina temp = raiz;
            raiz = raiz->Orama(0);
            temp->Prama(0, nullptr);
            delete temp;
        }
    }
}

void ArbolBPlus::eliminar(PPagina nodo, const string& categoria) {
    int i = 0;
    while (i < nodo->Ocuenta() && categoria > nodo->Oclave(i)) {
        i++;
    }

    bool encontrado = (i < nodo->Ocuenta() && categoria == nodo->Oclave(i));

    if (nodo->esHoja()) {
        // Si es hoja y encontramos la clave
        if (encontrado) {
            // Eliminar la clave y su valor
            if (nodo->Ovalor(i)) delete nodo->Ovalor(i);

            for (int j = i; j < nodo->Ocuenta() - 1; j++) {
                nodo->Pclave(j, nodo->Oclave(j + 1));
                nodo->Pvalor(j, nodo->Ovalor(j + 1));
            }
            nodo->Pcuenta(nodo->Ocuenta() - 1);
        }
    } else {
        // Nodo interno
        if (encontrado) {
            // Clave existe en nodo interno - no hacemos nada
            // (en B+ solo se usan como navegacion)
        } else {
            // Buscar en el hijo apropiado
            bool es_ultimo = (i == nodo->Ocuenta());

            // Asegurar que el hijo tenga suficientes claves antes de descender
            if (nodo->Orama(i) && nodo->Orama(i)->Ocuenta() < (orden / 2)) {
                llenarNodo(nodo, i);
            }

            // Después de llenarNodo, la posición podría haber cambiado
            if (es_ultimo && i > nodo->Ocuenta()) {
                eliminar(nodo->Orama(i - 1), categoria);
            } else {
                eliminar(nodo->Orama(i), categoria);
            }
        }
    }
}

void ArbolBPlus::llenarNodo(PPagina nodo, int pos) {
    // Si el hermano anterior tiene suficientes claves, tomar prestado
    if (pos != 0 && nodo->Orama(pos - 1) && nodo->Orama(pos - 1)->Ocuenta() >= orden / 2) {
        tomarDelAnterior(nodo, pos);
    }
    // Si el hermano siguiente tiene suficientes claves, tomar prestado
    else if (pos != nodo->Ocuenta() && nodo->Orama(pos + 1) && 
             nodo->Orama(pos + 1)->Ocuenta() >= orden / 2) {
        tomarDelSiguiente(nodo, pos);
    }
    // Si ambos hermanos tienen el mínimo, fusionar con uno de ellos
    else if (pos != 0) {
        fusionarNodos(nodo, pos);
    } else if (pos != nodo->Ocuenta()) {
        fusionarNodos(nodo, pos + 1);
    }
}

void ArbolBPlus::tomarDelAnterior(PPagina nodo, int pos) {
    PPagina hijo = nodo->Orama(pos);
    PPagina hermanoAnterior = nodo->Orama(pos - 1);

    // Desplazar las claves del hijo a la derecha
    for (int i = hijo->Ocuenta() - 1; i >= 0; i--) {
        hijo->Pclave(i + 1, hijo->Oclave(i));
        if (hijo->esHoja()) {
            hijo->Pvalor(i + 1, hijo->Ovalor(i));
        } else {
            hijo->Prama(i + 1, hijo->Orama(i));
        }
    }

    // Si no es hoja, mover la rama izquierda
    if (!hijo->esHoja()) {
        hijo->Prama(0, hermanoAnterior->Orama(hermanoAnterior->Ocuenta()));
    }

    // Mover clave del padre al hijo
    hijo->Pclave(0, nodo->Oclave(pos - 1));
    if (hijo->esHoja()) {
        hijo->Pvalor(0, nullptr);
    }

    // Mover clave del hermano al padre
    nodo->Pclave(pos - 1, hermanoAnterior->Oclave(hermanoAnterior->Ocuenta() - 1));

    hijo->Pcuenta(hijo->Ocuenta() + 1);
    hermanoAnterior->Pcuenta(hermanoAnterior->Ocuenta() - 1);
}

void ArbolBPlus::tomarDelSiguiente(PPagina nodo, int pos) {
    PPagina hijo = nodo->Orama(pos);
    PPagina hermanoSiguiente = nodo->Orama(pos + 1);

    // Mover clave del padre al hijo
    hijo->Pclave(hijo->Ocuenta(), nodo->Oclave(pos));
    if (hijo->esHoja()) {
        hijo->Pvalor(hijo->Ocuenta(), nullptr);
    }

    // Si no es hoja, mover rama del hermano
    if (!hijo->esHoja()) {
        hijo->Prama(hijo->Ocuenta() + 1, hermanoSiguiente->Orama(0));
    }

    // Mover clave del hermano al padre
    nodo->Pclave(pos, hermanoSiguiente->Oclave(0));

    // Desplazar claves del hermano a la izquierda
    for (int i = 0; i < hermanoSiguiente->Ocuenta() - 1; i++) {
        hermanoSiguiente->Pclave(i, hermanoSiguiente->Oclave(i + 1));
        if (hermanoSiguiente->esHoja()) {
            hermanoSiguiente->Pvalor(i, hermanoSiguiente->Ovalor(i + 1));
        } else {
            hermanoSiguiente->Prama(i, hermanoSiguiente->Orama(i + 1));
        }
    }

    // Desplazar última rama si no es hoja
    if (!hermanoSiguiente->esHoja()) {
        hermanoSiguiente->Prama(hermanoSiguiente->Ocuenta() - 1, 
                                hermanoSiguiente->Orama(hermanoSiguiente->Ocuenta()));
    }

    hijo->Pcuenta(hijo->Ocuenta() + 1);
    hermanoSiguiente->Pcuenta(hermanoSiguiente->Ocuenta() - 1);
}

void ArbolBPlus::fusionarNodos(PPagina nodo, int pos) {
    PPagina hijo = nodo->Orama(pos - 1);
    PPagina hermano = nodo->Orama(pos);

    // Mover clave del padre al hijo
    hijo->Pclave(hijo->Ocuenta(), nodo->Oclave(pos - 1));

    // Copiar todas las claves y valores del hermano al hijo
    for (int i = 0; i < hermano->Ocuenta(); i++) {
        hijo->Pclave(hijo->Ocuenta() + 1, hermano->Oclave(i));
        if (hermano->esHoja()) {
            hijo->Pvalor(hijo->Ocuenta(), hermano->Ovalor(i));
        } else {
            hijo->Prama(hijo->Ocuenta(), hermano->Orama(i));
        }
    }

    // Si no es hoja, copiar última rama
    if (!hermano->esHoja()) {
        hijo->Prama(hijo->Ocuenta() + hermano->Ocuenta(), hermano->Orama(hermano->Ocuenta()));
    }

    hijo->Pcuenta(hijo->Ocuenta() + hermano->Ocuenta() + 1);

    // Desplazar claves y ramas del padre
    for (int i = pos - 1; i < nodo->Ocuenta() - 1; i++) {
        nodo->Pclave(i, nodo->Oclave(i + 1));
        nodo->Prama(i, nodo->Orama(i + 1));
    }
    nodo->Prama(nodo->Ocuenta() - 1, nodo->Orama(nodo->Ocuenta()));

    nodo->Pcuenta(nodo->Ocuenta() - 1);
    delete hermano;
}

int ArbolBPlus::buscarIndice(PPagina nodo, const string& categoria) const {
    int i = 0;
    while (i < nodo->Ocuenta() && categoria > nodo->Oclave(i)) {
        i++;
    }
    return i;
}

void ArbolBPlus::mostrarArbol() {
    if (!raiz) {
        cout << "El árbol B+ está vacío." << endl;
        return;
    }
    cout << "Estructura del Árbol B+:" << endl;
    mostrarArbol(raiz, 0);
}

void ArbolBPlus::mostrarArbol(PPagina nodo, int profundidad) {
    if (!nodo) return;

    for (int i = 0; i < profundidad; i++) {
        cout << "  ";
    }

    cout << "[";
    for (int i = 0; i < nodo->Ocuenta(); i++) {
        cout << nodo->Oclave(i);
        if (i < nodo->Ocuenta() - 1) cout << ", ";
    }
    cout << "]" << endl;

    if (!nodo->esHoja()) {
        for (int i = 0; i <= nodo->Ocuenta(); i++) {
            mostrarArbol(nodo->Orama(i), profundidad + 1);
        }
    }
}

void ArbolBPlus::listarPorCategoria() {
    if (!hojaInicio) {
        cout << "No hay datos para mostrar." << endl;
        return;
    }

    PPagina hoja = hojaInicio;
    while (hoja) {
        for (int i = 0; i < hoja->Ocuenta(); i++) {
            cout << "Categoría: " << hoja->Oclave(i) << endl;
            if (hoja->Ovalor(i)) {
                // Aquí podrías iterar los productos en la lista
                cout << "  Productos: " << hoja->Ovalor(i)->getSize() << endl;
            }
        }
        hoja = hoja->OramaSiguiente();
    }
}

void ArbolBPlus::listarCategoria(const string& categoria) {
    ListaEnlazada* productos = buscar(categoria);
    if (productos) {
        cout << "Productos en categoría: " << categoria << endl;
        cout << "Total: " << productos->getSize() << endl;
    } else {
        cout << "No hay productos en la categoría: " << categoria << endl;
    }
}

// =============== MÉTODOS DE GENERACIÓN DOT ===============


