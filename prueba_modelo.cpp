#include "inspection_log.h"
#include <iostream>
#include <sstream>

bool probar(const std::string& nombre, const std::string& texto,
            bool debe_aceptar, const Conjunto& esperado) {
    const Conjunto previo{{Grupo{1, {{9, {6, 2}}}}}};
    Conjunto actual = previo;
    std::istringstream entrada{texto};
    const bool aceptado = leer_muestra_completa(entrada, actual);
    bool correcto = aceptado == debe_aceptar;
    if (debe_aceptar) {
        if (!mismos_datos(actual, esperado)) { correcto = false; }
        std::string compuesto;
        if (!componer_conjunto(actual, compuesto)) { correcto = false; }
        std::istringstream vuelta{compuesto};
        Conjunto recuperado{};
        if (!leer_muestra_completa(vuelta, recuperado)
            || !mismos_datos(actual, recuperado)) { correcto = false; }
    } else if (!mismos_datos(actual, previo)) {
        correcto = false;
    }
    std::cout << nombre << ": ";
    if (correcto) { std::cout << "OK\n"; }
    else { std::cout << "FALLO\n"; }
    return correcto;
}

int main() {
    bool correcto{true};
    const Conjunto base{{Grupo{0, {{7, {0, 0}}, {2, {3, 1}}}}, Grupo{2, {}}}};
    const Conjunto vacio{};
    const Conjunto presente{{Grupo{0, {}}}};
    const Conjunto local{{Grupo{0, {{7, {1, 0}}}}, Grupo{2, {{7, {2, 1}}}}}};
    if (!probar("Base", "{ conjunto { grupo rojo 7[0;0] 2[3;1] } { grupo azul } }", true, base)) { correcto = false; }
    if (!probar("Vacio", "{ conjunto }", true, vacio)) { correcto = false; }
    if (!probar("Presente vacio", "{ conjunto { grupo rojo } }", true, presente)) { correcto = false; }
    if (!probar("ID local", "{ conjunto { grupo rojo 7[1;0] } { grupo azul 7[2;1] } }", true, local)) { correcto = false; }
    if (!probar("Grupo repetido", "{ conjunto { grupo rojo } { grupo rojo } }", false, vacio)) { correcto = false; }
    if (!probar("Registro repetido", "{ conjunto { grupo rojo 7[1;0] 7[2;1] } }", false, vacio)) { correcto = false; }
    if (!probar("Cantidad invalida", "{ conjunto { grupo rojo 7[11;0] } }", false, vacio)) { correcto = false; }
    if (!probar("Segundo truncado", "{ conjunto { grupo rojo } { grupo azul 2[1;0]", false, vacio)) { correcto = false; }
    if (!probar("Texto posterior", "{ conjunto } resto", false, vacio)) { correcto = false; }
    if (!probar("Sin cierre exterior", "{ conjunto { grupo rojo 0[1;0] }", false, vacio)) { correcto = false; }
    if (!correcto) { return 1; }
    return 0;
}
