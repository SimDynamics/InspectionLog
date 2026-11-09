#include "aplicacion.h"
#include <iostream>

int main() {
    std::cout << "InspectionLog\nRuta de entrada:\n";
    std::string entrada;
    if (!std::getline(std::cin, entrada)) {
        std::cout << "Falta la ruta de entrada\n";
        return 1;
    }
    std::cout << "Ruta de salida:\n";
    std::string salida;
    if (!std::getline(std::cin, salida)) {
        std::cout << "Falta la ruta de salida\n";
        return 1;
    }
    Conjunto actual{};
    return procesar_archivos(entrada, salida, actual, std::cout);
}
