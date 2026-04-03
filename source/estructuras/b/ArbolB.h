#ifndef ARBOL_B_H
#define ARBOL_B_H

#include "NodoB.h"
#include "helpers/DotGenerator.h"
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <unordered_set>

using namespace std;

class ArbolB {
	protected:
		int orden;
		NodoB *raiz;
	public:
		explicit ArbolB();
		ArbolB(int m);
		~ArbolB();

		// Buscar un producto por su expiry_date
		Product* buscar(const string& clave);
		void insertar(Product* producto);
		// revisar la forma de eliminacion
		void eliminar(const string& clave);

		bool buscarNodo(NodoB* actual, Product* producto, int& k);
		bool empujar(NodoB* actual, Product* producto, Product*& productoMediana, NodoB*& nuevo);
		void insertarNoLleno(NodoB* nodo, Product* producto);
		void dividirHijo(NodoB* padre, int indiceHijo);
		
		// Generacion de DOT para visualizacion
		bool generarDot(const string& filepath);

	private:
	// Métodos auxiliares para generar DOT
		void asignarIdsRecursivo(NodoB* nodo, int& contador, std::map<NodoB*, int>& mapIds);
		void agregarNodosAlGenerador(NodoB* nodo, DotGenerator& gen, const std::map<NodoB*, int>& mapIds, std::unordered_set<NodoB*>& visitados);
		void agregarAristasAlGenerador(NodoB* nodo, DotGenerator& gen, const std::map<NodoB*, int>& mapIds, std::unordered_set<NodoB*>& visitados);
		
		// Metodos de la clase
		Product* buscar(NodoB* actual, const string& clave, int& n);
		NodoB* insertar(NodoB* raiz, Product* producto);
		void meterPagina(NodoB* actual, Product* producto, NodoB* ramaDr, int k);
		void dividirNodo(NodoB* actual, Product*& mediana, NodoB*& nuevo, int pos);
		void inOrder(NodoB* r);
		NodoB* eliminar(NodoB* nodo, const string& clave);
		void llenarNodo(NodoB* nodo, int k);
		void tomarDelAnterior(NodoB* nodo, int pos);
		void tomarDelSiguiente(NodoB* nodo, int pos);
		void fusionarNodos(NodoB* nodo, int k);
		
		// Métodos auxiliares para transacciones
		NodoB* cloneNodo(NodoB* nodo);
		void eliminarNodo(NodoB* nodo);
		
};

#endif
