#ifndef DOT_GENERATOR_H
#define DOT_GENERATOR_H

#include <string>
#include <sstream>
#include <fstream>
#include <memory>
#include <vector>
#include <iostream>

using namespace std;

/**
 * DotGenerator - Utilidad para generar archivos .DOT (Graphviz)
 * Permite visualizar árboles binarios y n-arios
 * 
 * Uso:
 *   DotGenerator gen("arbol_avl", "Árbol AVL");
 *   gen.addNode("1", "NodoAVL: 25 (FE:0)");
 *   gen.addEdge("1", "2", "Izqda");
 *   gen.saveToDot("output/arbol.dot");
 */
class DotGenerator {
private:
    string treeName;
    string treeTitle;
    stringstream dotContent;
    int nodeCount;

public:
    /**
     * Constructor
     * @param name Nombre único del árbol (sin espacios)
     * @param title Título para mostrar en el gráfico
     */
    DotGenerator(const string& name, const string& title) 
        : treeName(name), treeTitle(title), nodeCount(0) {
        initializeDot();
    }

    /**
     * Agrega un nodo al gráfico DOT
     * @param nodeId Identificador único del nodo
     * @param label Etiqueta a mostrar en el nodo
     * @param shape Forma del nodo: "box", "circle", "ellipse", "diamond", etc.
     * @param color Color del nodo: "lightblue", "lightgreen", "red", etc.
     */
    void addNode(const string& nodeId, const string& label, 
                 const string& shape = "box", const string& color = "lightblue") {
        dotContent << "    " << nodeId << " [label=\"" << escapeLabel(label) 
                   << "\", shape=" << shape << ", style=filled, fillcolor=" 
                   << color << "];\n";
        nodeCount++;
    }

    /**
     * Agrega una arista (edge) entre dos nodos
     * @param fromNode Nodo origen
     * @param toNode Nodo destino
     * @param label Etiqueta opcional de la arista
     */
    void addEdge(const string& fromNode, const string& toNode, 
                 const string& label = "") {
        dotContent << "    " << fromNode << " -> " << toNode;
        if (!label.empty()) {
            dotContent << " [label=\"" << escapeLabel(label) << "\"]";
        }
        dotContent << ";\n";
    }

    /**
     * Agrega múltiples aristas desde un nodo a varios nodos hijos
     * @param parentNode Nodo padre
     * @param childNodes Vector de IDs de nodos hijos
     */
    void addChildren(const string& parentNode, const vector<string>& childNodes) {
        for (const string& child : childNodes) {
            addEdge(parentNode, child);
        }
    }

    /**
     * Guarda el gráfico a un archivo .DOT
     * @param filepath Ruta completa del archivo (ej: "output/arbol.dot")
     * @return true si se guardó correctamente, false si hubo error
     */
    bool saveToDot(const string& filepath) {
        try {
            ofstream outFile(filepath);
            if (!outFile.is_open()) {
                cerr << "Error: No se pudo abrir el archivo: " << filepath << endl;
                return false;
            }

            finalizeDot();
            outFile << dotContent.str();
            outFile.close();

            cout << "✓ Archivo .DOT generado exitosamente: " << filepath << endl;
            cout << "  Nodos: " << nodeCount << endl;
            cout << "  Para visualizar, use Graphviz:" << endl;
            cout << "    dot -Tpng " << filepath << " -o " << filepath << ".png" << endl;
            cout << "    dot -Tsvg " << filepath << " -o " << filepath << ".svg" << endl;
            cout << "  O visualice en línea: https://dreampuf.github.io/GraphvizOnline/" << endl;

            return true;
        } catch (const exception& e) {
            cerr << "Error al guardar .DOT: " << e.what() << endl;
            return false;
        }
    }

    /**
     * Retorna el contenido DOT como string (sin guardar a archivo)
     */
    string toDotString() {
        finalizeDot();
        return dotContent.str();
    }

    /**
     * Obtiene el número de nodos agregados
     */
    int getNodeCount() const {
        return nodeCount;
    }

private:
    /**
     * Inicializa la estructura básica del gráfico DOT
     */
    void initializeDot() {
        dotContent << "digraph " << treeName << " {\n";
        dotContent << "    rankdir=TB;\n"; // De arriba hacia abajo
        dotContent << "    bgcolor=white;\n";
        dotContent << "    node [fontname=\"Arial\", fontsize=10];\n";
        dotContent << "    edge [fontname=\"Arial\", fontsize=9];\n";
        dotContent << "    label=\"" << escapeLabel(treeTitle) << "\";\n";
        dotContent << "    labelloc=top;\n\n";
    }

    /**
     * Finaliza la estructura del gráfico DOT
     */
    void finalizeDot() {
        dotContent << "}\n";
    }

    /**
     * Escapa caracteres especiales en etiquetas
     */
    string escapeLabel(const string& label) {
        string result;
        for (char c : label) {
            if (c == '"') {
                result += "\\\"";
            } else if (c == '\n') {
                result += "\\n";
            } else if (c == '\\') {
                result += "\\\\";
            } else {
                result += c;
            }
        }
        return result;
    }
};

#endif
