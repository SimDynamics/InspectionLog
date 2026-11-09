#include "aplicacion.h"

bool mostrar_informe(const Conjunto& conjunto, std::ostream& salida) {
    if (!conjunto_valido(conjunto)) { return false; }
    int aceptados{0};
    int rechazados{0};
    for (const Grupo& grupo : conjunto.grupos) {
        for (const Registro& registro : grupo.registros) {
            aceptados += registro.recuento.aceptados;
            rechazados += registro.recuento.rechazados;
        }
    }
    salida << "Grupos: " << conjunto.grupos.size()
           << "\nAceptados: " << aceptados
           << "\nRechazados: " << rechazados << "\n";
    const Informe_mayor mayor = seleccionar_mayor(conjunto);
    if (mayor.hay_grupo) {
        std::string nombre;
        if (!nombre_grupo(mayor.id, nombre)) { return false; }
        salida << "Mayor: " << nombre << ' ' << mayor.aceptados << "\n";
    } else {
        salida << "Mayor: ninguno\n";
    }
    return static_cast<bool>(salida);
}
