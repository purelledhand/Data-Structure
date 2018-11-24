#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int allEdges;
int allVertexs;
int *numberOfMST;
*numberOfMST=0;

typedef struct edge {
    int src;
    int dst;
    int cost;
} edge;

void insertMinEdgeHeap(edge edgeMinHeap[], edge newEdge, int *numberOfEdges) {
    int i = ++(*numberOfEdges);

    while((i != 1) && (newEdge.cost < edgeMinHeap[i/2].cost)) {
        edgeMinHeap[i] = edgeMinHeap[i/2];
        i /= 2;
    }

    edgeMinHeap[i] = newEdge;
    //printf("heap[%d] = %d\n", i, newEdge.cost);
}

edge deleteMinEdgeHeap(edge edgeMinHeap[], int *numberOfEdges) {
    int parent, child;
    edge item, temp;
    if(!(*numberOfEdges)) edgeMinHeap[0];
    item = edgeMinHeap[1];
    temp = edgeMinHeap[(*numberOfEdges)--];
    parent=1; child=2;

    while( child<=(*numberOfEdges)) {
        if((child<(*numberOfEdges)) && (edgeMinHeap[child].cost > edgeMinHeap[child+1].cost)) child++;
        if(temp.cost <= edgeMinHeap[child].cost) break;
        edgeMinHeap[parent] = edgeMinHeap[child];
        parent = child;
        child *= 2;
    }

    edgeMinHeap[parent] = temp;
    return item;
}

int find(int i, int connectedSet[]) {
    int parent=100;

    if(connectedSet[i]<0) {
        return i;
    } else {
        parent = connectedSet[i];
        while(connectedSet[parent]>0) {
            parent = connectedSet[parent];
        }
        return parent;
    }
}

int doesItMakeCycle(edge minCostEdge, int connectedSet[], edge MST[]) {

    //printf("[*] doesItMakeCycle\n");
    int src = minCostEdge.src; int dst = minCostEdge.dst;
    int find_src=100;
    int find_dst=100;
    int parent;

    if(connectedSet[src]<0) {
        find_src=src;
    } else {
        parent = connectedSet[src];
        while(connectedSet[parent]>0) {
            parent = connectedSet[parent];
        }
        find_src=parent;
    }

    if(connectedSet[dst]<0) {
        find_dst=dst;
    } else {
        parent = connectedSet[dst];
        while(connectedSet[parent]>0) {
            parent = connectedSet[parent];
        }
        find_dst=parent;
    }

    //printf("find(%d)=%d, find(%d)=%d\n", src, find_src, dst, find_dst);
    if(find_src == find_dst) {
        return 1;
    }
    return 0;

}

void addEdge(edge selectedEdge, edge MST[], int connectedSet[]) {
    int i = find(selectedEdge.src, connectedSet);
    int j = find(selectedEdge.dst, connectedSet);
    //printf("[before] set(%d)=%d, set(%d)=%d\n", i, connectedSet[i], j, connectedSet[j]);

    if(connectedSet[i] == connectedSet[j]) {
        connectedSet[i] += connectedSet[j];
        connectedSet[j] = i;
    } else if(connectedSet[i] < connectedSet[j]) {
        connectedSet[i] += connectedSet[j];
        connectedSet[j] = i;
    } else {
        connectedSet[j] += connectedSet[j];
        connectedSet[i] = j;
    }
    //printf("[after] set(%d)=%d, set(%d)=%d\n", i, connectedSet[i], j, connectedSet[j]);
    MST[(*numberOfMST)++] = selectedEdge;
}

int main(){
    int *numberOfEdges=0;
    int line=0;
    int allcost=0;
    edge tmp;
    edge edgeMinHeap[100];
    int connectedSet[100];
    edge MST[100];

    FILE *fp;
    char *out;
    numberOfEdges = (int *)calloc(10, sizeof(int));
    numberOfMST = (int *)calloc(100, sizeof(int));
    out = (char *)calloc(500, sizeof(char));
    fp = fopen("sample.txt","r");

    while(!feof(fp)) {
        fgets(out, 400, fp);
        if(line==0) {
            allVertexs = atoi(out);
            line++;
        }
        else {
            if(!strcmp(out,".\n")) break;
            tmp.src = atoi(strtok(out," "));
            tmp.dst = atoi(strtok(NULL," "));
            tmp.cost = atoi(strtok(NULL," "));
            insertMinEdgeHeap(edgeMinHeap, tmp, numberOfEdges);
        }
    }

    memset(connectedSet, -1, sizeof(connectedSet));

    while(*numberOfMST < (allVertexs-1)) {
        if(doesItMakeCycle(edgeMinHeap[1], connectedSet, MST)) {
            deleteMinEdgeHeap(edgeMinHeap, numberOfEdges);
            continue;
        }

        addEdge(edgeMinHeap[1], MST, connectedSet);
        //printf("(%d,%d) ", edgeMinHeap[1].src, edgeMinHeap[1].dst);
        allcost += edgeMinHeap[1].cost;
        deleteMinEdgeHeap(edgeMinHeap, numberOfEdges);
    }

    printf("Minimum cost spanning tree edges: ");
    for(int i=0;i<(*numberOfMST);i++) {
        printf("(%d,%d) ", MST[i].src, MST[i].dst);
    }
    printf("\n총비용 : %d\n", allcost);

    fclose(fp);
    free(out);
    free(numberOfMST);
    free(numberOfEdges);

    return 0;
}