#include <stdio.h>
#include <stdlib.h>

struct Edge {
    int v;
    struct Edge *next;
};
struct Vertex {
    int v;
    struct Edge *edge;
    struct Vertex *next;
};

struct Vertex *head = NULL;
/* Find vertex */
struct Vertex* find(int v) {
    struct Vertex *p = head;

    while (p != NULL) {
        if (p->v == v)
            return p;
        p = p->next;
    }
    return NULL;
}

/* Create graph */
void create(int n) {
    struct Vertex *p, *last = NULL;

    for (int i = 0; i < n; i++) {
        p = malloc(sizeof(struct Vertex));
        p->v = i;
        p->edge = NULL;
        p->next = NULL;

        if (head == NULL)
            head = p;
        else
            last->next = p;
        last = p;
    }
}

/* Search edge */
int searchEdge(int u, int v) {
    struct Vertex *p = find(u);
    struct Edge *e;

    if (p == NULL)
        return 0;

    e = p->edge;

    while (e != NULL) {
        if (e->v == v)
            return 1;
        e = e->next;
    }

    return 0;
}

/* Insert edge */
void insert(int u, int v) {
    struct Vertex *p = find(u);
    struct Edge *e;

    if (p == NULL || find(v) == NULL) {
        printf("Invalid vertex\n");
        return;
    }

    if (searchEdge(u, v)) {
        printf("Edge already exists\n");
        return;
    }

    e = malloc(sizeof(struct Edge));
    e->v = v;
    e->next = p->edge;
    p->edge = e;
}

/* Delete edge */
void deleteEdge(int u, int v) {
    struct Vertex *p = find(u);
    struct Edge *e, *prev = NULL;

    if (p == NULL)
        return;

    e = p->edge;

    while (e != NULL && e->v != v) {
        prev = e;
        e = e->next;
    }

    if (e == NULL) {
        printf("Edge not found\n");
        return;
    }

    if (prev == NULL)
        p->edge = e->next;
    else
        prev->next = e->next;

    free(e);
}

/* Display graph */
void display() {
    struct Vertex *p = head;
    struct Edge *e;

    while (p != NULL) {
        printf("%d -> ", p->v);

        e = p->edge;

        while (e != NULL) {
            printf("%d -> ", e->v);
            e = e->next;
        }

        printf("NULL\n");
        p = p->next;
    }
}

/* Degree */
void degree(int v) {
    struct Vertex *p = head;
    struct Edge *e;
    int in = 0, out = 0;

    if (find(v) == NULL) {
        printf("Invalid vertex\n");
        return;
    }

    while (p != NULL) {
        e = p->edge;
        while (e != NULL) {
            if (p->v == v)
                out++;
            if (e->v == v)
                in++;
            e = e->next;
        }
        p = p->next;
    }

    printf("In-degree = %d\n", in);
    printf("Out-degree = %d\n", out);
}

/* Count vertices and edges */
void count() {
    struct Vertex *p = head;
    struct Edge *e;
    int v = 0, edge = 0;
    while (p != NULL) {
        v++;
        e = p->edge;
        while (e != NULL) {
            edge++;
            e = e->next;
        }

        p = p->next;
    }

    printf("Vertices = %d\n", v);
    printf("Edges = %d\n", edge);
}

/* DFS */
void dfs(int v, int visited[]) {
    struct Vertex *p = find(v);
    struct Edge *e;

    visited[v] = 1;
    printf("%d ", v);
    e = p->edge;

    while (e != NULL) {
        if (!visited[e->v])
            dfs(e->v, visited);

        e = e->next;
    }
}

/* Path using DFS */
int path(int u, int v, int visited[]) {
    struct Vertex *p = find(u);
    struct Edge *e;

    if (u == v)
        return 1;
    visited[u] = 1;
    e = p->edge;
    while (e != NULL) {
        if (!visited[e->v] && path(e->v, v, visited))
            return 1;

        e = e->next;
    }

    return 0;
}

/* Maximum degree */
void maxDegree(int n) {
    int d[100] = {0};
    struct Vertex *p = head;
    struct Edge *e;
    int max = 0, vertex = 0;

    while (p != NULL) {
        e = p->edge;

        while (e != NULL) {
            d[p->v]++;
            d[e->v]++;
            e = e->next;
        }

        p = p->next;
    }

    for (int i = 0; i < n; i++) {
        if (d[i] > max) {
            max = d[i];
            vertex = i;
        }
    }

    printf("Maximum degree: Vertex %d = %d\n", vertex, max);
}

int main() {
    int n, choice, u, v;

    printf("Number of vertices: ");
    scanf("%d", &n);

    create(n);

    do {
        printf("\n1.Insert\n");
        printf("2.Delete\n");
        printf("3.Display\n");
        printf("4.Search Edge\n");
        printf("5.Degree\n");
        printf("6.Path\n");
        printf("7.DFS\n");
        printf("8.Count\n");
        printf("9.Maximum Degree\n");
        printf("10.Exit\n");

        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {

        case 1:
            scanf("%d %d", &u, &v);
            insert(u, v);
            break;
        case 2:
            scanf("%d %d", &u, &v);
            deleteEdge(u, v);
            break;
        case 3:
            display();
            break;
        case 4:
            scanf("%d %d", &u, &v);
            if (searchEdge(u, v))
                printf("Edge exists\n");
            else
                printf("Edge does not exist\n");
            break;
        case 5:
            scanf("%d", &v);
            degree(v);
            break;
        case 6: {
            int visited[100] = {0};
            scanf("%d %d", &u, &v);
            if (path(u, v, visited))
                printf("Path exists\n");
            else
                printf("No path\n");
            break;
        }
        case 7: {
            int visited[100] = {0};
            scanf("%d", &u);
            dfs(u, visited);
            printf("\n");
            break;
        }
        case 8:
            count();
            break;
        case 9:
            maxDegree(n);
            break;
        case 10:
            printf("Exit\n");
            break;
        default:
            printf("Invalid choice\n");
        }

    } while (choice != 10);

    return 0;
}
