#include "CSVLoader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <regex>
#include <ctime>

CSVLoader::CSVLoader(string logFilePath, string delimiter) 
    : delimiter(delimiter), logFile(logFilePath) {
    clearLogFile();
}

CSVLoader::~CSVLoader() = default;

void CSVLoader::clearLogFile() {
    ofstream file(logFile, ios::trunc);
    if (file.is_open()) {
        file << "=== Log de Errores - CSVLoader ===" << endl;
        // Obtener hora actual
        time_t now = time(nullptr);
        struct tm* timeinfo = localtime(&now);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
        file << "Inicio de carga: " << buffer << endl;
        file << "====================================\n\n";
        file.close();
    }
}

void CSVLoader::logError(const string& message, int lineNumber) {
    ofstream file(logFile, ios::app);
    if (file.is_open()) {
        if (lineNumber > 0) {
            file << "[Línea " << lineNumber << "] " << message << endl;
        } else {
            file << message << endl;
        }
        file.close();
    }
}

vector<string> CSVLoader::splitLineWithQuotes(const string& line, char delimiter) {
    vector<string> tokens;
    string token;
    bool insideQuotes = false;
    
    for (size_t i = 0; i < line.length(); ++i) {
        char c = line[i];
        
        if (c == '"') {
            insideQuotes = !insideQuotes;
            token += c;
        } else if (c == delimiter && !insideQuotes) {
            tokens.push_back(token);
            token.clear();
        } else {
            token += c;
        }
    }
    
    tokens.push_back(token);
    return tokens;
}

string CSVLoader::trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (string::npos == first) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

string CSVLoader::removeQuotes(const string& str) {
    string trimmed = trim(str);
    if (trimmed.length() >= 2 && trimmed.front() == '"' && trimmed.back() == '"') {
        return trimmed.substr(1, trimmed.length() - 2);
    }
    return trimmed;
}

bool CSVLoader::isValidISODate(const string& date) {
    // Formato esperado: YYYY-MM-DD
    regex isoDateRegex("^\\d{4}-\\d{2}-\\d{2}$");
    
    if (!regex_match(date, isoDateRegex)) {
        return false;
    }
    
    try {
        int month = stoi(date.substr(5, 2));
        int day = stoi(date.substr(8, 2));
        
        if (month < 1 || month > 12) return false;
        if (day < 1 || day > 31) return false;
        
        return true;
    } catch (...) {
        return false;
    }
}

bool CSVLoader::isValidDouble(const string& str) {
    try {
        size_t pos;
        stod(str, &pos);
        return pos == str.length();
    } catch (...) {
        return false;
    }
}

bool CSVLoader::isValidInteger(const string& str) {
    try {
        size_t pos;
        stoi(str, &pos);
        return pos == str.length();
    } catch (...) {
        return false;
    }
}

bool CSVLoader::isUniqueBarcode(const string& barcode) {
    if (loadedBarcodes.count(barcode) > 0) {
        return false;
    }
    loadedBarcodes.insert(barcode);
    return true;
}

vector<Product*> CSVLoader::loadProducts(const string& filename, bool hasHeader) {
    vector<Product*> products;
    
    // Inicializar estadísticas
    stats = LoadStats();
    
    ifstream file(filename);
    
    // Validar existencia y legibilidad del archivo
    if (!file.is_open()) {
        string errorMsg = "ERROR CRÍTICO: No se pudo abrir el archivo: " + filename;
        logError(errorMsg, -1);
        throw runtime_error(errorMsg);
    }
    
    if (!file.good()) {
        string errorMsg = "ERROR CRÍTICO: El archivo no es legible: " + filename;
        logError(errorMsg, -1);
        throw runtime_error(errorMsg);
    }
    
    string line;
    int lineNumber = 0;
    
    // Saltar la línea de encabezados si es necesario
    if (hasHeader && getline(file, line)) {
        lineNumber++;
    }
    
    // Leer cada línea del archivo
    while (getline(file, line)) {
        lineNumber++;
        
        // Ignorar líneas vacías
        if (trim(line).empty()) {
            continue;
        }
        
        stats.totalLineas++;
        
        vector<string> fields = splitLineWithQuotes(line, delimiter[0]);
        
        // Validar número de campos
        if (fields.size() < 7) {
            string errorMsg = "Número insuficiente de campos. Se esperaban 7, se encontraron " + 
                            to_string(fields.size());
            logError(errorMsg, lineNumber);
            stats.erroresLinea++;
            continue;
        }
        
        try {
            // Limpiar los campos (remover comillas y espacios)
            string nombre = removeQuotes(fields[0]);
            string codigoBarra = removeQuotes(fields[1]);
            string categoria = removeQuotes(fields[2]);
            string fechaCaducidad = removeQuotes(fields[3]);
            string marca = removeQuotes(fields[4]);
            string precioStr = removeQuotes(fields[5]);
            string stockStr = removeQuotes(fields[6]);
            
            // Validar que los campos no estén vacíos
            if (nombre.empty()) {
                logError("Campo 'Nombre' vacío", lineNumber);
                stats.erroresOtros++;
                continue;
            }
            
            if (codigoBarra.empty()) {
                logError("Campo 'CodigoBarra' vacío", lineNumber);
                stats.erroresOtros++;
                continue;
            }
            
            // Validar fecha ISO
            if (!isValidISODate(fechaCaducidad)) {
                string errorMsg = "Fecha de caducidad inválida: '" + fechaCaducidad + 
                                "'. Formato esperado: YYYY-MM-DD";
                logError(errorMsg, lineNumber);
                stats.erroresFecha++;
                continue;
            }
            
            // Validar precio
            if (!isValidDouble(precioStr)) {
                string errorMsg = "Precio inválido: '" + precioStr + "' no es un número válido";
                logError(errorMsg, lineNumber);
                stats.erroresNumeros++;
                continue;
            }
            
            // Validar stock
            if (!isValidInteger(stockStr)) {
                string errorMsg = "Stock inválido: '" + stockStr + "' no es un número entero válido";
                logError(errorMsg, lineNumber);
                stats.erroresNumeros++;
                continue;
            }
            
            // Validar unicidad de código de barra
            if (!isUniqueBarcode(codigoBarra)) {
                string errorMsg = "Código de barra duplicado: '" + codigoBarra + "'. Producto omitido.";
                logError(errorMsg, lineNumber);
                stats.erroresDuplicados++;
                continue;
            }
            
            // Crear el producto de forma dinámica
            Product* product = new Product();
            product->setName(nombre);
            product->setBarcode(codigoBarra);
            product->setCategory(categoria);
            product->setExpiry_date(fechaCaducidad);
            product->setBrand(marca);
            product->setPrice(stod(precioStr));
            product->setStock(stoi(stockStr));
            
            products.push_back(product);
            stats.productosExitosos++;
            
        } catch (const exception& e) {
            string errorMsg = string("Excepción durante procesamiento: ") + e.what();
            logError(errorMsg, lineNumber);
            stats.erroresOtros++;
            continue;  // Continuar con la siguiente línea
        }
    }
    
    file.close();
    
    // Loggear resumen
    logError("", -1);
    logError("=== RESUMEN DE CARGA ===", -1);
    logError("Total de líneas procesadas: " + to_string(stats.totalLineas), -1);
    logError("Productos cargados exitosamente: " + to_string(stats.productosExitosos), -1);
    logError("Errores de formato de línea: " + to_string(stats.erroresLinea), -1);
    logError("Códigos de barra duplicados: " + to_string(stats.erroresDuplicados), -1);
    logError("Errores de fecha: " + to_string(stats.erroresFecha), -1);
    logError("Errores de números: " + to_string(stats.erroresNumeros), -1);
    logError("Otros errores: " + to_string(stats.erroresOtros), -1);
    logError("Total de errores: " + to_string(
        stats.erroresLinea + stats.erroresDuplicados + stats.erroresFecha + 
        stats.erroresNumeros + stats.erroresOtros), -1);
    
    return products;
}

LoadStats CSVLoader::getLoadStats() const {
    return stats;
}
