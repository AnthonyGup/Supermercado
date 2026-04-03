#include "ArbolB.h"
#include <stdexcept>
#include <vector>

ArbolB::ArbolB() {
    this->orden = 0;
    this->raiz = nullptr;
}

ArbolB::ArbolB(int m) {
    this->orden = m;
    this->raiz = nullptr;
}

void ArbolB::insertar(Product* cl) {
    if (cl == nullptr) {
        throw std::runtime_error("producto nulo");
    }
    if (cl->getExpiry_date().empty()) {
        throw std::runtime_error("expiry_date vacio");
    }
    if (buscar(cl->getExpiry_date()) != nullptr) {
        throw std::runtime_error("ya existe un producto con expiry_date: " + cl->getExpiry_date());
    }

    if (raiz == nullptr) {
        raiz = new NodoB(orden);
        raiz->Pcuenta(1);
        raiz->Pclave(1, cl);
        return;
    }

    if (raiz->nodoLLeno()) {
        NodoB* nuevaRaiz = new NodoB(orden);
        nuevaRaiz->Prama(0, raiz);
        dividirHijo(nuevaRaiz, 0);
        raiz = nuevaRaiz;
    }

    insertarNoLleno(raiz, cl);
}

void ArbolB::eliminar(const string& cl) {
    raiz = eliminar(raiz, cl);
    // Si después de eliminar la raiz queda vacía pero tiene un solo hijo,
    // ese hijo se convierte en la nueva raiz
    if (raiz != nullptr && raiz->Ocuenta() == 0) {
        NodoB* nodo_temp = raiz;
        if (!raiz->nodoSemiVacio()) {
            raiz = raiz->Orama(0);
        }
        delete nodo_temp;
    }
}

Product* ArbolB::buscar(const string& clave) {
    int n = 0;
    return buscar(raiz, clave, n);
}

bool ArbolB::buscarNodo(NodoB* actual, Product* producto, int& k) {
    int index;
    bool encontrado;
    string claveBuscada = producto->getExpiry_date();
    
    if(claveBuscada < actual->Oclave(1)->getExpiry_date()) {
        encontrado = false;
        index = 0;
    } else { // orden descendente
        index = actual->Ocuenta();
        while (claveBuscada < actual->Oclave(index)->getExpiry_date() && (index > 1)) {
            index--;
        }
        encontrado = claveBuscada == actual->Oclave(index)->getExpiry_date();
    }
    k = index;
    return encontrado;
}

Product* ArbolB::buscar(NodoB* actual, const string& cl, int& n) {
    
    if (actual == nullptr) {
        return nullptr;
    }
    if (actual->Ocuenta() == 0) {
        if (actual->nodoSemiVacio()) {
            return nullptr;
        }
        return buscar(actual->Orama(0), cl, n);
    }
    else {
        int index;
        bool encontrado = false;
        
        // Buscar en las claves actuales
        if(cl < actual->Oclave(1)->getExpiry_date()) {
            encontrado = false;
            index = 0;
        } else {
            index = actual->Ocuenta();
            while (cl < actual->Oclave(index)->getExpiry_date() && (index > 1)) {
                index--;
            }
            encontrado = cl == actual->Oclave(index)->getExpiry_date();
        }
        
        if(encontrado) {
            return actual->Oclave(index);  // Retornar el Product* encontrado
        } else {
            return buscar(actual->Orama(index), cl, n);
        }
    }
}

NodoB* ArbolB::insertar(NodoB* raiz, Product* cl) {
    bool subeArriba;
    Product* productoMediana;
    NodoB* nd;
    subeArriba = empujar(raiz, cl, productoMediana, nd);
    if (subeArriba) {
        // El árbol crece en altura por la raiz,
        // sube un nuevo producto mediana y un nuevo hijo derecho nd
        // en la implementacion se mantiene que los productos que son
        // menores que la mediana se encuentran en raiz y los mayores en nd
        NodoB* p;
        p = new NodoB(orden);
        p->Pcuenta(1);
        p->Pclave(1, productoMediana);  // Almacenar el producto médiana directamente
        p->Prama(0, raiz);
        p->Prama(1, nd);
        raiz = p;  // La nueva raiz es p
    }
    return raiz;
}

void ArbolB::insertarNoLleno(NodoB* nodo, Product* producto) {
    if (nodo == nullptr) {
        return;
    }

    int i = nodo->Ocuenta();
    string clave = producto->getExpiry_date();

    if (nodo->nodoSemiVacio()) {
        while (i >= 1 && clave < nodo->Oclave(i)->getExpiry_date()) {
            nodo->Pclave(i + 1, nodo->Oclave(i));
            i--;
        }
        nodo->Pclave(i + 1, producto);
        nodo->Pcuenta(nodo->Ocuenta() + 1);
        return;
    }

    while (i >= 1 && clave < nodo->Oclave(i)->getExpiry_date()) {
        i--;
    }

    if (nodo->Orama(i) != nullptr && nodo->Orama(i)->nodoLLeno()) {
        dividirHijo(nodo, i);
        if (clave > nodo->Oclave(i + 1)->getExpiry_date()) {
            i++;
        }
    }

    insertarNoLleno(nodo->Orama(i), producto);
}

void ArbolB::dividirHijo(NodoB* padre, int indiceHijo) {
    if (padre == nullptr) {
        return;
    }

    NodoB* hijo = padre->Orama(indiceHijo);
    if (hijo == nullptr || !hijo->nodoLLeno()) {
        return;
    }

    const int claveMediana = orden / 2;
    Product* mediana = hijo->Oclave(claveMediana);

    NodoB* nuevo = new NodoB(orden);
    int clavesDerecha = hijo->Ocuenta() - claveMediana;

    nuevo->Pcuenta(clavesDerecha);
    int destinoClave = 1;
    for (int i = claveMediana + 1; i <= hijo->Ocuenta(); i++) {
        nuevo->Pclave(destinoClave++, hijo->Oclave(i));
        hijo->Pclave(i, nullptr);
    }

    if (!hijo->nodoSemiVacio()) {
        int destinoRama = 0;
        for (int i = claveMediana; i <= hijo->Ocuenta(); i++) {
            nuevo->Prama(destinoRama++, hijo->Orama(i));
            hijo->Prama(i, nullptr);
        }
    }

    hijo->Pcuenta(claveMediana - 1);

    for (int i = padre->Ocuenta(); i >= indiceHijo + 1; i--) {
        padre->Pclave(i + 1, padre->Oclave(i));
        padre->Prama(i + 1, padre->Orama(i));
    }

    padre->Pclave(indiceHijo + 1, mediana);
    padre->Prama(indiceHijo + 1, nuevo);
    padre->Pcuenta(padre->Ocuenta() + 1);
}

bool ArbolB::empujar(NodoB* actual, Product* producto, Product*& productoMediana, NodoB*& nuevo) {
    int k;
    bool subeArriba = false;
    if (actual == nullptr) {
        // Envía arriba el producto para que se inserte en la página padre
        subeArriba = true;
        productoMediana = producto;  // Almacena el producto
        nuevo = nullptr;
        // el dato NodoB de nuevo esta NULL
    }
    else {
        bool esta;
        esta = buscarNodo(actual, producto, k);  // Pasar el Product* completo
        if (esta) {
            throw std::runtime_error("clave duplicada");
        }
        subeArriba = empujar(actual->Orama(k), producto, productoMediana, nuevo);  // Sin mediana
        if(subeArriba) {
            if(actual->nodoLLeno())  { // hay que dividir las paginas
                dividirNodo(actual, productoMediana, nuevo, k);  // Pasar productoMediana
            }
            else {
                meterPagina(actual, productoMediana, nuevo, k);
            }
        }
    }
    return subeArriba;
}

void ArbolB::meterPagina(NodoB* actual, Product* producto, NodoB* ramaDr, int k) {
    // Desplaza a la derecha los elementos para hacer un hueco
    for (int i = actual->Ocuenta(); i >= k + 1; i--) {
        actual->Pclave(i + 1, actual->Oclave(i));
        actual->Prama(i + 1, actual->Orama(i));
    }
    // pone el producto y la rama en la posicion k+1
    actual->Pclave(k+1, producto);  // Almacenar el Product* directamente
    actual->Prama(k+1, ramaDr);
    // incrementa el contador de productos almacenados
    actual->Pcuenta(actual->Ocuenta()+1);
}

void ArbolB::dividirNodo(NodoB* actual, Product*& mediana, NodoB*& nuevo, int pos) {
    const int totalClaves = actual->Ocuenta() + 1;
    const int insertPos = pos + 1; // posicion 1-based dentro del nodo ya con la nueva clave
    const int posMdna = orden / 2;

    std::vector<Product*> clavesTemp(orden + 1, nullptr);
    std::vector<NodoB*> ramasTemp(orden + 2, nullptr);

    // Copiar ramas originales, desplazando la rama nueva al lugar correcto
    for (int i = 0; i <= actual->Ocuenta(); i++) {
        if (i < insertPos) {
            ramasTemp[i] = actual->Orama(i);
        } else {
            ramasTemp[i + 1] = actual->Orama(i);
        }
    }
    ramasTemp[insertPos] = nuevo;

    // Copiar claves originales, dejando espacio para la nueva clave
    for (int i = 1; i <= actual->Ocuenta(); i++) {
        if (i < insertPos) {
            clavesTemp[i] = actual->Oclave(i);
        } else {
            clavesTemp[i + 1] = actual->Oclave(i);
        }
    }
    clavesTemp[insertPos] = mediana;

    NodoB* nuevaPag = new NodoB(orden);

    // Nodo izquierdo: claves 1..posMdna-1 y ramas 0..posMdna-1
    actual->Pcuenta(posMdna - 1);
    for (int i = 1; i <= actual->Ocuenta(); i++) {
        actual->Pclave(i, clavesTemp[i]);
    }
    for (int i = 0; i <= actual->Ocuenta(); i++) {
        actual->Prama(i, ramasTemp[i]);
    }

    // Clave mediana que sube
    mediana = clavesTemp[posMdna];

    // Nodo derecho: claves posMdna+1..totalClaves y ramas posMdna..totalClaves
    int nuevaCuenta = totalClaves - posMdna;
    nuevaPag->Pcuenta(nuevaCuenta);
    int destinoClave = 1;
    for (int i = posMdna + 1; i <= totalClaves; i++) {
        nuevaPag->Pclave(destinoClave++, clavesTemp[i]);
    }
    int destinoRama = 0;
    for (int i = posMdna; i <= totalClaves; i++) {
        nuevaPag->Prama(destinoRama++, ramasTemp[i]);
    }

    nuevo = nuevaPag;
}


NodoB* ArbolB::eliminar(NodoB* nodo, const string& clave) {
    if (nodo == nullptr) {
        return nullptr;
    }

    const int minClaves = (orden - 1) / 2;

    int k = 1;
    while (k <= nodo->Ocuenta() && clave > nodo->Oclave(k)->getExpiry_date()) {
        k++;
    }

    bool encontrado = (k <= nodo->Ocuenta() && clave == nodo->Oclave(k)->getExpiry_date());

    if (nodo->nodoSemiVacio()) {
        // Caso 1: El nodo es una hoja
        if (encontrado) {
            for (int i = k; i < nodo->Ocuenta(); i++) {
                nodo->Pclave(i, nodo->Oclave(i + 1));
            }
            nodo->Pcuenta(nodo->Ocuenta() - 1);
        }
    } else {
        // Caso 2: El nodo es interno (no es hoja)
        if (encontrado) {
            // La clave se encuentra en este nodo
            if (nodo->Orama(k-1) != nullptr && nodo->Orama(k-1)->Ocuenta() > minClaves) {
                // El hijo izquierdo tiene suficientes claves, obtener predecesor
                NodoB* pred = nodo->Orama(k-1);
                while (pred != nullptr && !pred->nodoSemiVacio()) {
                    pred = pred->Orama(pred->Ocuenta());
                }
                Product* predecesor = pred->Oclave(pred->Ocuenta());
                nodo->Pclave(k, predecesor);
                nodo->Prama(k-1, eliminar(nodo->Orama(k-1), predecesor->getExpiry_date()));
            } else if (nodo->Orama(k) != nullptr && nodo->Orama(k)->Ocuenta() > minClaves) {
                // El hijo derecho tiene suficientes claves, obtener sucesor
                NodoB* succ = nodo->Orama(k);
                while (succ != nullptr && !succ->nodoSemiVacio()) {
                    succ = succ->Orama(0);
                }
                Product* sucesor = succ->Oclave(1);
                nodo->Pclave(k, sucesor);
                nodo->Prama(k, eliminar(nodo->Orama(k), sucesor->getExpiry_date()));
            } else {
                // Ambos hijos tienen t-1 claves, fusionar
                fusionarNodos(nodo, k);
                nodo->Prama(k-1, eliminar(nodo->Orama(k-1), clave));
            }
        } else {
            // La clave a eliminar está en el subárbol
            bool es_en_ultimo = (k == nodo->Ocuenta() + 1);
            
            // Asegurar que el hijo tenga suficientes claves
            if (nodo->Orama(k) != nullptr && nodo->Orama(k)->Ocuenta() <= minClaves) {
                llenarNodo(nodo, k);
            }
            
            if (es_en_ultimo && k > nodo->Ocuenta()) {
                nodo->Prama(k-1, eliminar(nodo->Orama(k-1), clave));
            } else {
                nodo->Prama(k, eliminar(nodo->Orama(k), clave));
            }
        }
    }
    return nodo;
}

void ArbolB::llenarNodo(NodoB* nodo, int k) {
    const int minClaves = (orden - 1) / 2;

    if (nodo == nullptr || nodo->Orama(k) == nullptr) {
        return;
    }

    // Si el hijo anterior tiene mas del minimo de claves, tomar una de el
    if (k > 0 && nodo->Orama(k-1) != nullptr && nodo->Orama(k-1)->Ocuenta() > minClaves) {
        tomarDelAnterior(nodo, k);
    }
    // Si el hijo siguiente tiene mas del minimo de claves, tomar una de el
    else if (k < nodo->Ocuenta() && nodo->Orama(k+1) != nullptr && nodo->Orama(k+1)->Ocuenta() > minClaves) {
        tomarDelSiguiente(nodo, k);
    }
    // Si ambos hermanos tienen el minimo, fusionar
    else if (k > 0) {
        fusionarNodos(nodo, k);
    } else {
        fusionarNodos(nodo, k+1);
    }
}

void ArbolB::tomarDelAnterior(NodoB* nodo, int pos) {
    NodoB* hijo = nodo->Orama(pos);
    NodoB* hermanoAnterior = nodo->Orama(pos - 1);
    bool esHoja = (hijo->Orama(0) == nullptr);

    int cuentaHijo = hijo->Ocuenta();
    int cuentaHermano = hermanoAnterior->Ocuenta();

    // Desplazar las claves del hijo a la derecha
    for (int i = cuentaHijo; i >= 1; i--) {
        hijo->Pclave(i + 1, hijo->Oclave(i));
    }
    if (!esHoja) {
        for (int i = cuentaHijo; i >= 0; i--) {
            hijo->Prama(i + 1, hijo->Orama(i));
        }
        hijo->Prama(0, hermanoAnterior->Orama(cuentaHermano));
    }

    hijo->Pclave(1, nodo->Oclave(pos));

    nodo->Pclave(pos, hermanoAnterior->Oclave(cuentaHermano));
    hijo->Pcuenta(cuentaHijo + 1);
    hermanoAnterior->Pcuenta(cuentaHermano - 1);
}

void ArbolB::tomarDelSiguiente(NodoB* nodo, int pos) {
    NodoB* hijo = nodo->Orama(pos);
    NodoB* hermanoSiguiente = nodo->Orama(pos + 1);
    bool esHoja = (hijo->Orama(0) == nullptr);

    int cuentaHijo = hijo->Ocuenta();
    int cuentaHermano = hermanoSiguiente->Ocuenta();

    hijo->Pclave(cuentaHijo + 1, nodo->Oclave(pos + 1));
    if (!esHoja) {
        hijo->Prama(cuentaHijo + 1, hermanoSiguiente->Orama(0));
    }

    nodo->Pclave(pos + 1, hermanoSiguiente->Oclave(1));

    for (int i = 1; i < cuentaHermano; i++) {
        hermanoSiguiente->Pclave(i, hermanoSiguiente->Oclave(i + 1));
    }
    if (!esHoja) {
        for (int i = 0; i < cuentaHermano; i++) {
            hermanoSiguiente->Prama(i, hermanoSiguiente->Orama(i + 1));
        }
    }

    hijo->Pcuenta(cuentaHijo + 1);
    hermanoSiguiente->Pcuenta(cuentaHermano - 1);
}

void ArbolB::fusionarNodos(NodoB* nodo, int k) {
    NodoB* hijo = nodo->Orama(k - 1);
    NodoB* hermano = nodo->Orama(k);
    bool esHoja = (hijo->Orama(0) == nullptr);

    int cuentaHijo = hijo->Ocuenta();
    int cuentaHermano = hermano->Ocuenta();

    hijo->Pclave(cuentaHijo + 1, nodo->Oclave(k));
    if (!esHoja) {
        hijo->Prama(cuentaHijo + 1, hermano->Orama(0));
    }
    for (int i = 1; i <= cuentaHermano; i++) {
        hijo->Pclave(cuentaHijo + 1 + i, hermano->Oclave(i));
    }
    if (!esHoja) {
        for (int i = 1; i <= cuentaHermano; i++) {
            hijo->Prama(cuentaHijo + 1 + i, hermano->Orama(i));
        }
    }

    hijo->Pcuenta(cuentaHijo + cuentaHermano + 1);

    // Desplazar las claves y ramas del nodo padre
    for (int i = k; i < nodo->Ocuenta(); i++) {
        nodo->Pclave(i, nodo->Oclave(i + 1));
        nodo->Prama(i, nodo->Orama(i + 1));
    }

    nodo->Pcuenta(nodo->Ocuenta() - 1);
    nodo->Prama(nodo->Ocuenta() + 1, nullptr);
    delete hermano;
}

void ArbolB::eliminarNodo(NodoB* nodo) {
    if (nodo == nullptr) {
        return;
    }
    
    // Eliminar recursivamente todos los hijos
    if (!nodo->nodoSemiVacio()) {
        for (int i = 0; i <= nodo->Ocuenta(); i++) {
            eliminarNodo(nodo->Orama(i));
        }
    }
    
    // Eliminar el nodo actual
    delete nodo;
}

ArbolB::~ArbolB() {
    eliminarNodo(raiz);
}

bool ArbolB::generarDot(const string& filepath) {
    try {
        DotGenerator gen("arbol_b", "Arbol B (Orden " + to_string(orden) + ")");
        
        if (raiz == nullptr) {
            gen.addNode("empty", "Arbol Vacio", "ellipse", "lightgray");
        } else {
            // Paso 1: Asignar IDs secuenciales a todos los nodos
            int contador = 0;
            map<NodoB*, int> mapIds;
            asignarIdsRecursivo(raiz, contador, mapIds);
            
            // Paso 2: Agregar todos los nodos al generador
            unordered_set<NodoB*> visitadosNodos;
            agregarNodosAlGenerador(raiz, gen, mapIds, visitadosNodos);
            
            // Paso 3: Agregar todas las aristas
            unordered_set<NodoB*> visitadosAristas;
            agregarAristasAlGenerador(raiz, gen, mapIds, visitadosAristas);
        }
        
        return gen.saveToDot(filepath);
    } catch (const exception& e) {
        cerr << "Error al generar DOT del Arbol B: " << e.what() << endl;
        return false;
    }
}

// Paso 1: Asignar IDs unicos y correlativos a cada nodo
void ArbolB::asignarIdsRecursivo(NodoB* nodo, int& contador, std::map<NodoB*, int>& mapIds) {
    if (nodo == nullptr) {
        return;
    }
    
    // Pre-order: asignar ID primero
    contador++;
    mapIds[nodo] = contador;
    
    // Luego procesar todos los hijos validos
    for (int i = 0; i <= nodo->Ocuenta(); i++) {
        if (nodo->Orama(i) != nullptr) {
            asignarIdsRecursivo(nodo->Orama(i), contador, mapIds);
        }
    }
}

// Paso 2: Agregar todos los nodos definidos correctamente
void ArbolB::agregarNodosAlGenerador(NodoB* nodo, DotGenerator& gen, const std::map<NodoB*, int>& mapIds, std::unordered_set<NodoB*>& visitados) {
    if (nodo == nullptr) {
        return;
    }

    if (visitados.count(nodo) > 0) {
        return;
    }
    visitados.insert(nodo);
    
    // Obtener el ID de este nodo
    int nodeId = mapIds.at(nodo);
    string id = "node_" + to_string(nodeId);
    
    // Crear etiqueta con las fechas de vencimiento
    stringstream label;
    label << "ID: " << nodeId << "\n";
    label << "Tipo: " << (nodo->Orama(0) == nullptr ? "Hoja" : "Interno") << "\n";
    label << "Claves: ";
    for (int i = 1; i <= nodo->Ocuenta(); i++) {
        if (nodo->Oclave(i) != nullptr) {
            label << nodo->Oclave(i)->getExpiry_date();
        }
        if (i < nodo->Ocuenta()) {
            label << " | ";
        }
    }
    
    // Determinar color y forma
    bool esHoja = (nodo->Orama(0) == nullptr);
    bool lleno = (nodo->Ocuenta() >= orden - 1);
    
    string color = lleno ? "lightyellow" : "lightblue";
    string shape = esHoja ? "ellipse" : "box";
    
    gen.addNode(id, label.str(), shape, color);
    
    // Recursivamente agregar nodos hijos
    for (int i = 0; i <= nodo->Ocuenta(); i++) {
        if (nodo->Orama(i) != nullptr) {
            agregarNodosAlGenerador(nodo->Orama(i), gen, mapIds, visitados);
        }
    }
}

// Paso 3: Agregar todas las aristas
void ArbolB::agregarAristasAlGenerador(NodoB* nodo, DotGenerator& gen, const std::map<NodoB*, int>& mapIds, std::unordered_set<NodoB*>& visitados) {
    if (nodo == nullptr) {
        return;
    }

    if (visitados.count(nodo) > 0) {
        return;
    }
    visitados.insert(nodo);
    
    // Obtener ID del padre
    int parentId = mapIds.at(nodo);
    string parentStr = "node_" + to_string(parentId);
    
    // Agregar aristas a todos los hijos validos
    for (int i = 0; i <= nodo->Ocuenta(); i++) {
        if (nodo->Orama(i) != nullptr) {
            int childId = mapIds.at(nodo->Orama(i));
            string childStr = "node_" + to_string(childId);
            gen.addEdge(parentStr, childStr, "r" + to_string(i));
            
            // Recursivamente procesar hijos
            agregarAristasAlGenerador(nodo->Orama(i), gen, mapIds, visitados);
        }
    }
}