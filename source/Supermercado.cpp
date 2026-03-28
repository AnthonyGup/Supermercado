#include <iostream>
#include <limits>
#include <sstream>
#include <string>

#include "estructuras/listas/normal/ListaEnlazada.h"
#include "estructuras/listas/ordenada/ListaOrdenada.h"
#include "excepciones/ProductoNoEncontradoException.h"

using namespace std;

namespace
{
    Product *crearProducto(const string &nombre, const string &codigo, const string &categoria,
                           const string &vencimiento, const string &marca, double precio, int stock)
    {
        Product *producto = new Product();
        producto->setName(nombre);
        producto->setBarcode(codigo);
        producto->setCategory(categoria);
        producto->setExpiry_date(vencimiento);
        producto->setBrand(marca);
        producto->setPrice(precio);
        producto->setStock(stock);
        return producto;
    }

    bool pedirDatosProducto(string &nombre, string &codigo, string &categoria,
                            string &vencimiento, string &marca, double &precio, int &stock)
    {
        cout << "Ingresa los datos en una linea con este formato:" << endl;
        cout << "nombre|codigo|categoria|vencimiento|marca|precio|stock" << endl;
        cout << "> ";

        string linea;
        getline(cin, linea);

        istringstream stream(linea);
        string precioTexto;
        string stockTexto;

        if (!getline(stream, nombre, '|') ||
            !getline(stream, codigo, '|') ||
            !getline(stream, categoria, '|') ||
            !getline(stream, vencimiento, '|') ||
            !getline(stream, marca, '|') ||
            !getline(stream, precioTexto, '|') ||
            !getline(stream, stockTexto, '|'))
        {
            cout << "Formato invalido. Intenta de nuevo." << endl;
            return false;
        }

        try
        {
            precio = stod(precioTexto);
            stock = stoi(stockTexto);
        }
        catch (const exception &)
        {
            cout << "Precio o stock invalidos." << endl;
            return false;
        }

        return true;
    }

    int pedirOpcion()
    {
        int opcion = -1;
        cout << "\n===== MENU SUPERMERCADO =====" << endl;
        cout << "1. Insertar producto en ambas listas" << endl;
        cout << "2. Eliminar producto de ambas listas" << endl;
        cout << "3. Mostrar ambas listas" << endl;
        cout << "0. Salir" << endl;
        cout << "Selecciona una opcion: ";

        if (!(cin >> opcion))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return -1;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return opcion;
    }
}

int main()
{
    ListaEnlazada listaEnlazada;
    ListaOrdenada listaOrdenada;

    while (true)
    {
        int opcion = pedirOpcion();

        if (opcion == 0)
        {
            cout << "Saliendo..." << endl;
            break;
        }

        switch (opcion)
        {
        case 1:
        {
            string nombre;
            string codigo;
            string categoria;
            string vencimiento;
            string marca;
            double precio = 0.0;
            int stock = 0;

            if (!pedirDatosProducto(nombre, codigo, categoria, vencimiento, marca, precio, stock))
            {
                break;
            }

            listaEnlazada.insertar(crearProducto(nombre, codigo, categoria, vencimiento, marca, precio, stock));
            listaOrdenada.insertar(crearProducto(nombre, codigo, categoria, vencimiento, marca, precio, stock));
            cout << "Producto insertado en ambas listas." << endl;
            break;
        }
        case 2:
        {
            string nombre;
            cout << "Nombre del producto a eliminar en ambas listas: ";
            getline(cin, nombre);

            try
            {
                listaEnlazada.eliminar(nombre);
                listaOrdenada.eliminar(nombre);
                cout << "Producto eliminado en ambas listas." << endl;
            }
            catch (const ProductoNoEncontradoException &ex)
            {
                cout << ex.what() << endl;
            }
            break;
        }
        case 3:
        {
            cout << "\n--- LISTA ENLAZADA ---" << endl;
            listaEnlazada.mostrar();
            cout << "\n--- LISTA ORDENADA ---" << endl;
            listaOrdenada.mostrar();
            break;
        }
        default:
            cout << "Opcion invalida." << endl;
            break;
        }
    }

    return 0;
}