#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
int city_num;

typedef struct city {
    char name[20];
} city;

city cities[100];
int distance[100];
int PI[100];

int choose(int distance[], int n, int found[]) {
    int i, min, minpos;
    min = INT_MAX;
    minpos = -1;
    for(i=0;i<n;i++) {
        if(distance[i]<min && found[i]!=1) {
            min = distance[i];
            minpos=i;
        }
    }
    return minpos;
}

void shortestPath(int v, int cost[][city_num], int distance[], int n, int found[]) {
    int i, u, w;

    for(i=0;i<n;i++) {
        distance[i] = cost[v][i];
    }

    found[v] = 1;
    distance[v] = 0;

    for(i=0; i<n-1; i++) {
        u = choose(distance, n, found);
        found[u] = 1;
        for(w=0;w<n;w++) {
            if(found[w]!=1) {
                if(distance[u]+cost[u][w] < distance[w]) {
                    distance[w] = distance[u]+cost[u][w];
                    PI[w] = u;
                }
            }
        }
    }
}

void printShortestPath(int source, int dest) { // 3 0
    if(PI[dest] == 0)
    {
        printf("%s " , cities[source].name);
        printf("%s ", cities[dest].name);
        return;
    }
    if(dest == source)
    {
        printf("%s ", cities[dest].name);
        return;
    }

    while(dest != source)
    {
        printShortestPath(source, PI[dest]);
        printf("%s ", cities[dest].name);
        return;
    }
}

int setCost(char *input, int inputcost, int cost[][city_num]) {
    char *city1 = (char *)calloc(20, sizeof(char));
    char *city2 = (char *)calloc(20, sizeof(char));
    strcpy(city1, strtok(input, "-"));
    strcpy(city2, strtok(NULL, ""));
    int city1_index=-1;
    int city2_index=-1;

    for(int i=0;i<city_num;i++)
    {
        //printf("city1 : %s, city2 : %s, cities[%d].name : %s\n", city1, city2, i, cities[i].name);
        if(!strcmp(city1, cities[i].name)) city1_index = i;
        if(!strcmp(city2, cities[i].name)) city2_index = i;
    }

    if(city1_index == -1 || city2_index == -1) {
        return 1;
    }

    cost[city1_index][city2_index] = inputcost;
    cost[city2_index][city1_index] = inputcost;

    free(city1);
    free(city2);

    return 0;
}


int main() {
    char *input = (char *)calloc(100,sizeof(char));
    int inputcost;

    //init city structure
    printf("[그래프 입력]\n");
    printf("Insert City Count : "); scanf("%d",&city_num);
    printf("Insert City Names : ");
    for(int i=0;i<city_num;i++)
        scanf("%s", cities[i].name);

    int cost[city_num][city_num];
    short int found[city_num];

    memset(found,0,sizeof(found));
    found[city_num] = 0;

    //init cost
    for(int i=0;i<city_num;i++) {
        for(int j=0;j<city_num;j++) {
            cost[i][j] = 10000;
        }
    }
    printf("\n[도시간 연결 입력, 종료하려면 \".\" 입력]\n");

    while(1)
    {
        scanf("%s", input);
        //printf("%s %d", input, inputcost);
        if(!strcmp(input,".")) break;
        scanf("%d", &inputcost);
        if(setCost(input, inputcost, cost))
        {
            printf("잘못된 도시 이름을 입력하였습니다.\n");
        }
    }

    printf("\n최단 거리 기준 도시 입력 : ");
    scanf("%s", input);

    for(int i=0;i<city_num;i++) {
        if(!strcmp(input, cities[i].name)) {
            shortestPath(i, cost, distance, city_num, found);
            break;
        }
    }

    int src=-1;
    int dst=-1;

    for(int k=0;k<city_num;k++) {
        if(!strcmp(input, cities[k].name)) {
            src = k;
            break;
        }
    }

    for(int i=0;i<city_num;i++) {
        if(!strcmp(input, cities[i].name)) continue;
        printf("%s : ", cities[i].name);

        for(int k=0;k<city_num;k++) {
            if(!strcmp(cities[i].name, cities[k].name)) {
                dst = k;
                break;
            }
        }
        printf("%d ", distance[dst]-distance[src]);
        printShortestPath(src, dst);
        printf("\n");
    }

    return 0;
}