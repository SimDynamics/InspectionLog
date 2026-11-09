#include "aplicacion.h"
#include <fstream>

int procesar_archivos(const std::string& ruta_entrada,
                      const std::string& ruta_salida,
                      Conjunto& actual, std::ostream& mensajes) {
    if (ruta_entrada.empty() || ruta_salida.empty()
        || ruta_entrada == ruta_salida) {
        mensajes << "Rutas vacias o iguales\n";
        return 1;
    }

    Conjunto candidato{};
    {
        std::ifstream entrada{ruta_entrada};
        if (!entrada) {
            mensajes << "Error al abrir entrada\n";
            return 1;
        }
        const bool aceptado = leer_muestra_completa(entrada, candidato);
        if (entrada.bad()) {
            mensajes << "Averia de lectura\n";
            return 1;
        }
        if (!aceptado) {
            mensajes << "Entrada rechazada. Estado conservado.\n";
            return 2;
        }
    }
    actual = candidato;

    std::string texto;
    if (!componer_conjunto(actual, texto)) {
        mensajes << "Error al componer datos\n";
        return 1;
    }
    std::ofstream guardado{ruta_salida};
    if (!guardado) {
        mensajes << "Error al abrir salida\n";
        return 1;
    }
    guardado << texto;
    if (!guardado) {
        mensajes << "Error al escribir salida\n";
        return 1;
    }
    guardado.close();
    if (!guardado) {
        mensajes << "Error al cerrar salida\n";
        return 1;
    }

    Conjunto recuperado{};
    {
        std::ifstream comprobacion{ruta_salida};
        if (!comprobacion) {
            mensajes << "Error al reabrir salida\n";
            return 1;
        }
        if (!leer_muestra_completa(comprobacion, recuperado)
            || comprobacion.bad()) {
            mensajes << "Error al releer salida\n";
            return 1;
        }
    }
    if (!mismos_datos(actual, recuperado)) {
        mensajes << "Error al comparar recuperacion\n";
        return 1;
    }
    mensajes << "Entrada aceptada\n";
    if (!mostrar_informe(actual, mensajes)) { return 1; }
    mensajes << "Guardado verificado\n";
    if (!mensajes) { return 1; }
    return 0;
}
