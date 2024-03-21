#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FILE *fin;
FILE *fout;

typedef struct _Queue {
	int* key;
	int first, rear, qsize, max_queue_size;
}Queue;

typedef struct _Graph {
	int size;
	int* node;
	int** matrix;
}Graph;

void countInput(int *n, char *numbers);
void parseInput(int *nodes, char *numbers, int n);
Graph* CreateGraph(int* nodes, int n);
void InsertEdge(Graph* G, int a, int b);
void DeleteGraph(Graph* G);
int* Topsort(Graph* G);
Queue* MakeNewQueue(int X);
int IsEmpty(Queue* Q);
int Dequeue(Queue* Q);
void Enqueue(Queue* Q, int X);
void DeleteQueue(Queue* Q);

int main(int argc, char* agrv[]) {
	fin = fopen(agrv[1], "r");
	fout = fopen(agrv[2], "w");

	char numbers[100], inputs[150];
	fgets(numbers, 100, fin);
	int n = 0;
	countInput(&n, numbers);
	int* nodes = (int*)malloc(sizeof(int)*n);
	int* sorted_arr = NULL;
	parseInput(nodes, numbers, n);

	Graph* G = CreateGraph(nodes, n);

	fgets(inputs, 100, fin);
	int len = strlen(inputs), i, j;
	for (i = 0; i < len; i += 4) {
		int a = inputs[i] - '0', b = inputs[i + 2] - '0';
		InsertEdge(G, a, b);
	}

	// PrintGraph(G);
	fprintf(fout, "%-3s", " ");
	for (i = 0; i < G->size; i++) fprintf(fout, "%-3d", G->node[i]);
	fprintf(fout, "\n");
	for (i = 0; i < G->size; i++) {
		fprintf(fout, "%-3d", G->node[i]);
		for (j = 0; j < G->size; j++) fprintf(fout, "%-3d", G->matrix[i][j]);
		fprintf(fout, "\n");
	}
	fprintf(fout, "\n");

	sorted_arr = Topsort(G);
	if(sorted_arr == NULL){
		fprintf(fout, "sorting error : cycle!");
	}else{
		for(i = 0; i < G->size; i ++)
			fprintf(fout, "%d ", sorted_arr[i]);
		fprintf(fout, "\n");
	}
    free(nodes);
	free(sorted_arr);
	DeleteGraph(G);

	fclose(fin);
	fclose(fout);

	return 0;
}

//n 세주기
void countInput(int *n, char *numbers){
    int idx=0, cnt=0;
    while(cnt<100&&numbers[idx]-'0'>=0&&numbers[idx]-'0'<=9) {
        cnt++;
        idx+=2;
    }
    *n=cnt;
}

//인풋 오름차순으로
void parseInput(int *nodes, char *numbers, int n){
    int num[10]={0,};
    for (int i=0; i<n*2; i+=2){
        num[numbers[i]-'0']=1;
    }
    int j=0;
    for (int i=0; i<10; i++){
        if (num[i]) {
            nodes[j]=i;
            j++;
        }
    }
}

/*
그래프 생성
 */
Graph* CreateGraph(int* nodes, int n) {
    Graph* ret=(Graph*)malloc(sizeof(Graph));
    ret->size=n;
    ret->node=(int*)malloc(sizeof(int)*n);
    for (int i=0; i<n; i++){
        ret->node[i]=nodes[i];
    }
    ret->matrix=(int**)malloc(sizeof(int*)*n);
    for (int i=0; i<n; i++){
        ret->matrix[i]=(int*)malloc(sizeof(int)*n);
    }
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++) ret->matrix[i][j]=0; //초기화
    }
    return ret;
}
/*
G의 node에서 a, b를 찾아 연결표시
*/
void InsertEdge(Graph* G, int a, int b) {
    int ida=0, idb=0;
    for (int i=0; i<G->size; i++){
        if (G->node[i]==a) ida=i;
        else if (G->node[i]==b) idb=i;
    }
    G->matrix[ida][idb]=1;
}
/*
G 할당 해제
*/
void DeleteGraph(Graph* G){
    free(G->node);
    for (int i=0; i<G->size; i++) free(G->matrix[i]);
    free(G->matrix);
    free(G);
}

/*
Topological sort
*/
int* Topsort(Graph* G){
    Queue* q= MakeNewQueue(G->size);
    int ind[10]={0,};
    int visit[10]={0,};
    int* retarr= (int*)malloc(sizeof(int)*G->size);
    for (int i=0; i<G->size; i++){
        for (int j=0; j<G->size; j++){
            if (G->matrix[j][i]) ind[i]++;
        }
    }
    for (int i=0; i<G->size; i++){
        if (ind[i]==0) Enqueue(q,i);
    }
    int idx=0;
    while(!IsEmpty(q)){
        int v=Dequeue(q);
        if (visit[v]) { //이미 방문한곳->cycle
            free(retarr);
            return NULL;
        }
        visit[v]=1;
        retarr[idx++]=G->node[v];
        for (int i=0; i<G->size; i++){
            if (G->matrix[v][i]){
                G->matrix[v][i]=0;
                if (--ind[i]==0) Enqueue(q,i);
            }
        }
    }
    if (idx<G->size){ //다 출력안됐는데 더이상 ind가 0인곳이 없음->clycle
        free(retarr);
        return NULL;
    }
    return retarr;
}

/*
큐 만들기
*/
Queue* MakeNewQueue(int X){
    Queue* q=(Queue*)malloc(sizeof(Queue));
    q->key=(int*)malloc(sizeof(int)*(X+1));
    q->first=0;
    q->rear=-1;
    q->qsize=0;
    q->max_queue_size=X+1;
    return q;
}

int IsEmpty(Queue* Q){
    if (Q->qsize==0) return 1;
    else return 0;
}

int Dequeue(Queue* Q){
    Q->qsize--;
    return Q->key[Q->first++];
}

void Enqueue(Queue* Q, int X){
    Q->qsize++;
    Q->key[++Q->rear]=X;
}

void DeleteQueue(Queue* Q){
    free(Q->key);
    free(Q);
}