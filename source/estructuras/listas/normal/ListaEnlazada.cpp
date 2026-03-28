#include "ListaEnlazada.h"

#include <iostream>

void ListaEnlazada::desvincularNodo(NodoList *anterior, NodoList *actual)
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

ListaEnlazada::~ListaEnlazada()
{
	limpiar();
}

bool ListaEnlazada::estaVacia() const
{
	return this->cabeza == nullptr;
}

int ListaEnlazada::getSize() const
{
	return this->size;
}

void ListaEnlazada::insertar(Product *dato)
{
	NodoList *nuevoNodo = new NodoList(dato);

	if (estaVacia())
	{
		this->cabeza = nuevoNodo;
		this->cola = nuevoNodo;
	}
	else
	{
		this->cola->setSiguiente(nuevoNodo);
		this->cola = nuevoNodo;
	}

	this->size++;
}

void ListaEnlazada::eliminar(const string &nombre)
{
	NodoList *anterior = nullptr;
	NodoList *actual = this->cabeza;

	while (actual != nullptr)
	{
		if (actual->getDato() != nullptr && actual->getDato()->getName() == nombre)
		{
			desvincularNodo(anterior, actual);
			delete actual;
			this->size--;
			return;
		}
		anterior = actual;
		actual = actual->getSiguiente();
	}

	throw ProductoNoEncontradoException("eliminar", "no existe un producto con nombre: " + nombre);
}

NodoList *ListaEnlazada::buscarNodo(const string &nombre)
{
	NodoList *actual = this->cabeza;
	while (actual != nullptr)
	{
		if (actual->getDato() != nullptr && actual->getDato()->getName() == nombre)
		{
			return actual;
		}
		actual = actual->getSiguiente();
	}
	return nullptr;
}

void ListaEnlazada::mostrar() const
{
	if (estaVacia())
	{
		cout << "Lista enlazada vacia." << endl;
		return;
	}

	cout << "Lista enlazada (" << this->size << " productos):" << endl;
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

void ListaEnlazada::limpiar()
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
