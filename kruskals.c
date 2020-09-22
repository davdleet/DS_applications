#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MALLOC(p, s) p = malloc(s)
#define MAX_ELEMENTS 200
#define MAX_S 100
#define MAX_C 100
#define readline fgets(buff, sizeof(buff), fp1)
#define BUFFSIZE 200

int n = 0;
int vertices, edges, count = 0;
typedef struct edge *edgePointer;
typedef struct graph *graphPointer;

//structure for edges
typedef struct edge
{
    int start;
    int dest;
    int weight;
} edge;

//array to store results
edge result[MAX_S];

//structure for graphs
typedef struct graph
{
    int vert;
    int edge;
    edge edgearr[MAX_S];
} graph;

//structure for subsets to be made from edges
typedef struct subset
{
    int parent;
    int rank;
} subset;

//global array to keep track of parents
subset parents[MAX_S];

//create a graph
graphPointer create(int V, int E, edge arr1[MAX_S])
{
    int i;
    graphPointer graph1;
    MALLOC(graph1, sizeof(graph1));
    graph1->vert = V;
    graph1->edge = E;
    for (i = 0; i < edges; i++)
        graph1->edgearr[i] = arr1[i];
    return graph1;
}

//swap function to be used in bubble sort
void swap(edge *xp, edge *yp)
{
    edge temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// Bubble sort function
void sort(edge arr[], int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - i - 1; j++)
        {
            if (arr[j].weight > arr[j + 1].weight)
                swap(&arr[j], &arr[j + 1]);
        }
    }
}

//get the first line
void getFirst(char *buff)
{
    int i = 0;
    char *str1 = strtok(buff, " ");
    char *sArr[20] = {
        NULL,
    };
    while (str1 != NULL)
    {
        sArr[i] = str1;
        i++;
        str1 = strtok(NULL, " ");
    }
    vertices = atoi(sArr[0]);
    edges = atoi(sArr[1]);
}

//fill array from reading lines
void fill(char *buff, int arr1[][3], int strcount)
{
    int i = 0;
    char *str1 = strtok(buff, " ");
    while (str1 != NULL)
    {
        arr1[strcount][i] = atoi(str1);
        i++;
        str1 = strtok(NULL, " ");
    }
}

//initialize parent global array
void parent_init(int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        parents[i].parent = i;
        parents[i].rank = 0;
    }
}

//find function for the kruskal algorithm
//slight change from text book implementation
int simpleFind(int i)
{
    for (; parents[i].parent != i; i = parents[i].parent)
        ;
    return i;
}

//union function for the kruskal algorithm
void rankedUnion(int i, int j)
{
    int root_i = simpleFind(i);
    int root_j = simpleFind(j);
    int temp_rank = parents[i].rank + parents[j].rank;
    if (parents[i].rank < parents[j].rank)
    {
        parents[i].parent = j;
        parents[j].rank = temp_rank;
    }
    else if (parents[i].rank > parents[j].rank)
    {
        parents[j].parent = i;
        parents[i].rank = temp_rank;
    }
    else
    {
        parents[i].parent = j;
        parents[j].rank++;
    }
}

//kruskal algorithm
void k_algo(graph *graph1)
{
    int x, y;
    int i = 0, j = 0;
    int V = vertices;
    while (count < V - 1 && i < graph1->edge)
    {
        edge next = graph1->edgearr[i++];
        x = simpleFind(next.start);
        y = simpleFind(next.dest);
        if (x != y)
        {
            result[count++] = next;
            rankedUnion(x, y);
        }
    }
}

//driver function
int main(int argc, char *argv[])
{
    char *inputname = argv[1];
    char *outputname = argv[2];
    int i = 0, j, n;
    int temparr[MAX_S][3];
    char buff[BUFFSIZE];
    edge tempedge[MAX_S];
    edgePointer tempedgep;
    FILE *fp1;
    fp1 = fopen(inputname, "rt");
    if (argc > 2)
    {
        readline;
        getFirst(buff);
        while (readline)
        {
            fill(buff, temparr, i);
            i++;
        }
    }
    fclose(fp1);
    if (argc < 2)
    {
        printf("Not enough arguments");
        return -1;
    }
    for (i = 0; i < edges; i++)
    {
        tempedge[i].start = temparr[i][0];
        tempedge[i].dest = temparr[i][1];
        tempedge[i].weight = temparr[i][2];
    }
    n = edges;
    sort(tempedge, n);
    graphPointer maingraph = create(vertices, edges, tempedge);
    parent_init(vertices);
    k_algo(maingraph);
    FILE *fp2;
    if (argc > 2)
    {
        fp2 = fopen(argv[2], "wt");
        k_algo(maingraph);
        for (i = 0; i < count; i++)
        {
            fprintf(fp2, "%d %d\n", result[i].start, result[i].dest);
        }
    }
    fclose(fp2);
}