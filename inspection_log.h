#ifndef INSPECTION_LOG_H
#define INSPECTION_LOG_H

#include <istream>
#include <string>
#include <vector>

struct Recuento {
    int aceptados;
    int rechazados;
};

struct Registro {
    int id;
    Recuento recuento;
};

struct Grupo {
    int id;
    std::vector<Registro> registros;
};

struct Conjunto {
    std::vector<Grupo> grupos;
};

struct Informe_mayor {
    bool hay_grupo;
    int id;
    int aceptados;
};

// Grupos: 0 rojo, 1 verde, 2 azul. No modifica destino si id es invalido.
bool nombre_grupo(int id, std::string& destino);
// Comprueba nombres, unicidad local de IDs, cantidades y capacidades.
bool conjunto_valido(const Conjunto& conjunto);
// Acepta el conjunto completo y solo blancos hasta EOF.
// Rechazar conserva destino; no restaura los caracteres consumidos.
bool leer_muestra_completa(std::istream& entrada, Conjunto& destino);
// Rechazar un conjunto invalido conserva destino.
bool componer_conjunto(const Conjunto& conjunto, std::string& destino);
// Exige validez y compara presencia, orden, IDs y ambas cantidades.
bool mismos_datos(const Conjunto& primero, const Conjunto& segundo);
// Requiere un conjunto valido. Los empates conservan el primer grupo.
Informe_mayor seleccionar_mayor(const Conjunto& conjunto);

#endif
