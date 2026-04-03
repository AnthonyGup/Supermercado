#include "ArbolBPlus.h"
#include <iostream>
#include <stdexcept>

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

    if (nodo->esHoja()) {
        for (int i = 0; i < nodo->Ocuenta(); i++) {
            if (categoria == nodo->Oclave(i)) {
                return nodo->Ovalor(i);
            }
        }
        return nullptr;
    }

    int i = 0;
    while (i < nodo->Ocuenta() && categoria >= nodo->Oclave(i)) {
        i++;
    }

    // Si no es hoja, descer a la rama apropiada
    return buscar(nodo->Orama(i), categoria);
}

void ArbolBPlus::insertar(Product* producto) {
    if (!raiz) crear();

    if (producto == nullptr) {
        throw runtime_error("producto nulo");
    }

    if (raiz->nodoLLeno()) {
        PPagina nuevaRaiz = new NodoBPlus(orden, false);
        nuevaRaiz->Prama(0, raiz);
        dividirNodo(nuevaRaiz, 0, raiz);
        raiz = nuevaRaiz;
    }

    insertatEnNodoNoLleno(raiz, producto);
}

void ArbolBPlus::insertatEnNodoNoLleno(PPagina nodo, Product* producto) {
    if (nodo == nullptr || producto == nullptr) {
        return;
    }

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
            if (categoria >= nodo->Oclave(i)) {
                i++;
            }
        }

        insertatEnNodoNoLleno(nodo->Orama(i), producto);
    }
}

void ArbolBPlus::dividirNodo(PPagina nodoPadre, int indice, PPagina nodoLleno) {
    if (nodoPadre == nullptr || nodoLleno == nullptr) {
        return;
    }

    bool esHoja = nodoLleno->esHoja();
    int totalClaves = nodoLleno->Ocuenta();
    int mitad = totalClaves / 2;

    PPagina nodoNuevo = new NodoBPlus(orden, esHoja);

    if (esHoja) {
        // Leaf split: left keeps the first half, right keeps the second half.
        int nuevaCuenta = totalClaves - mitad;
        for (int i = 0; i < nuevaCuenta; i++) {
            nodoNuevo->Pclave(i, nodoLleno->Oclave(mitad + i));
            nodoNuevo->Pvalor(i, nodoLleno->Ovalor(mitad + i));
        }
        nodoNuevo->Pcuenta(nuevaCuenta);
        nodoLleno->Pcuenta(mitad);

        nodoNuevo->PramaSiguiente(nodoLleno->OramaSiguiente());
        nodoLleno->PramaSiguiente(nodoNuevo);
    } else {
        // Internal split: promote the middle separator, move the right side to the new node.
        tipoClave separador = nodoLleno->Oclave(mitad);
        int nuevaCuenta = totalClaves - mitad - 1;

        for (int i = 0; i < nuevaCuenta; i++) {
            nodoNuevo->Pclave(i, nodoLleno->Oclave(mitad + 1 + i));
        }
        for (int i = 0; i <= nuevaCuenta; i++) {
            nodoNuevo->Prama(i, nodoLleno->Orama(mitad + 1 + i));
        }

        nodoNuevo->Pcuenta(nuevaCuenta);
        nodoLleno->Pcuenta(mitad);

        for (int i = nodoPadre->Ocuenta(); i >= indice + 1; i--) {
            nodoPadre->Prama(i + 1, nodoPadre->Orama(i));
        }
        for (int i = nodoPadre->Ocuenta() - 1; i >= indice; i--) {
            nodoPadre->Pclave(i + 1, nodoPadre->Oclave(i));
        }

        nodoPadre->Pclave(indice, separador);
        nodoPadre->Prama(indice + 1, nodoNuevo);
        nodoPadre->Pcuenta(nodoPadre->Ocuenta() + 1);
        return;
    }

    tipoClave separador = nodoNuevo->Oclave(0);

    for (int i = nodoPadre->Ocuenta(); i >= indice + 1; i--) {
        nodoPadre->Prama(i + 1, nodoPadre->Orama(i));
    }
    for (int i = nodoPadre->Ocuenta() - 1; i >= indice; i--) {
        nodoPadre->Pclave(i + 1, nodoPadre->Oclave(i));
    }

    nodoPadre->Pclave(indice, separador);
    nodoPadre->Prama(indice + 1, nodoNuevo);
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

bool ArbolBPlus::generarDot(const string& filepath) {
    try {
        DotGenerator gen("arbol_bplus", "Arbol B+ de Categorias (Orden " + to_string(orden) + ")");

        if (raiz == nullptr) {
            gen.addNode("empty", "Arbol Vacio", "ellipse", "lightgray");
        } else {
            int contador = 0;
            std::map<PPagina, int> mapIds;
            asignarIdsRecursivo(raiz, contador, mapIds);

            std::unordered_set<PPagina> visitadosNodos;
            agregarNodosAlGenerador(raiz, gen, mapIds, visitadosNodos);

            std::unordered_set<PPagina> visitadosAristas;
            agregarAristasAlGenerador(raiz, gen, mapIds, visitadosAristas);
        }

        return gen.saveToDot(filepath);
    } catch (const exception& e) {
        cerr << "Error al generar DOT del Arbol B+: " << e.what() << endl;
        return false;
    }
}

void ArbolBPlus::asignarIdsRecursivo(PPagina nodo, int& contador, std::map<PPagina, int>& mapIds) {
    if (nodo == nullptr) {
        return;
    }

    contador++;
    mapIds[nodo] = contador;

    if (!nodo->esHoja()) {
        for (int i = 0; i <= nodo->Ocuenta(); i++) {
            if (nodo->Orama(i) != nullptr) {
                asignarIdsRecursivo(nodo->Orama(i), contador, mapIds);
            }
        }
    }
}

void ArbolBPlus::agregarNodosAlGenerador(PPagina nodo, DotGenerator& gen, const std::map<PPagina, int>& mapIds, std::unordered_set<PPagina>& visitados) {
    if (nodo == nullptr) {
        return;
    }

    if (visitados.count(nodo) > 0) {
        return;
    }
    visitados.insert(nodo);

    int nodeId = mapIds.at(nodo);
    string id = "node_" + to_string(nodeId);

    stringstream label;
    label << "ID: " << nodeId << "\n";
    label << "Tipo: " << (nodo->esHoja() ? "Hoja" : "Interno") << "\n";
    label << "Claves: ";

    for (int i = 0; i < nodo->Ocuenta(); i++) {
        if (nodo->esHoja()) {
            int cantidad = 0;
            if (nodo->Ovalor(i) != nullptr) {
                cantidad = nodo->Ovalor(i)->getSize();
            }
            label << nodo->Oclave(i) << "(" << cantidad << ")";
        } else {
            label << nodo->Oclave(i);
        }
        if (i < nodo->Ocuenta() - 1) {
            label << " | ";
        }
    }

    bool lleno = nodo->nodoLLeno();
    string color = lleno ? "lightyellow" : "lightblue";
    string shape = nodo->esHoja() ? "ellipse" : "box";

    gen.addNode(id, label.str(), shape, color);

    if (!nodo->esHoja()) {
        for (int i = 0; i <= nodo->Ocuenta(); i++) {
            if (nodo->Orama(i) != nullptr) {
                agregarNodosAlGenerador(nodo->Orama(i), gen, mapIds, visitados);
            }
        }
    }
}

void ArbolBPlus::agregarAristasAlGenerador(PPagina nodo, DotGenerator& gen, const std::map<PPagina, int>& mapIds, std::unordered_set<PPagina>& visitados) {
    if (nodo == nullptr) {
        return;
    }

    if (visitados.count(nodo) > 0) {
        return;
    }
    visitados.insert(nodo);

    int parentId = mapIds.at(nodo);
    string parentStr = "node_" + to_string(parentId);

    if (!nodo->esHoja()) {
        for (int i = 0; i <= nodo->Ocuenta(); i++) {
            if (nodo->Orama(i) != nullptr) {
                int childId = mapIds.at(nodo->Orama(i));
                string childStr = "node_" + to_string(childId);
                gen.addEdge(parentStr, childStr, "r" + to_string(i));
                agregarAristasAlGenerador(nodo->Orama(i), gen, mapIds, visitados);
            }
        }
    }
}


