#include "aplicacion.h"
#include <fstream>
#include <iostream>
#include <sstream>

void comprobar(const std::string& nombre, bool condicion, int& fallos) {
    std::cout << nombre << ": ";
    if (condicion) { std::cout << "OK\n"; }
    else { std::cout << "FALLO\n"; ++fallos; }
}

void probar_informe(const std::string& nombre, const Conjunto& conjunto,
                    const std::string& esperado, int& fallos) {
    const Conjunto previo = conjunto;
    std::ostringstream salida;
    const bool correcto = mostrar_informe(conjunto, salida);
    comprobar(nombre, correcto && salida.str() == esperado
        && mismos_datos(conjunto, previo), fallos);
}

bool escribir_texto(const std::string& ruta, const std::string& texto) {
    std::ofstream salida{ruta};
    if (!salida) { return false; }
    salida << texto;
    if (!salida) { return false; }
    salida.close();
    return static_cast<bool>(salida);
}

int main() {
    int fallos{0};
    const Conjunto base{{Grupo{0, {{7, {0, 0}}, {2, {3, 1}}}}, Grupo{2, {}}}};
    probar_informe("informe_base", base,
        "Grupos: 2\nAceptados: 3\nRechazados: 1\nMayor: rojo 3\nRegistros rojo: 2\nRegistros azul: 0\n", fallos);
    probar_informe("informe_vacio", Conjunto{},
        "Grupos: 0\nAceptados: 0\nRechazados: 0\nMayor: ninguno\n", fallos);
    probar_informe("informe_presente", Conjunto{{Grupo{2, {}}}},
        "Grupos: 1\nAceptados: 0\nRechazados: 0\nMayor: azul 0\nRegistros azul: 0\n", fallos);
    probar_informe("informe_orden", Conjunto{{Grupo{2, {}}, Grupo{0, {{7, {0, 0}}}}}},
        "Grupos: 2\nAceptados: 0\nRechazados: 0\nMayor: azul 0\nRegistros azul: 0\nRegistros rojo: 1\n", fallos);
    probar_informe("informe_cero", Conjunto{{Grupo{1, {{4, {0, 0}}}}}},
        "Grupos: 1\nAceptados: 0\nRechazados: 0\nMayor: verde 0\nRegistros verde: 1\n", fallos);
    Grupo diez{0, {}};
    for (int id = 0; id < 10; ++id) { diez.registros.push_back(Registro{id, {0, 0}}); }
    probar_informe("informe_diez", Conjunto{{diez}},
        "Grupos: 1\nAceptados: 0\nRechazados: 0\nMayor: rojo 0\nRegistros rojo: 10\n", fallos);
    std::ostringstream invalida;
    invalida << "MARCA";
    comprobar("informe_invalido", !mostrar_informe(Conjunto{{Grupo{3, {}}}}, invalida)
        && invalida.str() == "MARCA", fallos);

    const std::string texto{"{ conjunto { grupo rojo 7[0;0] 2[3;1] } { grupo azul } }"};
    if (!escribir_texto("pruebas_tmp/variante_entrada.txt", texto)) {
        comprobar("preparacion_entrada", false, fallos);
        return 1;
    }
    Conjunto actual{};
    std::ostringstream mensajes;
    int resultado = procesar_archivos("pruebas_tmp/variante_entrada.txt",
        "pruebas_tmp/variante_salida.txt", actual, mensajes);
    Conjunto recuperado{};
    bool leido{false};
    {
        std::ifstream entrada{"pruebas_tmp/variante_salida.txt"};
        if (entrada) { leido = leer_muestra_completa(entrada, recuperado) && !entrada.bad(); }
    }
    comprobar("archivo_variante", resultado == 0 && mismos_datos(actual, base)
        && leido && mismos_datos(recuperado, base)
        && mensajes.str() == "Entrada aceptada\nGrupos: 2\nAceptados: 3\nRechazados: 1\nMayor: rojo 3\nRegistros rojo: 2\nRegistros azul: 0\nGuardado verificado\n", fallos);

    if (!escribir_texto("pruebas_tmp/variante_entrada.txt", "{ conjunto } resto")
        || !escribir_texto("pruebas_tmp/variante_salida.txt", "MARCA")) {
        comprobar("preparacion_rechazo", false, fallos);
        return 1;
    }
    const Conjunto previo = actual;
    mensajes.str("");
    resultado = procesar_archivos("pruebas_tmp/variante_entrada.txt",
        "pruebas_tmp/variante_salida.txt", actual, mensajes);
    std::ifstream entrada{"pruebas_tmp/variante_salida.txt"};
    std::string guardado;
    char caracter{0};
    while (entrada.get(caracter)) { guardado += caracter; }
    comprobar("rechazo_variante", resultado == 2 && mismos_datos(actual, previo)
        && entrada.eof() && !entrada.bad() && guardado == "MARCA"
        && mensajes.str() == "Entrada rechazada. Estado conservado.\n", fallos);

    if (fallos != 0) { std::cout << "Variante: FALLO\n"; return 1; }
    std::cout << "Variante: OK\n";
    return 0;
}
