# Programar desde cero con C++

Repositorio único del proyecto educativo de Programación 1. Su aplicación de consola consiste en un registro de inspecciones por grupos, cuya referencia didáctica se desarrolló inicialmente con el nombre interno «InspectionLog». El código, las pruebas, los ejemplos y la documentación se conservan juntos aquí, sin crear repositorios adicionales para partes o etapas del curso.

Esta implementación del autor permite contrastar una muestra de inspecciones, presentar un informe y guardar una copia recuperable. No acredita por sí misma el trabajo ni el dominio de un estudiante. El proyecto debe construirse y enseñarse progresivamente en paralelo al currículo, sin anticipar secciones que aún no se han estudiado ni declararlo finalizado.

## Contrato

- Grupos admitidos: rojo, verde y azul, cada uno como máximo una vez.
- Identificadores de registro: 0 a 9, únicos dentro de cada grupo.
- Aceptados y rechazados: cada cantidad entre 0 y 10.
- Se conservan el orden, los grupos presentes sin registros y los registros de ceros.
- Una entrada inválida conserva el estado previo y no abre la salida para escribir.
- El guardado se comprueba cerrando, reabriendo, interpretando y comparando todos los datos.

## Archivos

- inspection_log.h y modelo.cpp: tipos, interpretación, validación, composición, comparación y selección del máximo.
- aplicacion.h: contratos del informe y de la operación de archivos.
- informe.cpp: totales y grupo con más aceptados; los empates conservan el primero.
- archivos.cpp y main.cpp: lectura, guardado, verificación y consola.
- prueba_modelo.cpp: diez muestras del contrato original.
- pruebas.cpp: cuarenta y cinco comprobaciones adicionales.
- informe_variante.cpp y pruebas_variante.cpp: informe con registros por grupo y nueve comprobaciones.
- ejemplos/base.txt: muestra propia de demostración.
- depurar_informe.cpp e informe_error.cpp: muestra fija y copia con un defecto deliberado para practicar la depuración; no se incluyen en la construcción normal.

## Preparación y construcción en Windows

Requisitos: Visual Studio Community con la carga de desarrollo de escritorio con C++, compilador MSVC y Windows SDK. No se requieren bibliotecas externas. Abre la consola de herramientas nativas x64 de la instalación y comprueba where cl y cl. Entra en la raíz de esta carpeta de proyecto. Crea resultados y pruebas_tmp si todavía no existen.

Ejecuta cada orden por separado desde esa raíz:

```bat
mkdir resultados
mkdir pruebas_tmp
cl /W4 /EHsc modelo.cpp informe.cpp archivos.cpp main.cpp /Fe:inspectionlog.exe
cl /W4 /EHsc modelo.cpp prueba_modelo.cpp /Fe:prueba_modelo.exe
cl /W4 /EHsc modelo.cpp informe.cpp archivos.cpp pruebas.cpp /Fe:pruebas.exe
cl /W4 /EHsc modelo.cpp informe_variante.cpp archivos.cpp main.cpp /Fe:inspectionlog_variante.exe
cl /W4 /EHsc modelo.cpp informe_variante.cpp archivos.cpp pruebas_variante.cpp /Fe:pruebas_variante.exe
```

/W4 solicita advertencias, /EHsc conserva el modelo de excepciones usado en el curso y /Fe: nombra el ejecutable. Los fuentes se compilan y enlazan juntos; la cabecera se incorpora mediante #include. Cada orden contiene un solo main y una sola implementación del informe. Guarda los cambios y exige una construcción correcta antes de atribuir un resultado al código modificado.

## Demostración

Ejecuta .\inspectionlog.exe. Cuando pida la entrada, introduce ejemplos\base.txt; para la salida, resultados\base_guardada.txt. No escribas comillas alrededor de las rutas cuando las solicite el programa.

Después de los rótulos de entrada, el resultado esperado es:

```text
Entrada aceptada
Grupos: 2
Aceptados: 3
Rechazados: 1
Mayor: rojo 3
Guardado verificado
```

Consulta inmediatamente echo %ERRORLEVEL%: debe mostrar 0. type resultados\base_guardada.txt muestra el archivo conservado. La variante se ejecuta con .\inspectionlog_variante.exe y una salida diferente, como resultados\base_variante.txt; añade Registros rojo: 2 y Registros azul: 0 antes del mensaje final, sin cambiar el archivo de datos.

La aplicación devuelve 0 tras aceptar, guardar y verificar; 2 cuando rechaza el contenido; y 1 ante un problema técnico. Una muestra válida puede haber actualizado la memoria aunque falle después el guardado.

## Comprobación

Ejecuta .\prueba_modelo.exe, .\pruebas.exe y .\pruebas_variante.exe por separado. Después de cada ejecución consulta echo %ERRORLEVEL%. Todos los casos deben terminar en OK y cada proceso debe devolver 0. Las baterías usan archivos propios bajo pruebas_tmp, que se pueden sustituir en cada ejecución.

La contraprueba guiada cambia temporalmente > por >= en la comparación de seleccionar_mayor. Tras reconstruir, fallan los casos de empate y el informe del límite. Restaurar la comparación estricta y reconstruir recupera el comportamiento esperado. Es un error provocado para comprobar la sensibilidad de los casos.

## Depuración del defecto deliberado

La referencia conserva en informe_error.cpp una asignación que reemplaza el total de aceptados por el último registro. depurar_informe.cpp prepara un grupo verde con los registros 6[2;1] y 0[3;0]. El total correcto es 5; la copia defectuosa muestra 3. El informe de la aplicación, informe.cpp, conserva la acumulación correcta.

Desde la raíz, en la consola de desarrollo, construye el caso con símbolos y sin optimizar:

```bat
cl /W4 /EHsc /Zi /Od modelo.cpp informe_error.cpp depurar_informe.cpp /Fe:depurar_informe.exe
.\depurar_informe.exe
devenv /DebugExe depurar_informe.exe
```

/Zi produce la información de depuración y /Od desactiva optimizaciones. Conserva los fuentes y los archivos .pdb de esta construcción. En Visual Studio abre informe_error.cpp, coloca con F9 un punto en `aceptados = registro.recuento.aceptados;` y pulsa F5. En Variables locales observa aceptados y registro. F10 ejecuta la instrucción; F5 continúa hasta la siguiente visita. La primera asignación pasa de 0 a 2; la segunda reemplaza 2 por 3.

Detén la ejecución con Mayús+F5, cambia = por += en esa instrucción, guarda y repite la orden de construcción. Ejecuta y depura de nuevo: la segunda suma debe pasar de 2 a 5 y el informe debe mostrar 5 aceptados. Construye y ejecuta la batería contra esa misma copia corregida:

```bat
cl /W4 /EHsc modelo.cpp informe_error.cpp archivos.cpp pruebas.cpp /Fe:pruebas_corregidas.exe
.\pruebas_corregidas.exe
echo %ERRORLEVEL%
```

Deben pasar sus 45 comprobaciones y devolver 0. Como contraprueba, restaura temporalmente =, construye esa versión con otro nombre y ejecútala:

```bat
cl /W4 /EHsc modelo.cpp informe_error.cpp archivos.cpp pruebas.cpp /Fe:pruebas_error.exe
.\pruebas_error.exe
echo %ERRORLEVEL%
```

Esta versión debe fallar únicamente en informe_limite y en el resumen de la batería, con código 1. Después restaura +=, guarda y reconstruye el ejecutable de depuración. Es un defecto provocado para comprobar el diagnóstico y la sensibilidad de las pruebas, no una avería encontrada en la implementación correcta.

## Decisiones y alcance

El modelo no conoce las rutas ni la consola. El informe recibe un flujo y se puede comparar como texto. La variante sustituye una implementación del informe al construir, manteniendo modelo, persistencia y cliente. Contar registros incluye los de cantidades cero y presenta los grupos vacíos; sumar inspecciones tendría otro significado.

Se procesa una muestra por ejecución y se usan nombres de grupo fijos. Entrada y salida deben ser archivos distintos, sin alias de la misma ubicación. El programa compara literalmente las dos rutas, pero no resuelve todas sus equivalencias. La comprobación de escritura y relectura no implementa un reemplazo indivisible ni recuperación frente a una interrupción del equipo. Las pruebas de flujo cerrado son errores provocados; no demuestran averías físicas del disco.

La entrega del estudiante requiere su propio indicador adicional, casos que lo distingan de contar registros, una decisión documentada y evidencia de sus ejecuciones. Esta referencia no atribuye esa autoría ni esos resultados al estudiante.
