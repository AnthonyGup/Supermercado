#include "ProductoNoEncontradoException.h"

ProductoNoEncontradoException::ProductoNoEncontradoException(const std::string &codigoBarra)
    : std::runtime_error("No se encontro un producto con el valor de busqueda: " + codigoBarra) {}

ProductoNoEncontradoException::ProductoNoEncontradoException(const std::string &operacion, const std::string &detalle)
    : std::runtime_error("Error al " + operacion + ": " + detalle) {}
