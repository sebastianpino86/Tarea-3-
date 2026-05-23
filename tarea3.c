#include <stdio.h>
#include <stdlib.h>
#include "tdas/list.h"
#include "tdas/heap.h"
#include "tdas/extra.h"
#include <string.h>
#include <time.h>



// Definición de la estructura para el estado del puzzle
typedef struct {
    int maze[N][N]; // Matriz NxN que representa el tablero
    int x;    // Posición x del agente
    int y;    // Posición x del agente
    int steps; // Pasos realizados hasta la posición actual
    List* actions; //Secuencia de movimientos para llegar al estado
} State;

int distancia_L1(State* state) {
    return abs(state->x - (N-1)) + abs(state->y - (N-1));
}


// Función para imprimir el estado actual
void imprimirEstado(const State *estado) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (estado->x==i && estado->y==j) printf(" A ");
            else if (i == 0 && j == 0) printf(" I "); 
            else if (i == N-1 && j == N-1) printf(" M ");
            else if (estado->maze[i][j] == 0)
                printf(" . "); // Imprime un espacio en blanco para el espacio vacío
            else
                printf("[X]");
        }
        printf("\n");
    }
}

State crearEstadoInicial(int maze[N][N], int dificultad){
    State estado;
     // Copiar el laberinto generado al estado
    generate_maze(estado.maze,  dificultad);
    estado.x = 0;
    estado.y = 0;
    estado.steps = 0;
    estado.actions = list_create();
    return estado;
}

int is_final_state(State* state){
    return (state->x == N-1 && state->y == N-1);
}

List* obtener_adyacentes(State* estado){
    List* adyacentes = list_create();

    for (int i = 0; i < 4 ; i++){
        int nuevox = estado->x;
        int nuevoy = estado->y;
        switch(i){
            case 0 : nuevox -= 1;break;
            case 1 : nuevox += 1;break;
            case 2 : nuevoy -= 1;break;
            case 3 : nuevoy += 1;break;
        }
        if (nuevox < 0 || nuevox >= N || nuevoy < 0 || nuevoy >= N)continue;

        if (estado->maze[nuevox][nuevoy] == 1)continue;

        State* nuevo = (State*)malloc(sizeof(State));
        *nuevo = *estado;

        nuevo->actions = list_create();
        void* aux = list_first(estado->actions);
        while(aux != NULL){
            int * coordenada = (int*)malloc(2 * sizeof(int));
            coordenada[0] = ((int*)aux)[0];
            coordenada[1] = ((int*)aux)[1];
            list_pushBack(nuevo->actions, coordenada);
            aux = list_next(estado->actions);
        }
        int* anterior = (int*)malloc(2 * sizeof(int));
        anterior[0] = estado->x;
        anterior[1] = estado->y;
        list_pushBack(nuevo->actions, anterior);

        nuevo->x = nuevox;
        nuevo->y = nuevoy;
        nuevo->steps = estado->steps + 1;

        list_pushBack(adyacentes, nuevo);
    }
    return adyacentes; 
}
int main() {
    // Inicializar la semilla de aleatoriedad
    srand(time(NULL));

    int maze[N][N];
    int dificultad;
    
    State estado1;
    estado1.x = 5;
    estado1.y = 5;
    estado1.steps = 0;
    estado1.actions = list_create();
    generate_maze(estado1.maze, 60);
    printf("Maze de Ejemplo\n");
    imprimirEstado(&estado1);
    List* adyacentes1 = obtener_adyacentes(&estado1);
    State* siguiente1 = (State*)list_first(adyacentes1);
    while(siguiente1 != NULL){
        printf("Adyacente (%d, %d)\n",siguiente1->x,siguiente1->y);
        siguiente1 = (State*)list_next(adyacentes1);
    }
    // Solicitar la dificultad al usuario con validación
    do {
        printf("Ingrese la dificultad del laberinto (porcentaje de obstáculos, 0 a 100): ");
        scanf("%d", &dificultad);
        if (dificultad < 0 || dificultad > 100) {
            printf("Error: Por favor ingrese un valor válido entre 0 y 100.\n");
        }
    } while (dificultad < 0 || dificultad > 100);

    // Estado inicial del puzzle
    // Laberinto generado con la dificultad ingresada por el usuario
    State estado_inicial = crearEstadoInicial(maze, dificultad);

    // Imprime el estado inicial
    printf("\nEstado inicial del puzzle:\n");
    imprimirEstado(&estado_inicial);
    
    printf("Distancia L1: %d\n", distancia_L1(&estado_inicial));
    List* adyacentes = obtener_adyacentes(&estado_inicial);
    State* siguiente = (State*) list_first(adyacentes);
    while (siguiente != NULL){
        printf("Adyacentes = (%d, %d)\n",siguiente->x,siguiente->y);
        siguiente = (State*)list_next(adyacentes);
    }
    
    char opcion;
    do {
        printf("\n***** EJEMPLO MENU ******\n");
        puts("========================================");
        puts("     Escoge método de búsqueda");
        puts("========================================");

        puts("1) Búsqueda en Profundidad");
        puts("2) Buscar en Anchura");
        puts("3) Buscar Mejor Primero");
        puts("4) Salir");

        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);

        switch (opcion) {
        case '1':
          //dfs(estado_inicial);
          break;
        case '2':
          //bfs(estado_inicial);
          break;
        case '3':
          //best_first(estado_inicial);
          break;
        }

        // Evitamos pausar y limpiar pantalla si el usuario eligió salir
        if (opcion != '4') {
            presioneTeclaParaContinuar();
            limpiarPantalla();
        }

  } while (opcion != '4');

  return 0;
}
