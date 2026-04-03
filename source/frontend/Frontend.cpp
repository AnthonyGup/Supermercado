#include "Frontend.h"
#include "../source/helpers/CSVLoader.h"
#include "../source/helpers/StringHelper.h"
#include "../source/estructuras/listas/Nodo/NodoList.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <regex>
#include <cctype>
#include <chrono>

Frontend::Frontend(ArbolAvl* avl, ArbolB* b, ArbolBPlus* bplus, ListaEnlazada* l, ListaOrdenada* lo)
    : arbolAVL(avl), arbolB(b), arbolBPlus(bplus), lista(l), listaOrdenada(lo), isRunning(true) {}

Frontend::~Frontend() = default;

void Frontend::run() {
    while (isRunning) {
        showMainMenu();
    }
}

void Frontend::showMainMenu() {
    clearScreen();
    
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "  SISTEMA DE GESTION - SUPERMERCADO\n";
    std::cout << "========================================\n\n";
    
    std::cout << "1. Insertar Producto\n";
    std::cout << "2. Insertar desde CSV\n";
    std::cout << "3. Eliminar Producto\n";
    std::cout << "4. Buscar Producto\n";
    std::cout << "5. Exportar Arbol AVL a .DOT\n";
    std::cout << "6. Salir\n\n";
    
    int option = readOption(1, 6);
    
    switch (option) {
        case 1:
            insertProduct();
            break;
        case 2:
            insertFromCSV();
            break;
        case 3:
            deleteProduct();
            break;
        case 4:
            searchProduct();
            break;
        case 5:
            exportAVLToDot();
            break;
        case 6:
            exitApp();
            break;
    }
}

void Frontend::insertProduct() {
    clearScreen();
    showSeparator();
    std::cout << "INSERTAR NUEVO PRODUCTO\n";
    showSeparator();
    
    std::string nombre = readString("Nombre del producto: ");
    std::string codigo = readString("Codigo de barras: ");
    std::string categoria = readString("Categoria: ");
    std::string fecha = readDate("Fecha de vencimiento (YYYY-MM-DD): ");
    std::string marca = readString("Marca: ");
    double precio = readDouble("Precio: ");
    int stock = readInteger("Stock: ");
    
    // Crear producto
    Product* producto = new Product();
    producto->setName(nombre);
    producto->setBarcode(codigo);
    producto->setCategory(categoria);
    producto->setExpiry_date(fecha);
    producto->setBrand(marca);
    producto->setPrice(precio);
    producto->setStock(stock);
    
    // Insertar en todas las estructuras
    try {
        if (arbolAVL) {
            arbolAVL->insertar(producto);
        }
        if (arbolB) {
            arbolB->insertar(producto);
        }
        if (arbolBPlus) {
            arbolBPlus->insertar(producto);
        }
        if (lista) {
            lista->insertar(producto);
        }
        if (listaOrdenada) {
            listaOrdenada->insertar(producto);
        }
        
        showSuccess("Producto insertado en todas las estructuras de datos.");
    } catch (const std::exception& e) {
        showError(std::string("Error al insertar producto: ") + e.what());
        delete producto;
    }
    pause();
}

void Frontend::insertFromCSV() {
    clearScreen();
    showSeparator();
    std::cout << "INSERTAR DESDE ARCHIVO CSV\n";
    showSeparator();
    
    std::string rutaCSV = readString("Ruta del archivo CSV: ");
    
    std::cout << "\nCargando archivo...\n";
    
    try {
        CSVLoader loader("error.log", ",");
        std::vector<Product*> productos = loader.loadProducts(rutaCSV, true);
        
        LoadStats stats = loader.getLoadStats();
        
        std::cout << "\n=== ESTADISTICAS DE CARGA ===\n";
        std::cout << "Total de lineas: " << stats.totalLineas << "\n";
        std::cout << "Productos cargados: " << stats.productosExitosos << "\n";
        std::cout << "Errores totales: " << (stats.erroresLinea + stats.erroresDuplicados + 
                                             stats.erroresFecha + stats.erroresNumeros + 
                                             stats.erroresOtros) << "\n";
        
        // Insertar todos los productos en todas las estructuras con cronometro
        std::cout << "\n=== INSERCION EN ESTRUCTURAS ===\n";
        
        // Insertar en Arbol AVL
        if (arbolAVL) {
            auto inicio = std::chrono::high_resolution_clock::now();
            int errorAVL = 0;
            for (auto producto : productos) {
                try {
                    arbolAVL->insertar(producto);
                } catch (const std::exception& e) {
                    errorAVL++;
                }
            }
            auto fin = std::chrono::high_resolution_clock::now();
            auto duracion = std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio);
            std::cout << "Arbol AVL - " << duracion.count() << " milisegundos";
            if (errorAVL > 0) {
                std::cout << " (" << errorAVL << " errores)";
            }
            std::cout << "\n";
        }
        
        // Insertar en Arbol B
        if (arbolB) {
            auto inicio = std::chrono::high_resolution_clock::now();
            int errorB = 0;
            for (auto producto : productos) {
                try {
                    arbolB->insertar(producto);
                } catch (const std::exception& e) {
                    errorB++;
                }
            }
            auto fin = std::chrono::high_resolution_clock::now();
            auto duracion = std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio);
            std::cout << "Arbol B - " << duracion.count() << " milisegundos";
            if (errorB > 0) {
                std::cout << " (" << errorB << " errores)";
            }
            std::cout << "\n";
        }
        
        // Insertar en Lista Enlazada
        if (lista) {
            auto inicio = std::chrono::high_resolution_clock::now();
            int errorLista = 0;
            for (auto producto : productos) {
                try {
                    lista->insertar(producto);
                } catch (const std::exception& e) {
                    errorLista++;
                }
            }
            auto fin = std::chrono::high_resolution_clock::now();
            auto duracion = std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio);
            std::cout << "Lista Enlazada - " << duracion.count() << " milisegundos";
            if (errorLista > 0) {
                std::cout << " (" << errorLista << " errores)";
            }
            std::cout << "\n";
        }
        
        // Insertar en Lista Ordenada
        if (listaOrdenada) {
            auto inicio = std::chrono::high_resolution_clock::now();
            int errorListaOrdenada = 0;
            for (auto producto : productos) {
                try {
                    listaOrdenada->insertar(producto);
                } catch (const std::exception& e) {
                    errorListaOrdenada++;
                }
            }
            auto fin = std::chrono::high_resolution_clock::now();
            auto duracion = std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio);
            std::cout << "Lista Ordenada - " << duracion.count() << " milisegundos";
            if (errorListaOrdenada > 0) {
                std::cout << " (" << errorListaOrdenada << " errores)";
            }
            std::cout << "\n";
        }

        // Insertar en Arbol B Plus
        if (arbolBPlus) {
            auto inicio = std::chrono::high_resolution_clock::now();
            int errorBPlus = 0;
            for (auto producto : productos) {
                try {
                    arbolBPlus->insertar(producto);
                } catch (const std::exception& e) {
                    errorBPlus++;
                }
            }
            auto fin = std::chrono::high_resolution_clock::now();
            auto duracion = std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio);
            std::cout << "Arbol B Plus - " << duracion.count() << " milisegundos";
            if (errorBPlus > 0) {
                std::cout << " (" << errorBPlus << " errores)";
            }
            std::cout << "\n";
        }
        
        std::cout << "\nTotal de productos: " << productos.size() << "\n";
        std::cout << "\nRevisa error.log para detalles de errores.\n";
        
        showSuccess("Carga desde CSV completada.");
        
    } catch (const std::exception& e) {
        showError(std::string("Error al cargar CSV: ") + e.what());
    }
    
    pause();
}

void Frontend::deleteProduct() {
    clearScreen();
    showSeparator();
    std::cout << "ELIMINAR PRODUCTO\n";
    showSeparator();
    
    std::cout << "\nPor cual clave deseas eliminar?\n";
    std::cout << "1. Por Nombre\n";
    std::cout << "2. Por Fecha de Vencimiento\n";
    std::cout << "3. Cancelar\n\n";
    
    int opcion = readOption(1, 3);
    Product* productoAEliminar = nullptr;
    std::string nombre = "";
    std::string fecha = "";
    
    try {
        switch (opcion) {
            case 1: {
                // Buscar por nombre en ArbolAVL
                nombre = StringHelper::toLowerCase(readString("Nombre del producto a eliminar: "));
                if (arbolAVL) {
                    productoAEliminar = arbolAVL->obtenerProducto(nombre);
                    if (!productoAEliminar) {
                        showError("Producto no encontrado.");
                        pause();
                        return;
                    }
                }
                break;
            }
            case 2: {
                // Buscar por fecha en ArbolB
                fecha = readDate("Fecha de vencimiento a eliminar (YYYY-MM-DD): ");
                if (arbolB) {
                    productoAEliminar = arbolB->buscar(fecha);
                    if (!productoAEliminar) {
                        showError("Producto no encontrado.");
                        pause();
                        return;
                    }
                }
                break;
            }
            case 3:
                pause();
                return;
        }
        
        // Ya tenemos el producto a eliminar - guardar datos para rollback
        if (!productoAEliminar) {
            showError("Error al obtener el producto.");
            pause();
            return;
        }
        
        std::string nombreProducto = StringHelper::toLowerCase(productoAEliminar->getName());
        std::string fechaProducto = productoAEliminar->getExpiry_date();
        
        std::cout << "\nEliminando producto de todas las estructuras...\n";
        
        // Intentar eliminar de todas las estructuras con tracking de rollback
        bool eliminadoAVL = false;
        bool eliminadoB = false;
        bool eliminadoBPlus = false;
        bool eliminadoLista = false;
        bool eliminadoListaOrdenada = false;
        
        try {
            // Eliminar del Arbol AVL
            if (arbolAVL) {
                try {
                    arbolAVL->eliminar(productoAEliminar);
                    eliminadoAVL = true;
                    std::cout << "[OK] Eliminado de Arbol AVL\n";
                } catch (const std::exception& e) {
                    std::cout << "[FALLO] Error en AVL: " << e.what() << "\n";
                    throw;
                }
            }
            
            // Eliminar del Arbol B
            if (arbolB) {
                try {
                    arbolB->eliminar(fechaProducto);
                    eliminadoB = true;
                    std::cout << "[OK] Eliminado de Arbol B\n";
                } catch (const std::exception& e) {
                    std::cout << "[FALLO] Error en B: " << e.what() << "\n";
                    throw;
                }
            }

            // Eliminar del Arbol B Plus
            if (arbolBPlus) {
                try {
                    arbolBPlus->eliminar(productoAEliminar->getCategory());
                    eliminadoBPlus = true;
                    std::cout << "[OK] Eliminado de Arbol B Plus\n";
                } catch (const std::exception& e) {
                    std::cout << "[FALLO] Error en B Plus: " << e.what() << "\n";
                    throw;
                }
            }
            
            // Eliminar de Lista Enlazada
            if (lista) {
                try {
                    lista->eliminar(nombreProducto);
                    eliminadoLista = true;
                    std::cout << "[OK] Eliminado de Lista Enlazada\n";
                } catch (const std::exception& e) {
                    std::cout << "[FALLO] Error en Lista Enlazada: " << e.what() << "\n";
                    throw;
                }
            }
            
            // Eliminar de Lista Ordenada
            if (listaOrdenada) {
                try {
                    listaOrdenada->eliminar(nombreProducto);
                    eliminadoListaOrdenada = true;
                    std::cout << "[OK] Eliminado de Lista Ordenada\n";
                } catch (const std::exception& e) {
                    std::cout << "[FALLO] Error en Lista Ordenada: " << e.what() << "\n";
                    throw;
                }
            }
            
            showSuccess("Producto eliminado de todas las estructuras.");
            
        } catch (const std::exception& e) {
            showError("Error en eliminacion. Realizando rollback de estructuras eliminadas...");
            
            // ROLLBACK - reinsertar SOLO en las estructuras que fueron eliminadas exitosamente
            try {
                std::cout << "\nHaciendo rollback de estructuras afectadas...\n";
                
                if (eliminadoAVL) {
                    try {
                        arbolAVL->insertar(productoAEliminar);
                        std::cout << "Restaurado en Arbol AVL\n";
                    } catch (const std::exception& ex) {
                        std::cerr << "Error al restaurar en AVL: " << ex.what() << "\n";
                    }
                }
                if (eliminadoB) {
                    try {
                        arbolB->insertar(productoAEliminar);
                        std::cout << "Restaurado en Arbol B\n";
                    } catch (const std::exception& ex) {
                        std::cerr << "Error al restaurar en B: " << ex.what() << "\n";
                    }
                }
                if (eliminadoBPlus) {
                    try {
                        arbolBPlus->insertar(productoAEliminar);
                        std::cout << "Restaurado en Arbol B Plus\n";
                    } catch (const std::exception& ex) {
                        std::cerr << "Error al restaurar en B Plus: " << ex.what() << "\n";
                    }
                }
                if (eliminadoLista) {
                    try {
                        lista->insertar(productoAEliminar);
                        std::cout << "Restaurado en Lista Enlazada\n";
                    } catch (const std::exception& ex) {
                        std::cerr << "Error al restaurar en Lista Enlazada: " << ex.what() << "\n";
                    }
                }
                if (eliminadoListaOrdenada) {
                    try {
                        listaOrdenada->insertar(productoAEliminar);
                        std::cout << "Restaurado en Lista Ordenada\n";
                    } catch (const std::exception& ex) {
                        std::cerr << "Error al restaurar en Lista Ordenada: " << ex.what() << "\n";
                    }
                }
                
                showSuccess("Rollback completado. Producto restaurado en estructuras eliminadas.");
                
            } catch (const std::exception& rollbackEx) {
                showError(std::string("CRITICO: Fallo en rollback: ") + rollbackEx.what());
            }
        }
        
    } catch (const std::exception& e) {
        showError(std::string("Error: ") + e.what());
    }
    
    pause();
}

void Frontend::searchProduct() {
    clearScreen();
    showSeparator();
    std::cout << "BUSCAR PRODUCTO\n";
    showSeparator();
    
    std::cout << "\nEn cual estructura deseas buscar?\n";
    std::cout << "1. Arbol AVL (por Nombre)\n";
    std::cout << "2. Arbol B (por Fecha de Vencimiento)\n";
    std::cout << "3. Arbol B Plus (por Categoria)\n";
    std::cout << "4. Lista Enlazada (por Nombre)\n";
    std::cout << "5. Lista Ordenada (por Nombre)\n";
    std::cout << "6. Cancelar\n\n";
    
    int opcion = readOption(1, 6);
    
    switch (opcion) {
        case 1: {
            std::string nombre = StringHelper::toLowerCase(readString("Nombre a buscar: "));
            if (arbolAVL) {
                Product* resultado = arbolAVL->obtenerProducto(nombre);
                if (resultado) {
                    std::cout << "\n=== PRODUCTO ENCONTRADO ===\n";
                    std::cout << "Nombre: " << resultado->getName() << "\n";
                    std::cout << "Codigo: " << resultado->getBarcode() << "\n";
                    std::cout << "Categoria: " << resultado->getCategory() << "\n";
                    std::cout << "Marca: " << resultado->getBrand() << "\n";
                    std::cout << "Precio: $" << std::fixed << std::setprecision(2) 
                              << resultado->getPrice() << "\n";
                    std::cout << "Stock: " << resultado->getStock() << "\n";
                    std::cout << "Vencimiento: " << resultado->getExpiry_date() << "\n";
                    showSuccess("Busqueda completada.");
                } else {
                    showError("Producto no encontrado en Arbol AVL.");
                }
            }
            break;
        }
        case 2: {
            std::string fecha = readDate("Fecha de vencimiento a buscar (YYYY-MM-DD): ");
            if (arbolB) {
                Product* resultado = arbolB->buscar(fecha);
                if (resultado) {
                    std::cout << "\n=== PRODUCTO ENCONTRADO ===\n";
                    std::cout << "Nombre: " << resultado->getName() << "\n";
                    std::cout << "Codigo: " << resultado->getBarcode() << "\n";
                    std::cout << "Categoria: " << resultado->getCategory() << "\n";
                    std::cout << "Marca: " << resultado->getBrand() << "\n";
                    std::cout << "Precio: $" << std::fixed << std::setprecision(2) 
                              << resultado->getPrice() << "\n";
                    std::cout << "Stock: " << resultado->getStock() << "\n";
                    std::cout << "Vencimiento: " << resultado->getExpiry_date() << "\n";
                    showSuccess("Busqueda completada.");
                } else {
                    showError("Producto no encontrado en Arbol B.");
                }
            }
            break;
        }
        case 3: {
            std::string categoria = readString("Categoria a buscar: ");
            if (arbolBPlus) {
                // Listar por categoria especifica
                std::cout << "\nProductos en categoria: " << categoria << "\n";
                arbolBPlus->listarCategoria(categoria);
            }
            break;
        }
        case 4: {
            std::string nombre = StringHelper::toLowerCase(readString("Nombre a buscar: "));
            if (lista) {
                NodoList* nodo = lista->buscarNodo(nombre);
                if (nodo) {
                    Product* resultado = nodo->getDato();
                    std::cout << "\n=== PRODUCTO ENCONTRADO ===\n";
                    std::cout << "Nombre: " << resultado->getName() << "\n";
                    std::cout << "Codigo: " << resultado->getBarcode() << "\n";
                    std::cout << "Categoria: " << resultado->getCategory() << "\n";
                    std::cout << "Marca: " << resultado->getBrand() << "\n";
                    std::cout << "Precio: $" << std::fixed << std::setprecision(2) 
                              << resultado->getPrice() << "\n";
                    std::cout << "Stock: " << resultado->getStock() << "\n";
                    std::cout << "Vencimiento: " << resultado->getExpiry_date() << "\n";
                    showSuccess("Busqueda completada.");
                } else {
                    showError("Producto no encontrado en Lista.");
                }
            }
            break;
        }
        case 5: {
            std::string nombre = StringHelper::toLowerCase(readString("Nombre a buscar: "));
            if (listaOrdenada) {
                NodoList* nodo = listaOrdenada->buscarNodo(nombre);
                if (nodo) {
                    Product* resultado = nodo->getDato();
                    std::cout << "\n=== PRODUCTO ENCONTRADO ===\n";
                    std::cout << "Nombre: " << resultado->getName() << "\n";
                    std::cout << "Codigo: " << resultado->getBarcode() << "\n";
                    std::cout << "Categoria: " << resultado->getCategory() << "\n";
                    std::cout << "Marca: " << resultado->getBrand() << "\n";
                    std::cout << "Precio: $" << std::fixed << std::setprecision(2) 
                              << resultado->getPrice() << "\n";
                    std::cout << "Stock: " << resultado->getStock() << "\n";
                    std::cout << "Vencimiento: " << resultado->getExpiry_date() << "\n";
                    showSuccess("Busqueda completada.");
                } else {
                    showError("Producto no encontrado en Lista Ordenada.");
                }
            }
            break;
        }
        case 6:
            break;
    }
    
    pause();
}

void Frontend::exitApp() {
    clearScreen();
    std::cout << "\nGracias por usar el sistema de gestion!\n\n";
    isRunning = false;
}

std::string Frontend::readString(const std::string& prompt) {
    std::string input;
    
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        
        // Trim
        size_t start = input.find_first_not_of(" \t\n\r");
        if (start != std::string::npos) {
            input = input.substr(start);
        }
        
        if (!input.empty()) {
            return input;
        }
        
        std::cout << "El campo no puede estar vacio. Intenta de nuevo.\n";
    }
}

double Frontend::readDouble(const std::string& prompt) {
    std::string input;
    
    while (true) {
        input = readString(prompt);
        
        try {
            size_t pos;
            double value = std::stod(input, &pos);
            if (pos == input.length()) {
                return value;
            }
        } catch (...) {
        }
        
        std::cout << "Numero invalido. Ingresa un numero decimal: ";
    }
}

int Frontend::readInteger(const std::string& prompt) {
    std::string input;
    
    while (true) {
        input = readString(prompt);
        
        try {
            size_t pos;
            int value = std::stoi(input, &pos);
            if (pos == input.length()) {
                return value;
            }
        } catch (...) {
        }
        
        std::cout << "Numero invalido. Ingresa un numero entero: ";
    }
}

std::string Frontend::readDate(const std::string& prompt) {
    std::string input;
    
    while (true) {
        input = readString(prompt);
        
        if (isValidDate(input)) {
            return input;
        }
        
        std::cout << "Fecha invalida. Usa formato YYYY-MM-DD (ej: 2026-03-15): ";
    }
}

bool Frontend::isValidDate(const std::string& date) {
    // Verificar formato YYYY-MM-DD
    std::regex dateRegex("^\\d{4}-\\d{2}-\\d{2}$");
    
    if (!std::regex_match(date, dateRegex)) {
        return false;
    }
    
    try {
        int month = std::stoi(date.substr(5, 2));
        int day = std::stoi(date.substr(8, 2));
        
        if (month < 1 || month > 12) return false;
        if (day < 1 || day > 31) return false;
        
        return true;
    } catch (...) {
        return false;
    }
}

int Frontend::readOption(int minOp, int maxOp) {
    std::string input;
    
    while (true) {
        std::cout << "Opcion (" << minOp << "-" << maxOp << "): ";
        std::getline(std::cin, input);
        
        try {
            int option = std::stoi(input);
            if (option >= minOp && option <= maxOp) {
                return option;
            }
        } catch (...) {
        }
        
        std::cout << "Opcion invalida. Intenta de nuevo.\n";
    }
}

void Frontend::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void Frontend::pause() {
    std::cout << "\nPresiona ENTER para continuar...";
    std::string dummy;
    std::getline(std::cin, dummy);
}

void Frontend::showSuccess(const std::string& msg) {
    std::cout << "\n[Success] " << msg << "\n";
}

void Frontend::showError(const std::string& msg) {
    std::cout << "\n[Fallo] ERROR: " << msg << "\n";
}

void Frontend::showSeparator() {
    std::cout << std::string(50, '=') << "\n";
}

// ========== EXPORTAR ARBOL AVL A .DOT ==========

void Frontend::exportAVLToDot() {
    clearScreen();
    showSeparator();
    std::cout << "EXPORTAR ARBOL AVL A FORMATO .DOT\n";
    showSeparator();
    
    std::string outputDir = readString("Directorio de salida (ej: output) [Enter=current]: ");
    if (outputDir.empty()) {
        outputDir = ".";
    }
    
    // Crear directorio si no existe
    std::string mkdirCmd;
    #ifdef _WIN32
        mkdirCmd = "if not exist \"" + outputDir + "\" mkdir \"" + outputDir + "\"";
    #else
        mkdirCmd = "mkdir -p \"" + outputDir + "\"";
    #endif
    system(mkdirCmd.c_str());
    
    std::cout << "\nGenerando archivo DOT...\n";
    showSeparator();
    
    try {
        // Generar DOT para Arbol AVL
        std::string avlPath = outputDir + "/arbol_avl.dot";
        std::cout << "Generando Arbol AVL... ";
        if (arbolAVL && arbolAVL->generarDot(avlPath)) {
            std::cout << "[OK]\n";
            showSeparator();
            std::cout << "\nArchivo generado en: " << avlPath << "\n";
            std::cout << "\nPara visualizar el arbol:\n";
            std::cout << "  1. Instala Graphviz desde: https://graphviz.org/download/\n";
            std::cout << "  2. Convierte a imagen (Windows CMD):\n";
            std::cout << "     dot -Tpng " << avlPath << " -o " << outputDir << "\\arbol_avl.png\n";
            std::cout << "  3. O visualiza en linea en: https://dreampuf.github.io/GraphvizOnline/\n";
        } else {
            showError("Error al generar DOT del Arbol AVL");
        }
        
    } catch (const exception& e) {
        showError("Excepcion durante exportacion: " + string(e.what()));
    } catch (...) {
        showError("Error desconocido durante exportacion");
    }
    
    pause();
}

