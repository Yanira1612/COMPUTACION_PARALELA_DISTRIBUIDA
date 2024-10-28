# COMPUTACION_PARALELA_DISTRIBUIDA

# MPI Histogram Calculation

Este programa en C++ utiliza la biblioteca MPI (Message Passing Interface) para calcular un histograma de un conjunto de datos distribuidos entre múltiples procesos. El histograma se divide en un número específico de "bins" (contenedores) y cada proceso cuenta los valores en su rango asignado.

## Descripción del Código

El programa realiza las siguientes tareas:

1. **Inicialización de MPI**: Se inicializan MPI y se obtienen el rango y tamaño del comunicador.
2. **Definición de Parámetros**:
    - `min_meas`: El valor mínimo de las mediciones (0.0).
    - `max_meas`: El valor máximo de las mediciones (5.0).
    - `bin_count`: El número de bins en los que se dividirán los datos (5).
3. **Generación de Datos**: Se define un vector de datos de ejemplo.
4. **Cálculo del Ancho de Bin**: Se calcula el ancho de cada bin basado en el rango de datos y el número de bins.
5. **Distribución de Datos**: Cada proceso calcula su rango de datos y cuenta los valores en los bins correspondientes.
6. **Reducción de Resultados**: Se utiliza `MPI_Reduce` para sumar los resultados locales de cada proceso en un arreglo global que contiene los conteos de cada bin.
7. **Salida de Resultados**: Solo el proceso con rango 0 imprime el histograma resultante.

## Requisitos

- Un entorno de desarrollo con soporte para C++.
- MPI instalado en tu sistema (por ejemplo, OpenMPI o MPICH).

## Compilación

Para compilar el programa, utiliza el siguiente comando:

```bash
mpic++ -o histogram histogram.cpp
