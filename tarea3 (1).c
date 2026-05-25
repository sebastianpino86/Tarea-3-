#include <stdio.h>
#include <stdlib.h>
#include "tdas/list.h"
#include "tdas/heap.h"
#include "tdas/extra.h"
#include "tdas/queue.h"
#include "tdas/stack.h"
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

void imprimirCamino(State* estado){
    int maze[N][N];
    for(int i = 0; i < N ; i++){
        for(int j = 0; j < N ; j++){
            maze[i][j] = estado->maze[i][j];
        }
    }

    void* aux = list_first(estado->actions);
    while (aux != NULL){
        int x = ((int*)aux)[0];
        int y = ((int*)aux)[1];
        maze[x][y] = 2;
        aux = list_next(estado->actions);
    }
    maze[estado->x][estado->y] = 2;
    printf("Camino :\n");
    for (int k = 0; k < N; k ++){
        for(int l = 0; l < N; l ++){
            if (k == 0 && l == 0)printf(" I ");
            else if (k == N-1 && l== N-1)printf(" M ");
            else if (maze[k][l] == 2)printf(" + ");
            else if (maze[k][l] == 0)printf(" . ");
            else printf("[X]");
        } 
        printf("\n");
    }
}

void dfs(State estado){
    int visitado[N][N] = {0};
    Stack* pila = stack_create(pila);

    State* inicio = (State*)malloc(sizeof(State));
    *inicio = estado;
    inicio->actions = list_create();

    stack_push(pila,inicio);
    while(stack_top(pila)!= NULL){
        State* actual = (State*)stack_top(pila);
        stack_pop(pila);

        if (visitado[actual->x][actual->y]){
            free(actual);
            continue;
        }
        visitado[actual->x][actual->y] = 1;
        if (is_final_state(actual)){
            printf("Meta Encontrada\n");
            printf("Pasos : %d\n",actual->steps);
            imprimirCamino(actual);
            free(actual);
            break;
        }
        List* adyacentes = obtener_adyacentes(actual);
        State* posible = (State*)list_first(adyacentes);
        while(posible != NULL){
            if(!visitado[posible->x][posible->y]){
                stack_push(pila,posible);
            }
            posible = (State*)list_next(adyacentes);
        }
        list_clean(adyacentes);
        free(adyacentes);
        free(actual);
    }
    list_clean(pila);
    free(pila);
}
void bfs(State estado){
    int visitado[N][N] = {0};
    Queue* cola = queue_create(cola);

    State* inicio =(State*)malloc(sizeof(State));
    *inicio = estado;
    inicio->actions = list_create();

    queue_insert(cola,inicio);
    visitado[inicio->x][inicio->y] = 1;

    while(queue_front(cola) != NULL){
        State* actual = (State*)queue_front(cola);
        queue_remove(cola);

        if (is_final_state(actual)){
            printf("Pasos : %d\n",actual->steps);
            imprimirCamino(actual);
            free(actual);
            break;
        }
        List* adyacentes = obtener_adyacentes(actual);
        State* posible = (State*)list_first(adyacentes);
        while(posible != NULL){
            if (!visitado[posible->x][posible->y]){
                visitado[posible->x][posible->y] = 1;
                queue_insert(cola,posible);
            }
            posible = (State*)list_next(adyacentes);
        }
        list_clean(adyacentes);
        free(adyacentes);
        free(actual);
    }
    queue_clean(cola);
    free(cola);
}
void best_first(State estado){
    int visitado[N][N] = {0};
    Heap* heap = heap_create();

    State* inicio =(State*) malloc(sizeof(State));
    *inicio = estado;
    inicio->actions = list_create();

    int prioridad = -(inicio->steps + distancia_L1(inicio));
    heap_push(heap,inicio,prioridad);

    while(heap_top(heap) != NULL){
        State* actual = (State*)heap_top(heap);
        heap_pop(heap);

        if (visitado[actual->x][actual->y]){
            free(actual);
            continue;
        }
        visitado[actual->x][actual->y] = 1;

        if (is_final_state(actual)){
            printf("Pasos : %d\n",actual->steps);
            imprimirCamino(actual);
            free(actual);
            break;
        }
        List* adyacentes = obtener_adyacentes(actual);
        State* posible = (State*)list_first(adyacentes);
        while(posible != NULL){
            if (!visitado[posible->x][posible->y]){
                int priori = -(posible->steps + distancia_L1(posible));
                heap_push(heap,posible,priori);
            }
            posible = (State*)list_next(adyacentes);
        }
        list_clean(adyacentes);
        free(adyacentes);
        free(actual);
    }
}
int main() {
    // Inicializar la semilla de aleatoriedad
    srand(time(NULL));

    int maze[N][N];
    int dificultad;
    
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
          dfs(estado_inicial);
          break;
        case '2':
          bfs(estado_inicial);
          break;
        case '3':
          best_first(estado_inicial);
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
