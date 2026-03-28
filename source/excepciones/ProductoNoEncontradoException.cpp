#include "ProductoNoEncontradoException.h"

ProductoNoEncontradoException::ProductoNoEncontradoException(const std::string &codigoBarra)
    : mensaje("No se encontro un producto con el valor de busqueda: " + codigoBarra) {}

ProductoNoEncontradoException::ProductoNoEncontradoException(const std::string &operacion, const std::string &detalle)
    : mensaje("Error al " + operacion + ": " + detalle) {}

const char *ProductoNoEncontradoException::what() const noexcept {
    return mensaje.c_str();
}
