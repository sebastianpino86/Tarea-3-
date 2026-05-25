SISTEMA DE BUSQUEDA EN UN LABERINTO

DESCRIPCION 

Este sistema fue creado con el objetivo de resolver un laberinto usando distintos algoritmos de búsqueda. El agente parte desde la posición inicial (0,0) y debe encontrar el camino hacia la meta (N-1,N-1) con distintos algoritmos de busqueda evitando obstáculos. El programa permite comparar tres estrategias de búsqueda distintas, mostrando el camino encontrado y los pasos realizados.


COMO COMPILAR Y EJECUTAR 


Este sistema fue desarrollado en el lenguaje C y puede ejecutarse usando Visual Studio Code mientras cuente con la extensión de C/C++. Para comenzar a trabajar con él se deben seguir los siguientes pasos: Tener instalado Visual Studio Code, contar con la extensión de C/C++ de Microsoft y tener instalado un compilador de C como GCC. Una vez descargado el programa, se debe primero compilar el código con el siguiente comando en el terminal: gcc tarea3.c tdas/*.c -o tarea3 -Wno-unused-result, una vez compilado se puede ejecutar con el siguiente comando : ./tarea3.


FUNCIONALIDADES


Este programa cuenta con 3 algoritmos de búsqueda principales: 

Búsqueda en Profundidad (DFS): Utiliza una pila para explorar el laberinto yendo tan profundo como sea posible antes de retroceder. No garantiza el camino más corto pero encuentra una solución si existe.

Búsqueda en Anchura (BFS): Utiliza una cola para explorar el laberinto nivel por nivel. Garantiza encontrar el camino con menor cantidad de pasos hasta la meta.

Búsqueda Mejor Primero (Best-First): Utiliza un Heap con prioridad basada en la suma del costo acumulado (pasos) más la heurística (distancia_L1), dirigiendo la búsqueda hacia la meta de forma más inteligente.


EJEMPLO DE USO 


El usuario ejecuta el programa e ingresa la dificultad del laberinto, por ejemplo 90 para un 90% de obstáculos. El programa genera el laberinto aleatoriamente y muestra el estado inicial con la posición del agente A, el inicio I y la meta M.

El usuario selecciona la opción 1 para ejecutar DFS. El programa encuentra un camino y muestra el laberinto con el recorrido marcado con +, junto al número de pasos realizados.

El usuario selecciona la opción 2 para ejecutar BFS. El programa encuentra el camino más corto y lo muestra marcado con +.

El usuario selecciona la opción 3 para ejecutar Best-First. El programa usa la distancia Manhattan como heurística para guiar la búsqueda y muestra el camino encontrado.

El usuario selecciona la opción 4 para salir. El programa finaliza su ejecución.
