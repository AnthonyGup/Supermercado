#include <iostream>
#include "pojo/Product.h"

class NodoAvl {
    private:
        Product* dato;
        NodoAvl* derecho;
        NodoAvl* izquierdo;
        int fe;
        int altura;

    public:
        NodoAvl(Product *valor);
        ~NodoAvl();

        // GETTERS
        Product* getDato() const;
        NodoAvl* getDerecho() const;
        NodoAvl* getIzquierdo() const;
        int getFe() const;
        int getAltura() const;

        // SETTERS
        void setDato(Product* dato);
        void setDerecho(NodoAvl* derecho);
        void setIzquierdo(NodoAvl* izquierdo);
        void setFe(int fe);
        void setAltura(int altura);

};