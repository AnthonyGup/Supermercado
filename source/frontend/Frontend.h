#ifndef FRONTEND_H
#define FRONTEND_H

#include <string>
#include <vector>
#include "pojo/Product.h"
#include "estructuras/avl/ArbolAvl.h"
#include "estructuras/b/ArbolB.h"
#include "estructuras/bplus/ArbolBPlus.h"
#include "estructuras/listas/normal/ListaEnlazada.h"
#include "estructuras/listas/ordenada/ListaOrdenada.h"

class Frontend {
private:
    ArbolAvl* arbolAVL;
    ArbolB* arbolB;
    ArbolBPlus* arbolBPlus;
    ListaEnlazada* lista;
    ListaOrdenada* listaOrdenada;
    bool isRunning;

public:
    /**
     * Constructor que recibe referencias a las estructuras de datos
     */
    Frontend(ArbolAvl* avl, ArbolB* b, ArbolBPlus* bplus, ListaEnlazada* l, ListaOrdenada* lo);
    
    ~Frontend();
    
    /**
     * Inicia el loop principal del frontend
     */
    void run();
    
private:
    /**
     * Muestra el menu principal
     */
    void showMainMenu();
    
    /**
     * Opcion 1: Insertar producto manual
     */
    void insertProduct();
    
    /**
     * Opcion 2: Insertar desde CSV
     */
    void insertFromCSV();
    
    /**
     * Opcion 3: Eliminar producto
     */
    void deleteProduct();
    
    /**
     * Opcion 4: Buscar producto
     */
    void searchProduct();
    
    /**
     * Opcion 5: Mostrar Listas
     */
    void showListsMenu();
    
    /**
     * Opcion 6: Exportar Arboles a .DOT
     */
    void exportTreesMenu();
    
    /**
        * Opcion 7: Medir latencias de busqueda
        */
        void measureQueryLatencies();
    
        /**
        * Opcion 8: Salir
     */
    void exitApp();
    

    
    /**
     * Lee un string del usuario
     */
    std::string readString(const std::string& prompt);
    
    /**
     * Lee un double del usuario
     */
    double readDouble(const std::string& prompt);
    
    /**
     * Lee un int del usuario
     */
    int readInteger(const std::string& prompt);
    
    /**
     * Lee una fecha en formato ISO (YYYY-MM-DD)
     */
    std::string readDate(const std::string& prompt);
    
    /**
     * Valida que sea una fecha valida en formato YYYY-MM-DD
     */
    bool isValidDate(const std::string& date);
    
    /**
     * Lee opcion numerica entre min y max
     */
    int readOption(int minOp, int maxOp);
    
    /**
     * Limpia la pantalla
     */
    void clearScreen();
    
    /**
     * Pausa la ejecucion
     */
    void pause();
    
    /**
     * Muestra un mensaje de exito
     */
    void showSuccess(const std::string& msg);
    
    /**
     * Muestra un mensaje de error
     */
    void showError(const std::string& msg);
    
    /**
     * Muestra un separador
     */
    void showSeparator();
};

#endif
