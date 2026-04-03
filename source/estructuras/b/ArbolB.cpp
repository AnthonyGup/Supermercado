#include "ArbolB.h"
#include <stdexcept>

ArbolB::ArbolB() {
    this->orden = 0;
    this->raiz = nullptr;
    this->raizBackup = nullptr;
}

ArbolB::ArbolB(int m) {
    this->orden = m;
    this->raiz = nullptr;
    this->raizBackup = nullptr;
}

bool ArbolB::arbolBvacio() {
    return this->raiz == nullptr;
}

NodoB* ArbolB::Oraiz() {
    return this->raiz;
}

void ArbolB::Praiz(NodoB* r) {
    this->raiz = r;
}

int ArbolB::Oorden() {
    return this->orden;
}

void ArbolB::Porden(int ord) {
    this->orden = ord;
}

void ArbolB::crear() {
    orden = 0;
    raiz = nullptr;
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
    raiz = insertar(raiz, cl);
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
    int i, posMdna, k;
    NodoB* nuevaPag;
    k = pos;
    // posision de clave mediana
    posMdna = (k <= orden/2) ? orden/2 : orden/2 + 1;
    nuevaPag = new NodoB(orden);
    int cnt = 1;
    for (i = posMdna + 1; i < orden; i++) {
        /* desplazada la mitad derecha a el nuevo NodoB, la clave 
           mediana se queda en NodoB actual */
        nuevaPag->Pclave(cnt, actual->Oclave(i));
        nuevaPag->Prama(cnt - 1, actual->Orama(i));
        cnt++;
    }
    nuevaPag->Pcuenta((orden-1) - posMdna); // claves de nuevo nodo
    actual->Pcuenta(posMdna); // claves en NodoB origen
    // inserta la clave y rama en el NodoB que le corresponde
    if (k <= orden/2) {
        meterPagina(actual, mediana, nuevo, pos); // en NodoB origen
    } else {
        pos = k - posMdna;
        meterPagina(nuevaPag, mediana, nuevo, pos); // en NodoB nueva
    }
    // extrae clave mediana del NodoB origen
    mediana = actual->Oclave(actual->Ocuenta());
    // Rama0 del nuevo nodo ees la rama de la mediana 
    nuevaPag->Prama(0, actual->Orama(actual->Ocuenta()));
    actual->Pcuenta(actual->Ocuenta() -1); // se quita la mediana
    nuevo = nuevaPag; // devuelve el nuevo NodoB
}


void ArbolB::listarCreciente() {
    inOrder(raiz);
}

void ArbolB::inOrder(NodoB* r) {
    if (r) {
        inOrder(r->Orama(0));
        for(int k = 1; k <= r->Ocuenta(); k++) {
            cout << r->Oclave(k)->getName() << " ";
            inOrder(r->Orama(k));
        }
    }
}

NodoB* ArbolB::eliminar(NodoB* nodo, const string& clave) {
    if (nodo == nullptr) {
        return nullptr;
    }

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
            if (nodo->Orama(k-1)->Ocuenta() >= orden / 2 + 1) {
                // El hijo izquierdo tiene suficientes claves, obtener predecesor
                NodoB* pred = nodo->Orama(k-1);
                while (!pred->nodoSemiVacio()) {
                    pred = pred->Orama(pred->Ocuenta());
                }
                Product* predecesor = pred->Oclave(pred->Ocuenta());
                nodo->Pclave(k, predecesor);
                nodo->Prama(k-1, eliminar(nodo->Orama(k-1), predecesor->getExpiry_date()));
            } else if (nodo->Orama(k)->Ocuenta() >= orden / 2 + 1) {
                // El hijo derecho tiene suficientes claves, obtener sucesor
                NodoB* succ = nodo->Orama(k);
                while (!succ->nodoSemiVacio()) {
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
            if (nodo->Orama(k)->Ocuenta() < orden / 2 + 1) {
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
    // Si el hijo anterior tiene más del mínimo de claves, tomar una de él
    if (k != 0 && nodo->Orama(k)->Ocuenta() < orden / 2 + 1 && 
        nodo->Orama(k-1)->Ocuenta() >= orden / 2 + 1) {
        tomarDelAnterior(nodo, k);
    }
    // Si el hijo siguiente tiene más del mínimo de claves, tomar una de él
    else if (k != nodo->Ocuenta() && nodo->Orama(k)->Ocuenta() < orden / 2 + 1 && 
             nodo->Orama(k+1)->Ocuenta() >= orden / 2 + 1) {
        tomarDelSiguiente(nodo, k);
    }
    // Si ambos hermanos tienen el mínimo, fusionar
    else if (k != 0 && nodo->Orama(k)->Ocuenta() < orden / 2 + 1) {
        fusionarNodos(nodo, k);
    } else if (k != nodo->Ocuenta() && nodo->Orama(k)->Ocuenta() < orden / 2 + 1) {
        fusionarNodos(nodo, k+1);
    }
}

void ArbolB::tomarDelAnterior(NodoB* nodo, int pos) {
    NodoB* hijo = nodo->Orama(pos);
    NodoB* hermanoAnterior = nodo->Orama(pos - 1);

    // Desplazar las claves del hijo a la derecha
    for (int i = hijo->Ocuenta(); i > 0; i--) {
        hijo->Pclave(i + 1, hijo->Oclave(i));
        if (!hijo->nodoSemiVacio()) {
            hijo->Prama(i + 1, hijo->Orama(i));
        }
    }
    if (!hijo->nodoSemiVacio()) {
        hijo->Prama(1, hijo->Orama(0));
    }

    hijo->Pclave(1, nodo->Oclave(pos));
    if (!nodo->nodoSemiVacio()) {
        hijo->Prama(0, hermanoAnterior->Orama(hermanoAnterior->Ocuenta()));
    }

    nodo->Pclave(pos, hermanoAnterior->Oclave(hermanoAnterior->Ocuenta()));
    hijo->Pcuenta(hijo->Ocuenta() + 1);
    hermanoAnterior->Pcuenta(hermanoAnterior->Ocuenta() - 1);
}

void ArbolB::tomarDelSiguiente(NodoB* nodo, int pos) {
    NodoB* hijo = nodo->Orama(pos);
    NodoB* hermanoSiguiente = nodo->Orama(pos + 1);

    hijo->Pclave(hijo->Ocuenta() + 1, nodo->Oclave(pos + 1));
    if (!hijo->nodoSemiVacio()) {
        hijo->Prama(hijo->Ocuenta() + 1, hermanoSiguiente->Orama(0));
    }

    nodo->Pclave(pos + 1, hermanoSiguiente->Oclave(1));
    for (int i = 1; i < hermanoSiguiente->Ocuenta(); i++) {
        hermanoSiguiente->Pclave(i, hermanoSiguiente->Oclave(i + 1));
        if (!hermanoSiguiente->nodoSemiVacio()) {
            hermanoSiguiente->Prama(i, hermanoSiguiente->Orama(i + 1));
        }
    }

    hijo->Pcuenta(hijo->Ocuenta() + 1);
    hermanoSiguiente->Pcuenta(hermanoSiguiente->Ocuenta() - 1);
}

void ArbolB::fusionarNodos(NodoB* nodo, int k) {
    NodoB* hijo = nodo->Orama(k - 1);
    NodoB* hermano = nodo->Orama(k);

    hijo->Pclave(hijo->Ocuenta() + 1, nodo->Oclave(k));
    for (int i = 1; i <= hermano->Ocuenta(); i++) {
        hijo->Pclave(hijo->Ocuenta() + 1, hermano->Oclave(i));
        if (!hermano->nodoSemiVacio()) {
            hijo->Prama(hijo->Ocuenta(), hermano->Orama(i - 1));
        }
    }
    if (!hermano->nodoSemiVacio()) {
        hijo->Prama(hijo->Ocuenta(), hermano->Orama(hermano->Ocuenta()));
    }

    hijo->Pcuenta(hijo->Ocuenta() + hermano->Ocuenta() + 1);

    // Desplazar las claves y ramas del nodo padre
    for (int i = k; i < nodo->Ocuenta(); i++) {
        nodo->Pclave(i, nodo->Oclave(i + 1));
        nodo->Prama(i, nodo->Orama(i + 1));
    }

    nodo->Pcuenta(nodo->Ocuenta() - 1);
    delete hermano;
}

ArbolB::~ArbolB() {
    eliminarNodo(raiz);
    eliminarNodo(raizBackup);
}

void ArbolB::hacerBackup() {
    // Limpiar backup anterior si existe
    if (raizBackup != nullptr) {
        eliminarNodo(raizBackup);
    }
    // Crear copia profunda de la raíz actual
    raizBackup = cloneNodo(raiz);
}

void ArbolB::restaurarBackup() {
    if (raizBackup == nullptr) {
        throw std::runtime_error("no hay backup disponible");
    }
    // Eliminar el árbol actual
    eliminarNodo(raiz);
    // Restaurar desde el backup
    raiz = cloneNodo(raizBackup);
}

void ArbolB::limpiarBackup() {
    if (raizBackup != nullptr) {
        eliminarNodo(raizBackup);
        raizBackup = nullptr;
    }
}

NodoB* ArbolB::cloneNodo(NodoB* nodo) {
    if (nodo == nullptr) {
        return nullptr;
    }
    
    // Crear nuevo nodo con el mismo orden
    NodoB* nuevoNodo = new NodoB(nodo->Ocuenta() > 0 ? orden : orden);
    
    // Copiar cuenta
    nuevoNodo->Pcuenta(nodo->Ocuenta());
    
    // Copiar claves
    for (int i = 1; i <= nodo->Ocuenta(); i++) {
        nuevoNodo->Pclave(i, nodo->Oclave(i));
    }
    
    // Clonar recursivamente las ramas
    if (!nodo->nodoSemiVacio()) {
        for (int i = 0; i <= nodo->Ocuenta(); i++) {
            NodoB* ramaClonada = cloneNodo(nodo->Orama(i));
            nuevoNodo->Prama(i, ramaClonada);
        }
    }
    
    return nuevoNodo;
}

void ArbolB::eliminarNodo(NodoB* nodo) {
    if (nodo == nullptr) {
        return;
    }
    
    // Eliminar recursivamente todos los hijos
    for (int i = 0; i <= nodo->Ocuenta() && !nodo->nodoSemiVacio(); i++) {
        eliminarNodo(nodo->Orama(i));
    }
    
    // Eliminar el nodo actual
    delete nodo;
}
// =============== MÉTODOS DE GENERACIÓN DOT ===============

