#include "parser.hpp"
#include <vector>
#include <iostream>


int main() {
    // Definir la secuencia de tokens: IntType [ ] Identifier = CharLiteral ;
    std::vector<Token> tokens = {
        Token("IntType"),
        Token("Token_["),
        Token("Token_]"),
        Token("Token_ID"),
        Token("Token_="),
        Token("Token_Comilla"),
        Token("Token_ID"),
        Token("Token_Comilla"),
        Token("Token_;")
    };

    // Crear el objeto Parser con el vector de tokens
    Parser parser(tokens);

    // Ejecutar el análisis sintáctico
    if (parser.parse()) {
        std::cout << "Parsing exitoso." << std::endl;
    }
    else {
        std::cout << "Error de parsing." << std::endl;
    }

    return 0;
}
