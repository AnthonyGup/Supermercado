#include "ListaOrdenada.h"
#include "helpers/StringHelper.h"

#include <iostream>

void ListaOrdenada::desvincularNodo(NodoList *anterior, NodoList *actual)
{
    NodoList *siguiente = actual->getSiguiente();

    if (anterior != nullptr)
    {
        anterior->setSiguiente(siguiente);
    }
    else
    {
        this->cabeza = siguiente;
    }

    if (siguiente == nullptr)
    {
        this->cola = anterior;
    }
}

ListaOrdenada::~ListaOrdenada()
{
    limpiar();
}

bool ListaOrdenada::estaVacia() const
{
    return this->cabeza == nullptr;
}

int ListaOrdenada::getSize() const
{
    return this->size;
}

void ListaOrdenada::insertar(Product *dato)
{
    if (dato == nullptr)
    {
        throw ProductoNoEncontradoException("insertar", "dato nulo");
    }

    if (dato->getName().empty())
    {
        throw ProductoNoEncontradoException("insertar", "nombre vacio");
    }

    if (buscarNodo(dato->getName()) != nullptr)
    {
        throw ProductoNoEncontradoException("insertar", "ya existe un producto con nombre: " + dato->getName());
    }

    string nombreDato = StringHelper::toLowerCase(dato->getName());
    dato->setName(nombreDato);

    NodoList *nuevoNodo = new NodoList(dato);

    if (estaVacia())
    {
        this->cabeza = nuevoNodo;
        this->cola = nuevoNodo;
        this->size++;
        return;
    }

    if (this->cabeza->getDato() == nullptr ||
        nombreDato <= StringHelper::toLowerCase(this->cabeza->getDato()->getName()))
    {
        nuevoNodo->setSiguiente(this->cabeza);
        this->cabeza = nuevoNodo;
        this->size++;
        return;
    }

    NodoList *actual = this->cabeza;
    while (actual->getSiguiente() != nullptr &&
           actual->getSiguiente()->getDato() != nullptr &&
            StringHelper::toLowerCase(actual->getSiguiente()->getDato()->getName()) < nombreDato)
    {
        actual = actual->getSiguiente();
    }

    if (actual == nullptr)
    {
        throw ProductoNoEncontradoException("insertar", "la estructura de la lista es invalida");
    }

    NodoList *siguiente = actual->getSiguiente();

    nuevoNodo->setSiguiente(siguiente);
    actual->setSiguiente(nuevoNodo);

    if (siguiente == nullptr)
    {
        this->cola = nuevoNodo;
    }

    this->size++;
}

void ListaOrdenada::eliminar(const string &nombre)
{
    if (nombre.empty())
    {
        throw ProductoNoEncontradoException("eliminar", "nombre vacio");
    }

    string nombreBuscado = StringHelper::toLowerCase(nombre);

    NodoList *anterior = nullptr;
    NodoList *actual = this->cabeza;

    while (actual != nullptr)
    {
        if (actual->getDato() == nullptr)
        {
            anterior = actual;
            actual = actual->getSiguiente();
            continue;
        }

        string nombreActual = StringHelper::toLowerCase(actual->getDato()->getName());
        if (nombreActual == nombreBuscado)
        {
            desvincularNodo(anterior, actual);
            delete actual;
            this->size--;
            return;
        }

        if (nombreActual > nombreBuscado)
        {
            break;
        }

        anterior = actual;
        actual = actual->getSiguiente();
    }

    throw ProductoNoEncontradoException("eliminar", "no existe un producto con nombre: " + nombre);
}

NodoList *ListaOrdenada::buscarNodo(const string &nombre)
{
    string nombreBuscado = StringHelper::toLowerCase(nombre);
    NodoList *actual = this->cabeza;

    while (actual != nullptr)
    {
        if (actual->getDato() == nullptr)
        {
            actual = actual->getSiguiente();
            continue;
        }

        string nombreActual = StringHelper::toLowerCase(actual->getDato()->getName());
        if (nombreActual == nombreBuscado)
        {
            return actual;
        }

        if (nombreActual > nombreBuscado)
        {
            return nullptr;
        }

        actual = actual->getSiguiente();
    }

    return nullptr;
}

void ListaOrdenada::mostrar() const
{
    if (estaVacia())
    {
        cout << "Lista ordenada vacia." << endl;
        return;
    }

    cout << "Lista ordenada (" << this->size << " productos):" << endl;
    NodoList *actual = this->cabeza;
    while (actual != nullptr)
    {
        Product *producto = actual->getDato();
        if (producto != nullptr)
        {
            cout << "- Nombre: " << producto->getName()
                 << " | Codigo: " << producto->getBarcode()
                 << " | Categoria: " << producto->getCategory()
                 << " | Vencimiento: " << producto->getExpiry_date()
                 << " | Marca: " << producto->getBrand()
                 << " | Precio: " << producto->getPrice()
                 << " | Stock: " << producto->getStock() << endl;
        }
        actual = actual->getSiguiente();
    }
}

void ListaOrdenada::limpiar()
{
    NodoList *actual = this->cabeza;

    while (actual != nullptr)
    {
        NodoList *siguiente = actual->getSiguiente();
        delete actual;
        actual = siguiente;
    }

    this->cabeza = nullptr;
    this->cola = nullptr;
    this->size = 0;
}
