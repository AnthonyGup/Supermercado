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

    if(nodo->getFe() > 1) {
        if (nodo2->getFe() >= 0) {
            //DD hacer rotacion I
            return rotarI(nodo, nodo2);
        } else {
            //DI hacer rotacion DI
            return rotarDI(nodo, nodo2, nodo2->getIzquierdo());
        }
    } else if(nodo->getFe() < (-1)) {
        if (nodo2->getFe() <= 0) {
            //II hacer rotacion D
            return rotarD(nodo, nodo2);
        } else {
            //ID hacer rotacion ID
            return rotarID(nodo, nodo2, nodo2->getDerecho());
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

NodoAvl* ArbolAvl::rotarDI(NodoAvl* nodo1, NodoAvl* nodo2, NodoAvl* nodo3) {
    if (nodo3 == nullptr) {
        throw ProductoNoEncontradoException("rotar", "rotacion DI invalida");
    }

    NodoAvl* temp = nodo3->getIzquierdo();
    NodoAvl* temp2 = nodo3->getDerecho();

    nodo3->setDerecho(nodo2);
    nodo3->setIzquierdo(nodo1);   

    nodo1->setDerecho(temp);
    nodo2->setIzquierdo(temp2);

    actualizarFe(nodo1);
    actualizarFe(nodo2);
    actualizarFe(nodo3);

    return nodo3;
}

NodoAvl* ArbolAvl::rotarID(NodoAvl* nodo1, NodoAvl* nodo2, NodoAvl* nodo3) {
    if (nodo3 == nullptr) {
        throw ProductoNoEncontradoException("rotar", "rotacion ID invalida");
    }

    NodoAvl* temp = nodo3->getIzquierdo();
    NodoAvl* temp2 = nodo3->getDerecho();

    nodo3->setIzquierdo(nodo2);
    nodo3->setDerecho(nodo1);

    nodo1->setIzquierdo(temp);
    nodo2->setDerecho(temp2);

    actualizarFe(nodo1);
    actualizarFe(nodo2);
    actualizarFe(nodo3);

    return nodo3;
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
    if (nodo == nullptr) {
        throw ProductoNoEncontradoException("eliminar", "no existe un producto con nombre: " + valor->getName());
    }

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
        if (temp == nullptr || temp->getDato() == nullptr) {
            throw ProductoNoEncontradoException("eliminar", "estructura AVL invalida al reemplazar nodo");
        }

        if (nodo->getDato() == nullptr) {
            nodo->setDato(new Product());
        }

        *(nodo->getDato()) = *(temp->getDato());
        nodo->setIzquierdo(eliminarNodoRecursivo(nodo->getIzquierdo(), temp->getDato()));
    }

    actualizarFe(nodo);

    if (nodo->getFe() > 1 || nodo->getFe() < -1) {
        nodo = evaluarRotacion(nodo);
    }

    return nodo;
}

bool ArbolAvl::buscarNodoRecursivo(NodoAvl* nodo, const string& valor) const {
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

ArbolAvl::~ArbolAvl() {
    liberarRecursivo(raiz);
    raiz = nullptr;
}

void ArbolAvl::liberarRecursivo(NodoAvl* nodo) {
    if (nodo == nullptr) {
        return;
    }

    liberarRecursivo(nodo->getIzquierdo());
    liberarRecursivo(nodo->getDerecho());
    delete nodo;
}

void ArbolAvl::insertar(Product* valor) {
    if (valor == nullptr) {
        throw ProductoNoEncontradoException("insertar", "dato nulo en AVL");
    }

    if (valor->getName().empty()) {
        throw ProductoNoEncontradoException("insertar", "nombre vacio en AVL");
    }

    if (buscar(valor->getName())) {
        throw ProductoNoEncontradoException("insertar", "ya existe un producto con nombre: " + valor->getName());
    }

    raiz = insertarNodoRecursivo(raiz, valor);
}

void ArbolAvl::eliminar(Product* valor) {
    if (valor == nullptr) {
        throw ProductoNoEncontradoException("eliminar", "dato nulo en AVL");
    }

    if (valor->getName().empty()) {
        throw ProductoNoEncontradoException("eliminar", "nombre vacio en AVL");
    }

    raiz = eliminarNodoRecursivo(raiz, valor);
}

bool ArbolAvl::buscar(const string& nombre) const {
    if (nombre.empty()) {
        return false;
    }

    return buscarNodoRecursivo(raiz, nombre);
}

Product* ArbolAvl::obtenerProducto(const string& nombre) const {
    if (nombre.empty()) {
        return nullptr;
    }

    NodoAvl* nodo = raiz;
    while (nodo != nullptr) {
        if (!dato1MayorDato2(nodo->getDato()->getName(), nombre) &&
            !dato1MayorDato2(nombre, nodo->getDato()->getName())) {
            return nodo->getDato();
        }

        if (dato1MayorDato2(nodo->getDato()->getName(), nombre)) {
            nodo = nodo->getIzquierdo();
        } else {
            nodo = nodo->getDerecho();
        }
    }

    return nullptr;
}

// =============== MÉTODOS DE GENERACIÓN DOT ===============

bool ArbolAvl::generarDot(const string& filepath) {
    try {
        DotGenerator gen("arbol_avl", "Arbol AVL de Productos (Orden 2, por Nombre)");
        
        if (raiz == nullptr) {
            gen.addNode("empty", "Árbol Vacío", "ellipse", "lightgray");
        } else {
            // Paso 1: Asignar IDs secuenciales a todos los nodos
            int contador = 0;
            map<NodoAvl*, int> mapIds;
            asignarIdsRecursivo(raiz, contador, mapIds);
            
            // Paso 2: Agregar todos los nodos al generador
            agregarNodosAlGenerador(raiz, gen, mapIds);
            
            // Paso 3: Agregar todas las aristas
            agregarAristasAlGenerador(raiz, gen, mapIds);
        }
        
        return gen.saveToDot(filepath);
    } catch (const exception& e) {
        cerr << "Error al generar DOT del Árbol AVL: " << e.what() << endl;
        return false;
    }
}

// Paso 1: Asignar IDs únicos y correlativos a cada nodo
void ArbolAvl::asignarIdsRecursivo(NodoAvl* nodo, int& contador, map<NodoAvl*, int>& mapIds) {
    if (nodo == nullptr) {
        return;
    }
    
    // Pre-order: asignar ID primero
    contador++;
    mapIds[nodo] = contador;
    
    // Luego procesar izquierda y derecha
    asignarIdsRecursivo(nodo->getIzquierdo(), contador, mapIds);
    asignarIdsRecursivo(nodo->getDerecho(), contador, mapIds);
}

// Paso 2: Agregar todos los nodos definidos correctamente
void ArbolAvl::agregarNodosAlGenerador(NodoAvl* nodo, DotGenerator& gen, const map<NodoAvl*, int>& mapIds) {
    if (nodo == nullptr) {
        return;
    }
    
    // Obtener el ID de este nodo
    int nodeId = mapIds.at(nodo);
    string id = "node_" + to_string(nodeId);
    
    // Crear etiqueta clara y estructurada
    stringstream label;
    label << nodo->getDato()->getName() << "\n";
    label << "$" << static_cast<int>(nodo->getDato()->getPrice()) << "\n";
    label << "FE: " << nodo->getFe();
    
    // Determinar color según balance factor
    string color = "lightblue";
    if (nodo->getFe() > 1 || nodo->getFe() < -1) {
        color = "lightcoral";  // Desbalanceado crítico
    } else if (nodo->getFe() != 0) {
        color = "lightyellow"; // Ligeramente desbalanceado
    }
    
    gen.addNode(id, label.str(), "box", color);
    
    // Recursivamente agregar nodos hijos
    agregarNodosAlGenerador(nodo->getIzquierdo(), gen, mapIds);
    agregarNodosAlGenerador(nodo->getDerecho(), gen, mapIds);
}

// Paso 3: Agregar todas las aristas
void ArbolAvl::agregarAristasAlGenerador(NodoAvl* nodo, DotGenerator& gen, const map<NodoAvl*, int>& mapIds) {
    if (nodo == nullptr) {
        return;
    }
    
    // Obtener ID del padre
    int parentId = mapIds.at(nodo);
    string parentStr = "node_" + to_string(parentId);
    
    // Agregar arista al hijo izquierdo
    if (nodo->getIzquierdo() != nullptr) {
        int leftId = mapIds.at(nodo->getIzquierdo());
        string leftStr = "node_" + to_string(leftId);
        gen.addEdge(parentStr, leftStr, "←");
        agregarAristasAlGenerador(nodo->getIzquierdo(), gen, mapIds);
    }
    
    // Agregar arista al hijo derecho
    if (nodo->getDerecho() != nullptr) {
        int rightId = mapIds.at(nodo->getDerecho());
        string rightStr = "node_" + to_string(rightId);
        gen.addEdge(parentStr, rightStr, "→");
        agregarAristasAlGenerador(nodo->getDerecho(), gen, mapIds);
    }
}

