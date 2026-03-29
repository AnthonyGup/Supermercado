#include "NodoAvl.h"

NodoAvl::NodoAvl(Product *valor)
{
    this->dato = valor;
    izquierdo = nullptr;
    derecho = nullptr;
    fe = 0;
    altura = 1;
}

NodoAvl::~NodoAvl()
{
    delete this->dato;
    this->izquierdo = nullptr;
    this->derecho = nullptr;
}

int NodoAvl::getAltura() const
{
    return this->altura;
}

Product *NodoAvl::getDato() const
{
    return this->dato;
}

NodoAvl *NodoAvl::getDerecho() const
{
    return this->derecho;
}

NodoAvl *NodoAvl::getIzquierdo() const
{
    return this->izquierdo;
}

int NodoAvl::getFe() const
{
    return this->fe;
}

void NodoAvl::setDato(Product *dato)
{
    this->dato = dato;
}

void NodoAvl::setDerecho(NodoAvl *derecho)
{
    this->derecho = derecho;
}

void NodoAvl::setIzquierdo(NodoAvl *izquierdo)
{
    this->izquierdo = izquierdo;
}

void NodoAvl::setFe(int fe)
{
    this->fe = fe;
}

void NodoAvl::setAltura(int altura)
{
    this->altura = altura;
}