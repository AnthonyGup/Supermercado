#include "StringHelper.h"

#include <algorithm>
#include <cctype>

std::string StringHelper::toLowerCase(const std::string &texto)
{
    std::string resultado = texto;
    std::transform(resultado.begin(), resultado.end(), resultado.begin(),
                   [](unsigned char c)
                   { return static_cast<char>(std::tolower(c)); });
    return resultado;
}
