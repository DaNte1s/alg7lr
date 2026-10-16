#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <windows.h>

struct Graph {
    int n;
    int** adj;
};

struct Node {
    int val;
    struct Node* next;
};

struct AdjList {
    int n;
    struct Node** head;
};

struct Graph* create_graph(int n) {
    struct Graph* g = (struct Graph*)malloc(sizeof(struct Graph));
    g->n = n;
    g->adj = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        g->adj[i] = (int*)calloc(n, sizeof(int));
    }
    return g;
}

void free_graph(struct Graph* g) {
    if (!g) return;
    for (int i = 0; i < g->n; i++) {
        free(g->adj[i]);
    }
    free(g->adj);
    free(g);
}

void print_adj(struct Graph* g) {
    printf("Матрица смежности:\n    ");
    for (int j = 0; j < g->n; j++) {
        printf("v%-2d ", j + 1);
    }
    printf("\n");
    for (int i = 0; i < g->n; i++) {
        printf("v%-2d ", i + 1);
        for (int j = 0; j < g->n; j++) {
            printf("%-4d", g->adj[i][j]);
        }
        printf("\n");
    }
}

struct Graph* random_graph(int n, int prob) {
    struct Graph* g = create_graph(n);
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (rand() % 100 < prob) {
                g->adj[i][j] = g->adj[j][i] = 1;
            }
        }
    }
    return g;
}

struct AdjList* create_adj_list(int n) {
    struct AdjList* l = (struct AdjList*)malloc(sizeof(struct AdjList));
    l->n = n;
    l->head = (struct Node**)calloc(n, sizeof(struct Node*));
    return l;
}

void free_adj_list(struct AdjList* l) {
    if (!l) return;
    for (int i = 0; i < l->n; i++) {
        struct Node* curr = l->head[i];
        while (curr) {
            struct Node* tmp = curr;
            curr = curr->next;
            free(tmp);
        }
    }
    free(l->head);
    free(l);
}

void add_node(struct AdjList* l, int u, int v) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->val = v;
    node->next = NULL;
    if (!l->head[u]) {
        l->head[u] = node;
    } else {
        struct Node* curr = l->head[u];
        while (curr->next) curr = curr->next;
        curr->next = node;
    }
}

struct AdjList* matrix_to_list(struct Graph* g) {
    struct AdjList* l = create_adj_list(g->n);
    for (int i = 0; i < g->n; i++) {
        for (int j = 0; j < g->n; j++) {
            if (g->adj[i][j]) {
                add_node(l, i, j + 1);
            }
        }
    }
    return l;
}

void print_list(struct AdjList* l) {
    printf("Списки смежности:\n");
    for (int i = 0; i < l->n; i++) {
        printf("v%d: ", i + 1);
        struct Node* curr = l->head[i];
        while (curr) {
            printf("v%d ", curr->val);
            curr = curr->next;
        }
        printf("\n");
    }
}

void dfs_matrix_rec(struct Graph* g, int v, int* num) {
    num[v] = 1;
    printf("%d ", v + 1);
    for (int i = 0; i < g->n; i++) {
        if (g->adj[v][i] == 1 && !num[i]) {
            dfs_matrix_rec(g, i, num);
        }
    }
}

void pog_matrix_rec(struct Graph* g, int start_v) {
    int* num = (int*)calloc(g->n, sizeof(int));
    printf("Обход (матрица, рекурсивно): ");
    if (start_v >= 0 && start_v < g->n) {
        dfs_matrix_rec(g, start_v, num);
    }
    for (int v = 0; v < g->n; v++) {
        if (!num[v]) {
            dfs_matrix_rec(g, v, num);
        }
    }
    printf("\n");
    free(num);
}

void dfs_list_rec(struct AdjList* l, int v, int* num) {
    num[v] = 1;
    printf("%d ", v + 1);
    struct Node* curr = l->head[v];
    while (curr) {
        int to = curr->val - 1;
        if (!num[to]) {
            dfs_list_rec(l, to, num);
        }
        curr = curr->next;
    }
}

void pog_list_rec(struct AdjList* l, int start_v) {
    int* num = (int*)calloc(l->n, sizeof(int));
    printf("Обход (списки, рекурсивно): ");
    if (start_v >= 0 && start_v < l->n) {
        dfs_list_rec(l, start_v, num);
    }
    for (int v = 0; v < l->n; v++) {
        if (!num[v]) {
            dfs_list_rec(l, v, num);
        }
    }
    printf("\n");
    free(num);
}

void dfs_matrix_non_rec(struct Graph* g, int start_v, int* num) {
    int* stack = (int*)malloc(g->n * sizeof(int));
    int top = -1;

    num[start_v] = 1;
    printf("%d ", start_v + 1);
    stack[++top] = start_v;

    while (top >= 0) {
        int v = stack[top];
        int found = 0;
        for (int i = 0; i < g->n; i++) {
            if (g->adj[v][i] == 1 && !num[i]) {
                num[i] = 1;
                printf("%d ", i + 1);
                stack[++top] = i;
                found = 1;
                break;
            }
        }
        if (!found) {
            top--;
        }
    }
    free(stack);
}

void pog_matrix_non_rec(struct Graph* g, int start_v) {
    int* num = (int*)calloc(g->n, sizeof(int));
    printf("Обход (матрица, стек): ");
    if (start_v >= 0 && start_v < g->n) {
        dfs_matrix_non_rec(g, start_v, num);
    }
    for (int v = 0; v < g->n; v++) {
        if (!num[v]) {
            dfs_matrix_non_rec(g, v, num);
        }
    }
    printf("\n");
    free(num);
}

int main() {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, ".UTF8");
    srand((unsigned int)time(NULL));

    int n = 6;
    struct Graph* g = random_graph(n, 40);
    struct AdjList* l = matrix_to_list(g);

    int choice;
    while (1) {
        printf("\nМеню:\n");
        printf("1. Задание 1 (матрица, рекурсивно)\n");
        printf("2. Задание 1 (3*) (списки смежности)\n");
        printf("3. Задание 2* (матрица, стек)\n");
        printf("4. Выполнить все задания\n");
        printf("5. Новый случайный граф (N и вероятность 1-100%%)\n");
        printf("6. Тестовый граф из методички\n");
        printf("0. Выход\n");
        printf("Выбор: ");

        if (scanf("%d", &choice) != 1 || choice == 0) break;

        int v = 1;
        if (choice >= 1 && choice <= 4) {
            printf("Стартовая вершина (1-%d): ", g->n);
            if (scanf("%d", &v) != 1 || v < 1 || v > g->n) v = 1;
        }

        if (choice == 1) {
            print_adj(g);
            pog_matrix_rec(g, v - 1);
        }
        else if (choice == 2) {
            print_list(l);
            pog_list_rec(l, v - 1);
        }
        else if (choice == 3) {
            print_adj(g);
            pog_matrix_non_rec(g, v - 1);
        }
        else if (choice == 4) {
            print_adj(g);
            print_list(l);
            pog_matrix_rec(g, v - 1);
            pog_list_rec(l, v - 1);
            pog_matrix_non_rec(g, v - 1);
        }
        else if (choice == 5) {
            int prob = 40;
            printf("Количество вершин: ");
            if (scanf("%d", &n) != 1 || n < 1) n = 6;
            printf("Вероятность заполнения (1-100%%): ");
            if (scanf("%d", &prob) != 1) prob = 40;
            if (prob < 1) prob = 1;
            if (prob > 100) prob = 100;
            free_graph(g);
            free_adj_list(l);
            g = random_graph(n, prob);
            l = matrix_to_list(g);
            print_adj(g);
        }
        else if (choice == 6) {
            free_graph(g);
            free_adj_list(l);
            g = create_graph(6);
            g->adj[0][1] = g->adj[1][0] = 1;
            g->adj[0][2] = g->adj[2][0] = 1;
            g->adj[0][5] = g->adj[5][0] = 1;
            g->adj[1][3] = g->adj[3][1] = 1;
            g->adj[2][4] = g->adj[4][2] = 1;
            l = matrix_to_list(g);
            print_adj(g);
            print_list(l);
            pog_matrix_rec(g, 0);
            pog_list_rec(l, 0);
            pog_matrix_non_rec(g, 0);
        }
    }

    free_graph(g);
    free_adj_list(l);
    return 0;
}
