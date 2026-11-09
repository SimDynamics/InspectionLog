#include "aplicacion.h"
#include <fstream>
#include <iostream>
#include <sstream>

void comprobar(const std::string& nombre, bool condicion, int& fallos) {
    std::cout << nombre << ": ";
    if (condicion) { std::cout << "OK\n"; }
    else { std::cout << "FALLO\n"; ++fallos; }
}

bool escribir_texto(const std::string& ruta, const std::string& texto) {
    std::ofstream salida{ruta};
    if (!salida) { return false; }
    salida << texto;
    if (!salida) { return false; }
    salida.close();
    return static_cast<bool>(salida);
}

bool leer_texto(const std::string& ruta, std::string& destino) {
    std::ifstream entrada{ruta};
    if (!entrada) { return false; }
    std::string candidato;
    char caracter{0};
    while (entrada.get(caracter)) { candidato += caracter; }
    if (entrada.bad() || !entrada.eof()) { return false; }
    destino = candidato;
    return true;
}

void probar_mayor(const std::string& nombre, const Conjunto& conjunto,
                  bool hay_grupo, int id, int aceptados, int& fallos) {
    const Conjunto previo = conjunto;
    const Informe_mayor resultado = seleccionar_mayor(conjunto);
    comprobar(nombre, resultado.hay_grupo == hay_grupo
        && resultado.id == id && resultado.aceptados == aceptados
        && mismos_datos(previo, conjunto), fallos);
}

void probar_rechazo(const std::string& nombre, const std::string& texto,
                    int& fallos) {
    const Conjunto previo{{Grupo{1, {{9, {6, 2}}}}}};
    Conjunto actual = previo;
    std::istringstream entrada{texto};
    const bool aceptado = leer_muestra_completa(entrada, actual);
    comprobar(nombre, !aceptado && mismos_datos(previo, actual), fallos);
}

void probar_informe(const std::string& nombre, const Conjunto& conjunto,
                    const std::string& esperado, int& fallos) {
    std::ostringstream salida;
    const bool correcto = mostrar_informe(conjunto, salida);
    comprobar(nombre, correcto && salida.str() == esperado, fallos);
}

void probar_rechazo_archivo(const std::string& nombre,
                            const std::string& texto, int& fallos) {
    if (!escribir_texto("pruebas_tmp/entrada.txt", texto)
        || !escribir_texto("pruebas_tmp/salida.txt", "MARCA")) {
        comprobar(nombre + " preparacion", false, fallos);
        return;
    }
    const Conjunto previo{{Grupo{1, {{9, {6, 2}}}}}};
    Conjunto actual = previo;
    std::ostringstream mensajes;
    const int resultado = procesar_archivos("pruebas_tmp/entrada.txt",
        "pruebas_tmp/salida.txt", actual, mensajes);
    std::string guardado;
    const bool leido = leer_texto("pruebas_tmp/salida.txt", guardado);
    comprobar(nombre, resultado == 2 && mismos_datos(actual, previo)
        && leido && guardado == "MARCA"
        && mensajes.str() == "Entrada rechazada. Estado conservado.\n", fallos);
}

int main() {
    int fallos{0};
    const Conjunto por_suma{{Grupo{0, {{1, {4, 0}}, {2, {4, 0}}}},
                            Grupo{2, {{3, {7, 0}}}}}};
    probar_mayor("maximo_por_suma", por_suma, true, 0, 8, fallos);
    probar_mayor("maximo_sin_grupos", Conjunto{}, false, -1, 0, fallos);
    probar_mayor("maximo_grupo_vacio", Conjunto{{Grupo{2, {}}}}, true, 2, 0, fallos);
    probar_mayor("empate_orden", Conjunto{{Grupo{2, {{0, {5, 0}}}},
        Grupo{0, {{1, {2, 0}}, {2, {3, 0}}}}}}, true, 2, 5, fallos);
    probar_mayor("empate_vacio_primero", Conjunto{{Grupo{2, {}},
        Grupo{0, {{7, {0, 0}}}}}}, true, 2, 0, fallos);
    probar_mayor("maximo_posterior", Conjunto{{Grupo{0, {{0, {2, 0}}}},
        Grupo{1, {{0, {9, 0}}}}}}, true, 1, 9, fallos);
    probar_mayor("maximo_intermedio", Conjunto{{Grupo{2, {{0, {1, 0}}}},
        Grupo{0, {{0, {9, 0}}}}, Grupo{1, {{0, {7, 0}}}}}}, true, 0, 9, fallos);
    probar_mayor("empate_tres_ceros", Conjunto{{Grupo{2, {}}, Grupo{1, {}},
        Grupo{0, {}}}}, true, 2, 0, fallos);

    const Conjunto base{{Grupo{0, {{7, {0, 0}}, {2, {3, 1}}}}, Grupo{2, {}}}};
    comprobar("comparacion_igual", mismos_datos(base, base), fallos);
    Conjunto cambio{{base.grupos.at(1), base.grupos.at(0)}};
    comprobar("comparacion_orden_grupos", !mismos_datos(base, cambio), fallos);
    cambio = base;
    cambio.grupos.at(0).registros = {base.grupos.at(0).registros.at(1),
                                    base.grupos.at(0).registros.at(0)};
    comprobar("comparacion_orden_registros", !mismos_datos(base, cambio), fallos);
    cambio = Conjunto{{base.grupos.at(0)}};
    comprobar("comparacion_presencia_vacia", !mismos_datos(base, cambio), fallos);
    cambio = base;
    cambio.grupos.at(0).registros = {base.grupos.at(0).registros.at(1)};
    comprobar("comparacion_registro_cero", !mismos_datos(base, cambio), fallos);
    cambio = base;
    cambio.grupos.at(0).registros.at(1).recuento.rechazados = 2;
    comprobar("comparacion_rechazados", !mismos_datos(base, cambio), fallos);
    cambio = base;
    cambio.grupos.at(0).registros.at(1).id = 7;
    comprobar("comparacion_invalida", !mismos_datos(cambio, cambio), fallos);
    std::string compuesto{"MARCA"};
    comprobar("composicion_no_sustituye", !componer_conjunto(cambio, compuesto)
        && compuesto == "MARCA", fallos);

    Conjunto limite{};
    for (int grupo = 0; grupo < 3; ++grupo) {
        Grupo nuevo{grupo, {}};
        for (int id = 0; id < 10; ++id) {
            nuevo.registros.push_back(Registro{id, {10, 10}});
        }
        limite.grupos.push_back(nuevo);
    }
    comprobar("limite_valido", conjunto_valido(limite), fallos);
    compuesto.clear();
    const bool compuesto_limite = componer_conjunto(limite, compuesto);
    std::istringstream lectura_limite{compuesto};
    Conjunto vuelta{};
    const bool leido_limite = leer_muestra_completa(lectura_limite, vuelta);
    comprobar("limite_ida_y_vuelta", compuesto_limite && leido_limite
        && mismos_datos(limite, vuelta), fallos);
    probar_informe("informe_limite", limite,
        "Grupos: 3\nAceptados: 300\nRechazados: 300\nMayor: rojo 100\n", fallos);

    probar_rechazo("nombre_desconocido", "{ conjunto { grupo negro } }", fallos);
    probar_rechazo("id_negativo", "{ conjunto { grupo rojo -1[0;0] } }", fallos);
    probar_rechazo("id_diez", "{ conjunto { grupo rojo 10[0;0] } }", fallos);
    probar_rechazo("cantidad_negativa", "{ conjunto { grupo rojo 0[-1;0] } }", fallos);
    probar_rechazo("rechazados_once", "{ conjunto { grupo rojo 0[0;11] } }", fallos);
    probar_rechazo("entero_excesivo", "{ conjunto { grupo rojo 0[999999999999999999999999999999;0] } }", fallos);
    probar_rechazo("separador_incorrecto", "{ conjunto { grupo rojo 0[1:2] } }", fallos);
    probar_rechazo("apertura_incorrecta", "[ conjunto }", fallos);
    probar_rechazo("texto_posterior", "{ conjunto } resto", fallos);
    probar_rechazo("duplicado_al_final", "{ conjunto { grupo azul } { grupo rojo } { grupo azul } }", fallos);
    std::istringstream blancos{" \n\t{ conjunto { grupo azul } }\r\n\t "};
    Conjunto con_blancos{};
    comprobar("blancos_completos", leer_muestra_completa(blancos, con_blancos)
        && mismos_datos(con_blancos, Conjunto{{Grupo{2, {}}}}), fallos);
    std::istringstream averiada{"{ conjunto }"};
    averiada.setstate(std::ios::badbit);
    Conjunto conservado = base;
    comprobar("flujo_averiado", !leer_muestra_completa(averiada, conservado)
        && mismos_datos(conservado, base), fallos);

    probar_informe("informe_base", base,
        "Grupos: 2\nAceptados: 3\nRechazados: 1\nMayor: rojo 3\n", fallos);
    probar_informe("informe_vacio", Conjunto{},
        "Grupos: 0\nAceptados: 0\nRechazados: 0\nMayor: ninguno\n", fallos);
    probar_informe("informe_presente_vacio", Conjunto{{Grupo{2, {}}}},
        "Grupos: 1\nAceptados: 0\nRechazados: 0\nMayor: azul 0\n", fallos);
    std::ostringstream sin_informe;
    sin_informe << "MARCA";
    comprobar("informe_invalido", !mostrar_informe(cambio, sin_informe)
        && sin_informe.str() == "MARCA", fallos);

    probar_rechazo_archivo("archivo_cantidad", "{ conjunto { grupo rojo 0[11;0] } }", fallos);
    probar_rechazo_archivo("archivo_duplicado", "{ conjunto { grupo rojo } { grupo rojo } }", fallos);
    probar_rechazo_archivo("archivo_truncado", "{ conjunto { grupo rojo } { grupo azul 2[1;0]", fallos);
    probar_rechazo_archivo("archivo_texto_posterior", "{ conjunto } resto", fallos);

    const std::string texto_base{"{ conjunto { grupo rojo 7[0;0] 2[3;1] } { grupo azul } }"};
    if (!escribir_texto("pruebas_tmp/entrada.txt", texto_base)
        || !escribir_texto("pruebas_tmp/bloqueo.txt", "Archivo normal")) {
        comprobar("preparacion_archivos", false, fallos);
        return 1;
    }
    Conjunto actual{};
    std::ostringstream mensajes;
    int resultado = procesar_archivos("pruebas_tmp/entrada.txt",
        "pruebas_tmp/salida.txt", actual, mensajes);
    std::string guardado;
    comprobar("archivo_valido", resultado == 0 && mismos_datos(actual, base)
        && leer_texto("pruebas_tmp/salida.txt", guardado) && guardado == texto_base
        && mensajes.str() == "Entrada aceptada\nGrupos: 2\nAceptados: 3\nRechazados: 1\nMayor: rojo 3\nGuardado verificado\n", fallos);

    const Conjunto previo{{Grupo{1, {{9, {6, 2}}}}}};
    actual = previo;
    mensajes.str("");
    resultado = procesar_archivos("pruebas_tmp/bloqueo.txt/entrada.txt",
        "pruebas_tmp/salida.txt", actual, mensajes);
    comprobar("entrada_no_abre", resultado == 1 && mismos_datos(actual, previo)
        && leer_texto("pruebas_tmp/salida.txt", guardado) && guardado == texto_base
        && mensajes.str() == "Error al abrir entrada\n", fallos);
    actual = previo;
    mensajes.str("");
    resultado = procesar_archivos("pruebas_tmp/entrada.txt",
        "pruebas_tmp/bloqueo.txt/salida.txt", actual, mensajes);
    comprobar("salida_no_abre", resultado == 1 && mismos_datos(actual, base)
        && mensajes.str() == "Error al abrir salida\n", fallos);
    actual = previo;
    mensajes.str("");
    resultado = procesar_archivos("pruebas_tmp/entrada.txt",
        "pruebas_tmp/entrada.txt", actual, mensajes);
    comprobar("rutas_iguales", resultado == 1 && mismos_datos(actual, previo)
        && leer_texto("pruebas_tmp/entrada.txt", guardado) && guardado == texto_base
        && mensajes.str() == "Rutas vacias o iguales\n", fallos);

    std::ofstream cerrado{"pruebas_tmp/cerrado.txt"};
    if (!cerrado) { comprobar("preparacion_cerrado", false, fallos); return 1; }
    cerrado.close();
    if (!cerrado) { comprobar("cierre_inicial", false, fallos); return 1; }
    cerrado << "MARCA";
    comprobar("escritura_tras_cierre", cerrado.fail()
        && leer_texto("pruebas_tmp/cerrado.txt", guardado) && guardado.empty(), fallos);
    std::ofstream doble{"pruebas_tmp/doble.txt"};
    if (!doble) { comprobar("preparacion_doble", false, fallos); return 1; }
    doble << "MARCA";
    if (!doble) { comprobar("escritura_doble", false, fallos); return 1; }
    doble.close();
    if (!doble) { comprobar("primer_cierre", false, fallos); return 1; }
    doble.close();
    comprobar("segundo_cierre", doble.fail()
        && leer_texto("pruebas_tmp/doble.txt", guardado) && guardado == "MARCA", fallos);

    if (fallos != 0) { std::cout << "Bateria completa: FALLO\n"; return 1; }
    std::cout << "Bateria completa: OK\n";
    return 0;
}
