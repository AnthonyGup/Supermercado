#include <iostream>
#include <string>
#include <vector>
#include "helpers/CSVLoader.h"
#include "estructuras/avl/ArbolAvl.h"
#include "estructuras/b/ArbolB.h"
#include "estructuras/bplus/ArbolBPlus.h"
#include "estructuras/listas/normal/ListaEnlazada.h"
#include "estructuras/listas/ordenada/ListaOrdenada.h"
#include "frontend/Frontend.h"

using namespace std;

int main() {
    try {
        // Crear las estructuras de datos
        ArbolAvl* arbolAVL = new ArbolAvl();
        ArbolB* arbolB = new ArbolB(4);
        ArbolBPlus* arbolBPlus = new ArbolBPlus(4);
        ListaEnlazada* lista = new ListaEnlazada();
        ListaOrdenada* listaOrdenada = new ListaOrdenada();
        
        // Crear y ejecutar el frontend
        Frontend frontend(arbolAVL, arbolB, arbolBPlus, lista, listaOrdenada);
        frontend.run();
        
        // Liberar memoria
        delete arbolAVL;
        delete arbolB;
        delete arbolBPlus;
        delete lista;
        delete listaOrdenada;
        
        return 0;
        
    } catch (const exception& e) {
        cerr << "Error crítico: " << e.what() << endl;
        return 1;
    }
}