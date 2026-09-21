#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Structure for an Edge node (Adjacency List item)
struct EdgeNode {
    int destVertex;
    struct EdgeNode* nextEdge;
};

// Structure for a Vertex node
struct VertexNode {
    int vertexId;
    struct EdgeNode* edgeHead;     // Pointer to the list of edges leaving this vertex
    struct VertexNode* nextVertex; // Pointer to the next vertex in the graph
};

// Head of the graph's vertex list
struct VertexNode* graphHead = NULL;

// --- 1. Create a graph with a given number of vertices ---
void createGraph(int vertices) {
    // Clear any existing graph first
    struct VertexNode* curr = graphHead;
    while (curr != NULL) {
        struct EdgeNode* edge = curr->edgeHead;
        while (edge != NULL) {
            struct EdgeNode* tempEdge = edge;
            edge = edge->nextEdge;
            free(tempEdge);
        }
        struct VertexNode* tempVert = curr;
        curr = curr->nextVertex;
        free(tempVert);
    }
    graphHead = NULL;

    // Create new vertices sequentially (0 to vertices-1)
    struct VertexNode* tail = NULL;
    for (int i = 0; i < vertices; i++) {
        struct VertexNode* newVert = (struct VertexNode*)malloc(sizeof(struct VertexNode));
        newVert->vertexId = i;
        newVert->edgeHead = NULL;
        newVert->nextVertex = NULL;

        if (graphHead == NULL) {
            graphHead = newVert;
        } else {
            tail->nextVertex = newVert;
        }
        tail = newVert;
    }
    printf("Graph initialized with %d vertices (0 to %d) linked together.\n", vertices, vertices - 1);
}

// Helper to find a specific vertex node in the vertex linked list
struct VertexNode* findVertex(int id) {
    struct VertexNode* curr = graphHead;
    while (curr != NULL) {
        if (curr->vertexId == id) return curr;
        curr = curr->nextVertex;
    }
    return NULL;
}

// --- 5. Search whether a specific edge exists or not ---
bool edgeExists(int u, int v) {
    struct VertexNode* uNode = findVertex(u);
    if (uNode == NULL) return false;

    struct EdgeNode* edge = uNode->edgeHead;
    while (edge != NULL) {
        if (edge->destVertex == v) return true;
        edge = edge->nextEdge;
    }
    return false;
}

// --- 2. Insert an edge into the graph ---
void insertEdge(int u, int v) {
    struct VertexNode* uNode = findVertex(u);
    struct VertexNode* vNode = findVertex(v);

    if (uNode == NULL || vNode == NULL) {
        printf("Error: One or both vertices do not exist!\n");
        return;
    }

    if (edgeExists(u, v)) {
        printf("Edge (%d -> %d) already exists.\n", u, v);
        return;
    }

    // Allocate and insert a new edge at the front of u's edge list
    struct EdgeNode* newEdge = (struct EdgeNode*)malloc(sizeof(struct EdgeNode));
    newEdge->destVertex = v;
    newEdge->nextEdge = uNode->edgeHead;
    uNode->edgeHead = newEdge;

    printf("Edge (%d -> %d) inserted successfully.\n", u, v);
}

// --- 3. Delete an edge from the graph ---
void deleteEdge(int u, int v) {
    struct VertexNode* uNode = findVertex(u);
    if (uNode == NULL) {
        printf("Error: Source vertex %d not found!\n", u);
        return;
    }

    struct EdgeNode* curr = uNode->edgeHead;
    struct EdgeNode* prev = NULL;

    while (curr != NULL && curr->destVertex != v) {
        prev = curr;
        curr = curr->nextEdge;
    }

    if (curr == NULL) {
        printf("Edge (%d -> %d) does not exist.\n", u, v);
        return;
    }

    if (prev == NULL) {
        uNode->edgeHead = curr->nextEdge; // Removing head edge
    } else {
        prev->nextEdge = curr->nextEdge;  // Unlinking node
    }

    free(curr);
    printf("Edge (%d -> %d) deleted successfully.\n", u, v);
}

// --- 4. Display the complete graph using adjacency list ---
void displayGraph() {
    printf("\n--- Adjacency List Representation (Linked Nodes) ---\n");
    struct VertexNode* vert = graphHead;
    while (vert != NULL) {
        printf("Vertex %d:", vert->vertexId);
        struct EdgeNode* edge = vert->edgeHead;
        while (edge != NULL) {
            printf(" -> %d", edge->destVertex);
            edge = edge->nextEdge;
        }
        printf(" -> NULL\n");
        vert = vert->nextVertex;
    }
}

// --- 6. Find the in-degree and out-degree of a vertex ---
void findDegrees(int v) {
    if (findVertex(v) == NULL) {
        printf("Error: Vertex %d does not exist!\n", v);
        return;
    }

    int outDegree = 0;
    int inDegree = 0;

    struct VertexNode* vert = graphHead;
    while (vert != NULL) {
        // Calculate out-degree for target vertex
        if (vert->vertexId == v) {
            struct EdgeNode* edge = vert->edgeHead;
            while (edge != NULL) {
                outDegree++;
                edge = edge->nextEdge;
            }
        }
        // Scan all edge lists across the entire graph to count matches for in-degree
        struct EdgeNode* edge = vert->edgeHead;
        while (edge != NULL) {
            if (edge->destVertex == v) {
                inDegree++;
            }
            edge = edge->nextEdge;
        }
        vert = vert->nextVertex;
    }

    printf("Vertex %d -> In-degree: %d, Out-degree: %d\n", v, inDegree, outDegree);
}

// Helper utility to count total vertices currently in list
int getVertexCount() {
    int count = 0;
    struct VertexNode* curr = graphHead;
    while (curr != NULL) {
        count++;
        curr = curr->nextVertex;
    }
    return count;
}

// Dynamic helper array to see if an element has been visited in lists
bool isVisited(int* visitedList, int count, int id) {
    for (int i = 0; i < count; i++) {
        if (visitedList[i] == id) return true;
    }
    return false;
}

// --- 7. Check whether a path exists between two given vertices ---
void checkPath(int src, int dest) {
    if (findVertex(src) == NULL || findVertex(dest) == NULL) {
        printf("Error: Invalid source or destination vertex!\n");
        return;
    }
    if (src == dest) {
        printf("Path exists from %d to %d.\n", src, dest);
        return;
    }

    int totalV = getVertexCount();
    int* queue = (int*)malloc(totalV * sizeof(int));
    bool* visited = (bool*)calloc(totalV, sizeof(bool));
    int front = 0, rear = 0;

    visited[src] = true;
    queue[rear++] = src;

    bool found = false;
    while (front < rear) {
        int u = queue[front++];
        struct VertexNode* uNode = findVertex(u);

        if (uNode != NULL) {
            struct EdgeNode* edge = uNode->edgeHead;
            while (edge != NULL) {
                if (edge->destVertex == dest) {
                    found = true;
                    break;
                }
                if (!visited[edge->destVertex]) {
                    visited[edge->destVertex] = true;
                    queue[rear++] = edge->destVertex;
                }
                edge = edge->nextEdge;
            }
        }
        if (found) break;
    }

    if (found) printf("Path exists from %d to %d.\n", src, dest);
    else printf("No path exists from %d to %d.\n", src, dest);

    free(queue);
    free(visited);
}

// Recursive helper logic for linked list DFS
void DFSUtil(int v, bool* visited) {
    visited[v] = true;
    printf("%d ", v);

    struct VertexNode* vNode = findVertex(v);
    if (vNode != NULL) {
        struct EdgeNode* edge = vNode->edgeHead;
        while (edge != NULL) {
            if (!visited[edge->destVertex]) {
                DFSUtil(edge->destVertex, visited);
            }
            edge = edge->nextEdge;
        }
    }
}

// --- 8. Perform DFS starting from a given vertex ---
void performDFS(int startVertex) {
    if (findVertex(startVertex) == NULL) {
        printf("Error: Starting vertex %d not found!\n", startVertex);
        return;
    }
    int totalV = getVertexCount();
    bool* visited = (bool*)calloc(totalV, sizeof(bool));

    printf("DFS Traversal starting from vertex %d: ", startVertex);
    DFSUtil(startVertex, visited);
    printf("\n");

    free(visited);
}

// --- 9. Count the total number of vertices and edges ---
void countVerticesAndEdges() {
    int vertexCount = 0;
    int edgeCount = 0;

    struct VertexNode* vert = graphHead;
    while (vert != NULL) {
        vertexCount++;
        struct EdgeNode* edge = vert->edgeHead;
        while (edge != NULL) {
            edgeCount++;
            edge = edge->nextEdge;
        }
        vert = vert->nextVertex;
    }
    printf("Total Vertices: %d\n", vertexCount);
    printf("Total Edges: %d\n", edgeCount);
}

// --- 10. Find the vertex with the maximum degree ---
void maxDegreeVertex() {
    if (graphHead == NULL) {
        printf("Graph is empty.\n");
        return;
    }

    int totalV = getVertexCount();
    int* totalDegrees = (int*)calloc(totalV, sizeof(int));

    // Map through our vertex structures to calculate degrees
    struct VertexNode* vert = graphHead;
    while (vert != NULL) {
        struct EdgeNode* edge = vert->edgeHead;
        while (edge != NULL) {
            totalDegrees[vert->vertexId]++;           // Out-degree
            totalDegrees[edge->destVertex]++;         // In-degree
            edge = edge->nextEdge;
        }
        vert = vert->nextVertex;
    }

    int maxDeg = -1;
    int maxVertex = -1;

    for (int i = 0; i < totalV; i++) {
        if (totalDegrees[i] > maxDeg) {
            maxDeg = totalDegrees[i];
            maxVertex = i;
        }
    }

    printf("Vertex with maximum total degree is Vertex %d with a degree of %d.\n", maxVertex, maxDeg);
    free(totalDegrees);
}

// Free all memory on exit
void freeGraphMemory() {
    struct VertexNode* vert = graphHead;
    while (vert != NULL) {
        struct EdgeNode* edge = vert->edgeHead;
        while (edge != NULL) {
            struct EdgeNode* tempEdge = edge;
            edge = edge->nextEdge;
            free(tempEdge);
        }
        struct VertexNode* tempVert = vert;
        vert = vert->nextVertex;
        free(tempVert);
    }
    graphHead = NULL;
}

int main() {
    int vertices, choice;
    printf("Enter the total number of vertices to create the initial graph: ");
    if (scanf("%d", &vertices) != 1 || vertices <= 0) {
        printf("Invalid configuration value.\n");
        return 1;
    }

    createGraph(vertices);

    do {
        printf("\n================ MENU ================\n");
        printf("1. Insert an edge\n");
        printf("2. Delete an edge\n");
        printf("3. Display the graph (Adjacency List)\n");
        printf("4. Search if an edge exists\n");
        printf("5. Find In-degree and Out-degree of a vertex\n");
        printf("6. Check if a path exists between two vertices\n");
        printf("7. Perform DFS traversal\n");
        printf("8. Count total number of vertices and edges\n");
        printf("9. Find vertex with maximum degree\n");
        printf("10. Exit\n");
        printf("Enter your choice (1-10): ");
        scanf("%d", &choice);

        int u, v;
        switch (choice) {
            case 1:
                printf("Enter source (u) and destination (v): ");
                scanf("%d %d", &u, &v);
                insertEdge(u, v);
                break;
            case 2:
                printf("Enter source (u) and destination (v) to delete: ");
                scanf("%d %d", &u, &v);
                deleteEdge(u, v);
                break;
            case 3:
                displayGraph();
                break;
            case 4:
                printf("Enter source (u) and destination (v) to search: ");
                scanf("%d %d", &u, &v);
                if (edgeExists(u, v))
                    printf("Edge (%d -> %d) exists.\n", u, v);
                else
                    printf("Edge (%d -> %d) does not exist.\n", u, v);
                break;
            case 5:
                printf("Enter vertex: ");
                scanf("%d", &u);
                findDegrees(u);
                break;
            case 6:
                printf("Enter source and destination vertices: ");
                scanf("%d %d", &u, &v);
                checkPath(u, v);
                break;
            case 7:
                printf("Enter starting vertex for DFS: ");
                scanf("%d", &u);
                performDFS(u);
                break;
            case 8:
                countVerticesAndEdges();
                break;
            case 9:
                maxDegreeVertex();
                break;
            case 10:
                printf("Cleaning memory... Goodbye!\n");
                freeGraphMemory();
                break;
            default:
                printf("Invalid selection! Please enter a choice between 1 and 10.\n");
        }
    } while (choice != 10);

    return 0;
}
