#ifndef INSPECTION_LOG_APLICACION_H
#define INSPECTION_LOG_APLICACION_H

#include "inspection_log.h"
#include <ostream>

// Un conjunto invalido se rechaza antes de escribir el informe.
bool mostrar_informe(const Conjunto& conjunto, std::ostream& salida);

// 0: aceptado, guardado y verificado; 2: contenido rechazado; 1: fallo tecnico.
// El rechazo conserva actual y no abre ruta_salida para escribir.
// Tras aceptar la carga, un fallo de guardado no revierte actual ni el disco.
// Las rutas deben nombrar archivos distintos, no alias del mismo archivo.
int procesar_archivos(const std::string& ruta_entrada,
                      const std::string& ruta_salida,
                      Conjunto& actual, std::ostream& mensajes);

#endif
