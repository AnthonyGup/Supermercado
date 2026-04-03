#ifndef CSVLOADER_H
#define CSVLOADER_H

#include <string>
#include <vector>
#include <set>
#include "../pojo/Product.h"

using namespace std;

// Estructura para almacenar estadísticas de carga
struct LoadStats {
    int totalLineas = 0;           // Total de líneas procesadas (sin contar encabezado)
    int productosExitosos = 0;     // Productos cargados correctamente
    int erroresLinea = 0;          // Errores por formato de línea
    int erroresDuplicados = 0;     // Códigos de barra duplicados
    int erroresFecha = 0;          // Errores en formato de fecha
    int erroresNumeros = 0;        // Errores en conversión de números
    int erroresOtros = 0;          // Otros errores
};

class CSVLoader {
    private:
        string delimiter;
        string logFile;
        set<string> loadedBarcodes;  // Para validar unicidad de códigos de barra
        LoadStats stats;             // Estadísticas de carga
        
        // Función auxiliar para dividir una línea respetando comillas
        vector<string> splitLineWithQuotes(const string& line, char delimiter);
        
        // Función auxiliar para remover espacios y comillas
        string trim(const string& str);
        
        // Función auxiliar para remover comillas
        string removeQuotes(const string& str);
        
        // Validar que la fecha esté en formato ISO (YYYY-MM-DD)
        bool isValidISODate(const string& date);
        
        // Validar que sea un número (double)
        bool isValidDouble(const string& str);
        
        // Validar que sea un número entero
        bool isValidInteger(const string& str);
        
        // Validar unicidad del código de barra
        bool isUniqueBarcode(const string& barcode);
        
        // Loggear error en archivo error.log
        void logError(const string& message, int lineNumber);
        
        // Limpiar archivo de log al inicio
        void clearLogFile();

    public:
        CSVLoader(string logFilePath = "error.log", string delimiter = ",");
        ~CSVLoader();
        
        /**
         * Carga productos desde un archivo CSV
         * Continúa la carga incluso si hay errores, los cuales son registrados en error.log
         * 
         * Formato esperado:
         * "Nombre","CodigoBarra","Categoria","FechaCaducidad","Marca","Precio","Stock"
         * @param filename Ruta del archivo CSV
         * @param hasHeader Si true, ignora la primera línea (encabezados)
         * @return Vector de punteros a productos cargados exitosamente
         * @throws runtime_error Si el archivo no existe o no es legible
         */
        vector<Product*> loadProducts(const string& filename, bool hasHeader = true);
        
        /**
         * Obtiene las estadísticas de la última carga
         * @return Estructura LoadStats con información detallada
         */
        LoadStats getLoadStats() const;
};

#endif
