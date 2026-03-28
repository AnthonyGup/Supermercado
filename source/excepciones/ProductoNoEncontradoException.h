#ifndef PRODUCTO_NO_ENCONTRADO_EXCEPTION_H
#define PRODUCTO_NO_ENCONTRADO_EXCEPTION_H

#include <exception>
#include <string>

class ProductoNoEncontradoException : public std::exception {
    private:
        std::string mensaje;

    public:
        explicit ProductoNoEncontradoException(const std::string &codigoBarra);
        ProductoNoEncontradoException(const std::string &operacion, const std::string &detalle);
        const char *what() const noexcept override;
};

#endif
