#include <stdio.h>
#include <stdlib.h>
#define MAX 100
struct Node{
    int vertex;
    struct Node *next;
} *head[MAX];

int vertices;
int visited[MAX];

void createGraph(){
    printf("Enter number of vertices: ");
    scanf("%d", &vertices);

    for(int i = 0; i < vertices; i++) {
        head[i] = NULL;
    }
     printf("Graph created successfully.\n");
}

struct Node* createNode(int item){
    struct Node *curr = (struct Node*)malloc(sizeof(struct Node));
    curr->vertex = item;
    curr->next = NULL;
    return curr;
}

void insertEdge(){
    int source,destination ;
    struct Node *curr;

    printf("Enter source vertex: ");
    scanf("%d", &source);

    printf("Enter destination vertex: ");
    scanf("%d", &destination);

    if(source < 0 || source >= vertices || destination  < 0 || destination  >= vertices) {
        printf("Invalid vertex!\n");
        return;
    }
    curr = createNode(destination );
    curr->next = head[source];
    head[source] = curr;

    printf("Edge %d -> %d inserted.\n", source,destination );
}

void deleteEdge(){
    int source, destination;
    struct Node *temp, *prev;

    printf("Enter source vertex: ");
    scanf("%d", &source);
    printf("Enter destination vertex: ");
    scanf("%d", &destination);
    if(source < 0 || source >= vertices || destination  < 0 || destination  >= vertices) {
        printf("Invalid vertex!\n");
        return;
    }

    temp = head[source];
    prev = NULL;

    while(temp != NULL) {
        if(temp->vertex == destination ) {
            if(prev == NULL)
                head[source] = temp->next;
            else
                prev->next = temp->next;
            free(temp);
            printf("Edge deleted.\n");
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    printf("Edge not found.\n");
}

void displayGraph(){
    struct Node *temp;
    printf("\nAdjacency List:\n");

    for(int i = 0; i < vertices; i++) {
        printf("%d->", i);
        temp = head[i];
        while(temp != NULL) {
            printf("%d -> ", temp->vertex);
         temp= temp->next;
    }
    printf("NULL\n");
 }
}

void searchEdge(){
    int source, destination ;
    struct Node *temp;
    printf("Enter source vertex: ");
    scanf("%d", &source);
    printf("Enter destination vertex: ");
    scanf("%d", &destination );
    if(source < 0 || source >= vertices || destination  < 0 || destination  >= vertices) {
        printf("Invalid vertex!\n");
        return;
    }
    temp = head[source];

    while(temp != NULL) {
        if(temp->vertex == destination ) {
            printf("Edge %d -> %d exists.\n", source, destination );
            return;
        }
        temp = temp->next;
    }
    printf("Edge %d -> %d does not exist.\n", source, destination );
}

void degree(){
    int v, in = 0,out = 0;
    struct Node *temp;

    printf("Enter vertex: ");
    scanf("%d", &v);

    temp = head[v];
    while(temp != NULL){
        out++;
        temp = temp->next;
    }

    for(int i = 0; i < vertices; i++) {
        temp = head[i];
        while(temp != NULL)   {
            if(temp->vertex == v)
                in++;
            temp = temp->next;
        }
    }
    printf("In-degree = %d\n", in);
    printf("Out-degree = %d\n", out);
}

void dfs(int v){
    struct Node *temp;
    visited[v] = 1;
    printf("%d ", v);

    temp = head[v];
    while(temp != NULL){
        if(visited[temp->vertex] == 0) {
            dfs(temp->vertex);
        }
        temp = temp->next;
    }
}

void checkPath(){
    int start, end;
    printf("Enter starting vertex: ");
    scanf("%d", &start);
    printf("Enter ending vertex: ");
    scanf("%d", &end);
    for(int i = 0; i < vertices; i++)
        visited[i] = 0;

    dfs(start);

    if(visited[end] == 1){
        printf("Path exists.\n");
    }else{
        printf("Path does not exist.\n");
    }
}

void performDFS(){
    int start;
    printf("Enter starting vertex: ");
    scanf("%d", &start);
    for(int i = 0; i < vertices; i++){
        visited[i] = 0;
    }
    printf("DFS: ");
    dfs(start);
    printf("\n");
}

void countGraph(){
    int edges = 0;
    struct Node *temp;
    for(int i = 0; i < vertices; i++){
        temp = head[i];
        while(temp != NULL)  {
            edges++;
            temp = temp->next;
        }
    }
    printf("Total vertices = %d\n", vertices);
    printf("Total edges = %d\n", edges);
}

void maxDegree(){
    int max = -1,maxVertex = 0,in,out;
    struct Node *temp;
    for(int i = 0; i < vertices; i++) {
        in = 0;
        out = 0;
        temp = head[i];
        while(temp != NULL) {
            out++;
            temp = temp->next;
        }
        for(int j = 0; j < vertices; j++){
            temp = head[j];
            while(temp != NULL) {
                if(temp->vertex == i)
                    in++;
                temp = temp->next;
            }
        }
        if(in + out > max) {
            max = in + out;
            maxVertex = i;
        }
    }
    printf("Vertex = %d\n", maxVertex);
    printf("Maximum degree = %d\n", max);
}

int main(){
    int choice;
    while(1) {
        printf("\n1. Create Graph\n");
        printf("2. Insert Edge\n");
        printf("3. Delete Edge\n");
        printf("4. Display Graph\n");
        printf("5. Search Edge\n");
        printf("6. Find In-degree and Out-degree\n");
        printf("7. Check Path\n");
        printf("8. DFS\n");
        printf("9. Count Vertices and Edges\n");
        printf("10.Find Maximum Degree\n");
        printf("11. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                createGraph();
                break;
            case 2:
                insertEdge();
                break;
            case 3:
                deleteEdge();
                break;
            case 4:
                displayGraph();
                break;
            case 5:
                searchEdge();
                break;
            case 6:
                degree();
                break;
            case 7:
                checkPath();
                break;
            case 8:
                performDFS();
                break;
            case 9:
                countGraph();
                break;
            case 10:
                maxDegree();
                break;
            case 11:
                return 0;
            default:
                printf("Invalid\n");
        }
    }
    return 0;
}
