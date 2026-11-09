#include "inspection_log.h"
#include <ostream>
#include <sstream>

const std::vector<std::string> nombres_grupos{"rojo", "verde", "azul"};

bool buscar_grupo(const std::string& nombre, int& destino) {
    for (int id = 0; id < 3; ++id) {
        if (nombres_grupos.at(id) == nombre) {
            destino = id;
            return true;
        }
    }
    return false;
}

bool nombre_grupo(int id, std::string& destino) {
    if (id < 0 || id >= 3) {
        return false;
    }
    destino = nombres_grupos.at(id);
    return true;
}

bool recuento_valido(const Recuento& valor) {
    return valor.aceptados >= 0 && valor.aceptados <= 10
        && valor.rechazados >= 0 && valor.rechazados <= 10;
}

std::ostream& operator<<(std::ostream& destino, const Recuento& valor) {
    return destino << '[' << valor.aceptados << ';' << valor.rechazados << ']';
}

std::istream& operator>>(std::istream& origen, Recuento& destino) {
    char abre{0};
    char separador{0};
    char cierra{0};
    int aceptados{0};
    int rechazados{0};
    if (!(origen >> abre >> aceptados >> separador >> rechazados >> cierra)) {
        return origen;
    }
    Recuento candidato{aceptados, rechazados};
    if (abre != '[' || separador != ';' || cierra != ']'
        || !recuento_valido(candidato)) {
        origen.setstate(std::ios::failbit);
        return origen;
    }
    destino = candidato;
    return origen;
}

bool contiene_registro(const Grupo& grupo, int id) {
    for (const Registro& registro : grupo.registros) {
        if (registro.id == id) { return true; }
    }
    return false;
}

bool leer_grupo(std::istream& entrada, Grupo& destino) {
    char abre{0};
    std::string etiqueta;
    std::string nombre;
    if (!(entrada >> abre >> etiqueta >> nombre)
        || abre != '{' || etiqueta != "grupo") {
        return false;
    }
    int id{-1};
    if (!buscar_grupo(nombre, id)) { return false; }
    Grupo candidato{id, {}};
    while (true) {
        entrada >> std::ws;
        if (!entrada || entrada.eof()) { return false; }
        if (entrada.peek() == '}') {
            entrada.get();
            if (!entrada) { return false; }
            destino = candidato;
            return true;
        }
        Registro registro{0, {0, 0}};
        if (!(entrada >> registro.id >> registro.recuento)
            || registro.id < 0 || registro.id > 9
            || contiene_registro(candidato, registro.id)) {
            return false;
        }
        candidato.registros.push_back(registro);
    }
}

bool contiene_grupo(const Conjunto& conjunto, int id) {
    for (const Grupo& grupo : conjunto.grupos) {
        if (grupo.id == id) { return true; }
    }
    return false;
}

bool leer_conjunto(std::istream& entrada, Conjunto& destino) {
    char abre{0};
    std::string etiqueta;
    if (!(entrada >> abre >> etiqueta)
        || abre != '{' || etiqueta != "conjunto") {
        return false;
    }
    Conjunto candidato{};
    while (true) {
        entrada >> std::ws;
        if (!entrada || entrada.eof()) { return false; }
        if (entrada.peek() == '}') {
            entrada.get();
            if (!entrada) { return false; }
            destino = candidato;
            return true;
        }
        Grupo grupo{0, {}};
        if (!leer_grupo(entrada, grupo)
            || contiene_grupo(candidato, grupo.id)) {
            return false;
        }
        candidato.grupos.push_back(grupo);
    }
}

bool leer_muestra_completa(std::istream& entrada, Conjunto& destino) {
    Conjunto candidato{};
    if (!leer_conjunto(entrada, candidato)) { return false; }
    entrada >> std::ws;
    if (entrada.bad() || !entrada.eof()) { return false; }
    destino = candidato;
    return true;
}

bool grupo_valido(const Grupo& grupo) {
    if (grupo.id < 0 || grupo.id >= 3 || grupo.registros.size() > 10) {
        return false;
    }
    for (const Registro& registro : grupo.registros) {
        if (registro.id < 0 || registro.id > 9
            || !recuento_valido(registro.recuento)) {
            return false;
        }
        int coincidencias{0};
        for (const Registro& otro : grupo.registros) {
            if (otro.id == registro.id) { ++coincidencias; }
        }
        if (coincidencias != 1) { return false; }
    }
    return true;
}

bool conjunto_valido(const Conjunto& conjunto) {
    if (conjunto.grupos.size() > 3) { return false; }
    for (const Grupo& grupo : conjunto.grupos) {
        if (!grupo_valido(grupo)) { return false; }
        int coincidencias{0};
        for (const Grupo& otro : conjunto.grupos) {
            if (otro.id == grupo.id) { ++coincidencias; }
        }
        if (coincidencias != 1) { return false; }
    }
    return true;
}

bool componer_conjunto(const Conjunto& conjunto, std::string& destino) {
    if (!conjunto_valido(conjunto)) { return false; }
    std::ostringstream salida;
    salida << "{ conjunto";
    for (const Grupo& grupo : conjunto.grupos) {
        std::string nombre;
        if (!nombre_grupo(grupo.id, nombre)) { return false; }
        salida << " { grupo " << nombre;
        for (const Registro& registro : grupo.registros) {
            salida << ' ' << registro.id << registro.recuento;
        }
        salida << " }";
    }
    salida << " }";
    if (!salida) { return false; }
    destino = salida.str();
    return true;
}

bool mismos_datos(const Conjunto& primero, const Conjunto& segundo) {
    if (!conjunto_valido(primero) || !conjunto_valido(segundo)
        || primero.grupos.size() != segundo.grupos.size()) {
        return false;
    }
    const int cantidad_grupos = static_cast<int>(primero.grupos.size());
    for (int i = 0; i < cantidad_grupos; ++i) {
        const Grupo& a = primero.grupos.at(i);
        const Grupo& b = segundo.grupos.at(i);
        if (a.id != b.id || a.registros.size() != b.registros.size()) {
            return false;
        }
        const int cantidad_registros = static_cast<int>(a.registros.size());
        for (int j = 0; j < cantidad_registros; ++j) {
            const Registro& x = a.registros.at(j);
            const Registro& y = b.registros.at(j);
            if (x.id != y.id || x.recuento.aceptados != y.recuento.aceptados
                || x.recuento.rechazados != y.recuento.rechazados) {
                return false;
            }
        }
    }
    return true;
}

Informe_mayor seleccionar_mayor(const Conjunto& conjunto) {
    Informe_mayor mejor{false, -1, 0};
    for (const Grupo& grupo : conjunto.grupos) {
        int total{0};
        for (const Registro& registro : grupo.registros) {
            total += registro.recuento.aceptados;
        }
        if (!mejor.hay_grupo || total > mejor.aceptados) {
            mejor = Informe_mayor{true, grupo.id, total};
        }
    }
    return mejor;
}
