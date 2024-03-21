#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct Node {
	int vertex;
	int dist;	//distance
	int prev;
}Node;

typedef struct Graph {
	int size;
	int** vertices; //가중치
	Node* nodes;
}Graph;

typedef struct Heap {
	int Capacity;
	int Size;
	Node* Element;
}Heap;

Graph* createGraph(int size);
void deleteGraph(Graph* g);
void dijkstra(Graph* g);
int* shortestPath(Graph* g, int dest);
Heap* createMinHeap(int heapSize);
void deleteMinHeap(Heap* minHeap);
void insertToMinHeap(Heap* minHeap, int vertex, int distance);
Node deleteMin(Heap* minHeap);

void swap(Node* node1, Node* node2) {
	Node temp = *node1;
	*node1 = *node2;
	*node2 = temp;
}

void main(int argc, char *argv[]) {
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	Graph* g;
	int size;
	fscanf(fin, "%d\n", &size);
	g = createGraph(size + 1);

	char tmp = 0;
	while (tmp != '\n' && tmp != EOF) {
		int node1, node2, weight;
		fscanf(fin, "%d-%d-%d", &node1, &node2, &weight);
		g->vertices[node1][node2] = weight;
		tmp = fgetc(fin);
	}

	dijkstra(g);

	int j;
	for(int i = 2; i < g->size; i++){
		int* path = shortestPath(g, i);
		if(path == NULL){
			fprintf(fout, "can not reach to node %d\n", i);
			continue;
		}
		for (j = g->size - 1; j > 0; j--){
			if(path[j] == 0) continue;
			fprintf(fout, "%d->", path[j]);
		} 
		fprintf(fout, "%d (cost : %d)\n", i, g->nodes[i].dist);
		free(path);
	}
	deleteGraph(g);
}
/*
거리는 첫번째노드빼고 다 무한
prev는 다 -1
main에서 이 함수로 매개변수 넘길때 size+1 해주던데
보통 반복문에서 범위를 <로 하니까 1부터 size인거 표현하기 편하라고 그렇게 한거라 생각하고
그냥 매개변수로 받은 사이즈 그대로 저장했음
*/
Graph* createGraph(int size) {
    Graph* ret = (Graph*)malloc(sizeof(Graph));
    ret->size=size;
    ret->vertices = (int**)malloc(sizeof(int*)*size);
    for (int i=0; i<size; i++){
        ret->vertices[i] = (int*)malloc(sizeof(int)*size);
        for (int j=1; j<size; j++) ret->vertices[i][j]=INF;
    }
    ret->nodes = (Node*)malloc(sizeof(Node)*size);
    for (int i=0; i<size; i++){
        ret->nodes[i].vertex=i;
        ret->nodes[i].dist=INF;
        ret->nodes[i].prev=-1;
    }
    ret->nodes[1].dist=0;
    return ret;
}

void deleteGraph(Graph* g){
    free(g->nodes);
    for (int i=0; i<g->size; i++){
        free(g->vertices[i]);
    }
    free(g->vertices);
    free (g);
}
/*
우선순위큐 표현하기위한 minheap
대부분 9장 heap에 있는 피피티를 적극 참고했습니다
*/ 
Heap* createMinHeap(int heapSize) {
    Heap* ret = (Heap*)malloc(sizeof(Heap));
    ret->Capacity=heapSize;
    ret->Size=0;
    ret->Element=(Node*)malloc(sizeof(Node)*(heapSize+1)); //1부터 시작하니까 공간은 1더필요
    ret->Element[0].dist=0; //0에 최솟값
    return ret;
}

void deleteMinHeap(Heap* minHeap){
    free(minHeap->Element);
    free(minHeap);
}
/*
다익스트라
우선순위 큐 만들고 시작점인 1 넣기
이후로 while문 사용하여 우선순위큐에 남은게 없을때까지 돌기
인접한 노드의 거리를 업데이트 할 수 있으면 업데이트 해주고 우선순위 큐에 넣기
prev 업데이트
*/
void dijkstra(Graph* g){
    Heap* pq = createMinHeap(g->size);
    insertToMinHeap(pq,1,0);
    while (pq->Size){
        Node min=deleteMin(pq);
        for (int i=1; i<g->size; i++){
            if (i==min.vertex) continue;
            if (min.dist+g->vertices[min.vertex][i]<g->nodes[i].dist){
                g->nodes[i].dist=min.dist+g->vertices[min.vertex][i];
                insertToMinHeap(pq,i,g->nodes[i].dist);
                g->nodes[i].prev=min.vertex;
            }
        }
    }
}
/*
저장한 prev 따라서 차례로 저장
중간에 prev -1 나오면 NULL 리턴하고 끝
끝까지 저장하면 경로 저장된 path 리턴
*/
int* shortestPath(Graph* g, int dest){
    int *path= (int*)malloc(sizeof(int)*g->size);
    for (int i=0; i<g->size; i++) path[i]=0;
    int i=0;
    path[i]=dest;
    while (dest!=1){
        i++;
        if (g->nodes[dest].prev==-1) return NULL;
        dest=g->nodes[dest].prev;
        path[i]=dest;
    }
    return path;
}
/*
노드 minheap에 넣기
minheap에서는 prev 필요없어보여서 안썼음
dist 기준으로 minheap
*/
void insertToMinHeap(Heap* minHeap, int vertex, int distance) {
    Node push;
    push.dist=distance;
    push.vertex=vertex;
    int i=++minHeap->Size;
    for (i; minHeap->Element[i/2].dist>distance; i/=2){
        swap(&minHeap->Element[i],&minHeap->Element[i/2]);
    }
    minHeap->Element[i]=push;
}
/*
minheap에서 최소를 삭제하는 동시에 리턴
dist가 가장 작은 node 반환하게됨
*/
Node deleteMin(Heap *minHeap) {
    Node min_element = minHeap->Element[1];
    Node last_element = minHeap->Element[minHeap->Size--];
    int i, child;
    for (i=1; i*2<=minHeap->Size; i=child){
        child=i*2;
        if (child < minHeap->Size && minHeap->Element[child+1].dist < minHeap->Element[child].dist) child++;
        if (last_element.dist > minHeap->Element[child].dist) minHeap->Element[i] = minHeap->Element[child];
        else break;
    }
    minHeap->Element[i] = last_element;
    return min_element;
}

