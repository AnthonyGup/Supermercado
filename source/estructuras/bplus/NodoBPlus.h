#ifndef NODO_B_PLUS_H
#define NODO_B_PLUS_H

#include "pojo/Product.h"
#include "listas/normal/ListaEnlazada.h"
#include <string>

using namespace std;

// Forward declaration
class NodoBPlus;
typedef NodoBPlus* PPagina;
typedef string tipoClave;
typedef ListaEnlazada* tipoValor;

class NodoBPlus {

    protected:
        tipoClave* claves;          // Array de categorías (strings)
        PPagina* ramas;             // Array de ramas (para nodos internos)
        tipoValor* valores;         // Array de listas de productos (para hojas)
        PPagina siguiente;          // Puntero al siguiente nodo (para hojas)
        bool isHoja;                // Indica si el nodo es hoja
        int cuenta;                 // Cantidad de claves
        int m;                      // Orden del árbol

    public:
        NodoBPlus(int orden, bool hoja = false);
        ~NodoBPlus();

        // Getters
        tipoClave Oclave(int i) const;
        PPagina Orama(int i) const;
        tipoValor Ovalor(int i) const;
        PPagina OramaSiguiente() const;
        bool esHoja() const;
        int Ocuenta() const;
        int Oorden() const;

        // Setters
        void Pclave(int i, tipoClave clave);
        void Prama(int i, PPagina p);
        void Pvalor(int i, tipoValor lista);
        void PramaSiguiente(PPagina p);
        void Pcuenta(int valor);

        // Verificadores
        bool nodoLLeno() const;
        bool nodoSemiVacio() const;

        // Operaciones auxiliares
        int buscarPosicion(tipoClave clave) const;
        void agregarProductoEnHoja(tipoValor lista, Product* producto);
};

#endif
