# COMPUTACION_PARALELA_DISTRIBUIDA

### 3.1 Histograma Paralelo
Implementación de un programa paralelo para calcular un histograma. El proceso 0 lee un conjunto de datos y lo distribuye entre los procesos. Cada proceso calcula los conteos de los intervalos en sus datos locales, y el proceso 0 recopila y muestra el histograma final.

### 3.2 Lanzamiento de dardos
Este programa estima el valor de π lanzando "dardos" a una cuadrícula. Cada proceso genera coordenadas aleatorias dentro de un cuadrado y cuenta cuántos puntos caen dentro del círculo inscrito. El proceso 0 recopila los resultados de todos los procesos para calcular una estimación de π utilizando la proporción de puntos en el círculo respecto al total.

### 3.3 A - Suma global potencia 2
La implementación consiste en un programa que calcula la suma global de manera eficiente utilizando una estructura de árbol binario. En este enfoque, cada proceso del sistema calcula su propia suma local. Luego, mediante una comunicación en pares, los procesos envían y reciben sus sumas entre sí, lo que permite reducir gradualmente el total hasta que solo el proceso raíz tiene la suma global final.

### 3.3 B - Suma global 
El programa está diseñado para funcionar con cualquier cantidad de procesos, incluyendo aquellos casos en los que el número total de procesos no es una potencia de dos. En situaciones donde hay un número impar de procesos, el proceso raíz se encarga de recibir las sumas de los procesos que no tienen un par correspondiente, garantizando así que todos los procesos contribuyan al resultado final.

### 3.5 -  Multiplicación Matriz-Vector
Este programa realiza la multiplicación de una matriz cuadrada de orden 𝑛 y un vector mediante una distribución de bloques de columnas en un entorno paralelo utilizando MPI. El proceso con rango 0 es responsable de leer los elementos de la matriz y el vector desde la entrada. Luego, el vector se distribuye a todos los procesos a través de la función MPI_Bcast. La matriz se divide en partes iguales, enviando a cada proceso una sección de la matriz mediante MPI_Scatter.

Cada proceso lleva a cabo la multiplicación de su bloque de filas de la matriz con el vector recibido, almacenando los resultados locales en un vector. Finalmente, los resultados locales se recopilan en el proceso 0 utilizando MPI_Gather, que muestra el resultado final de la multiplicación de matriz y vector. 

### 3.8 - Merge Sort Paralelo
El programa realiza un ordenamiento por mezcla en paralelo, comenzando con un conjunto de claves asignadas a cada proceso. El proceso con rango 0 solicita el tamaño total de la lista y lo distribuye a los demás procesos. Cada proceso genera una lista local de enteros aleatorios, que se ordena utilizando la función std::sort. Luego, los procesos muestran sus listas ordenadas locales.

La fusión de listas se lleva a cabo mediante una comunicación estructurada en forma de árbol. Los procesos con rango par reciben listas de otros procesos, fusionando estas listas en su propia lista ordenada. Los procesos con rango impar envían sus listas a procesos de rango par. Este proceso se repite en varias etapas hasta que todos los elementos están ordenados y se recopilan en el proceso 0.
