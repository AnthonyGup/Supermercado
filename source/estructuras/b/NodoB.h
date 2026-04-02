#ifndef NODO_B_H
#define NODO_B_H

#include "pojo/Product.h"

class NodoB {

    typedef NodoB* PPagina;
    typedef Product* tipoClave;

    protected:
        tipoClave* claves;  // Array de Product*
        PPagina* ramas;
        int cuenta;

    private:
        int m;

    public:
        NodoB(int orden);
        ~NodoB();

        bool nodoLLeno() const;
        bool nodoSemiVacio() const;
        
        tipoClave Oclave(int i) const;

        //Cambiar el product que ocupa la posicion i en el array
        void Pclave(int i, tipoClave producto);
        
        PPagina Orama(int i) const;

        // cambia la rama que ocupa lla posicion i en eel array de ramas
        void Prama(int i, PPagina p);
        int Ocuenta() const;
        
        // cambiar el valor de cuenta
        void Pcuenta(int valor);
};

#endif