#ifndef PRODUCTO_NO_ENCONTRADO_EXCEPTION_H
#define PRODUCTO_NO_ENCONTRADO_EXCEPTION_H

#include <stdexcept>
#include <string>

class ProductoNoEncontradoException : public std::runtime_error {
    public:
        explicit ProductoNoEncontradoException(const std::string &codigoBarra);
        ProductoNoEncontradoException(const std::string &operacion, const std::string &detalle);
};

#endif
