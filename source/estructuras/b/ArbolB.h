#ifndef ARBOL_B_H
#define ARBOL_B_H

#include "NodoB.h"
#include <string>
#include "iostream"

using namespace std;

class ArbolB {
	protected:
		int orden;
		NodoB *raiz;
		NodoB *raizBackup;  // Backup para transacciones
	public:
		explicit ArbolB();
		ArbolB(int m);
		~ArbolB();

		bool arbolBvacio();
		NodoB* Oraiz();
		void Praiz(NodoB* r);
		int Oorden();
		void Porden(int ord);
		void crear();
		// Buscar un producto por su expiry_date
		Product* buscar(const string& clave);
		void insertar(Product* producto);
		// revisar la forma de eliminacion
		void eliminar(const string& clave);

		// Métodos para transacaciones y rollback
		void hacerBackup();
		void restaurarBackup();
		void limpiarBackup();

		bool buscarNodo(NodoB* actual, Product* producto, int& k);
		bool empujar(NodoB* actual, Product* producto, Product*& productoMediana, NodoB*& nuevo);
		void escribir();
		void listarCreciente();

	private:
		Product* buscar(NodoB* actual, const string& clave, int& n);
		NodoB* insertar(NodoB* raiz, Product* producto);
		void meterPagina(NodoB* actual, Product* producto, NodoB* ramaDr, int k);
		void dividirNodo(NodoB* actual, Product*& mediana, NodoB*& nuevo, int pos);
		void escribir(NodoB* r, int h);
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
