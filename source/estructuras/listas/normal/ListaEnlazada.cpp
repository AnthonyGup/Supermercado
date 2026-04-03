#include "ListaEnlazada.h"
#include "helpers/StringHelper.h"

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

	NodoList *nuevoNodo = new NodoList(dato);

	if (estaVacia())
	{
		this->cabeza = nuevoNodo;
		this->cola = nuevoNodo;
	}
	else
	{
		if (this->cola == nullptr)
		{
			delete nuevoNodo;
			throw ProductoNoEncontradoException("insertar", "la estructura de la lista es invalida");
		}

		this->cola->setSiguiente(nuevoNodo);
		this->cola = nuevoNodo;
	}

	this->size++;
}

void ListaEnlazada::eliminar(const string &nombre)
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
		if (actual->getDato() != nullptr &&
			StringHelper::toLowerCase(actual->getDato()->getName()) == nombreBuscado)
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
	string nombreBuscado = StringHelper::toLowerCase(nombre);

	NodoList *actual = this->cabeza;
	while (actual != nullptr)
	{
		if (actual->getDato() != nullptr &&
			StringHelper::toLowerCase(actual->getDato()->getName()) == nombreBuscado)
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
			cout << "- " << producto->getName() << endl;
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
