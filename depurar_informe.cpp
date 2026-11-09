#include "aplicacion.h"
#include <iostream>

int main() {
    const Conjunto conjunto{{
        Grupo{1, {
            Registro{6, Recuento{2, 1}},
            Registro{0, Recuento{3, 0}}
        }}
    }};
    if (!mostrar_informe(conjunto, std::cout)) { return 1; }
    return 0;
}
