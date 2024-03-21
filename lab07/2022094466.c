#include<stdio.h>
#include<stdlib.h>
#include<time.h>

FILE *fin;
FILE *fout;

typedef struct _DisjointSet {
	int size_maze;
	int *ptr_arr;
}DisjointSets;

void init(DisjointSets *sets, DisjointSets *maze_print, int num);
void Union(DisjointSets *sets, int i, int j);
int find(DisjointSets *sets, int i);
void createMaze(DisjointSets *sets, DisjointSets *maze_print, int num);
void printMaze(DisjointSets *sets, int num);
void freeMaze(DisjointSets *sets, DisjointSets *maze_print);


int main(int argc, char* agrv[]) {
	srand((unsigned int)time(NULL));

	int num, i;
	fin = fopen(agrv[1], "r");
	fout = fopen(agrv[2], "w");

	DisjointSets *sets, *maze_print;
	fscanf(fin, "%d", &num);

	sets = (DisjointSets*)malloc(sizeof(DisjointSets));
	maze_print = (DisjointSets*)malloc(sizeof(DisjointSets));

	init(sets, maze_print, num);
	createMaze(sets, maze_print, num);
	printMaze(maze_print, num);

	freeMaze(sets, maze_print);

	fclose(fin);
	fclose(fout);

	return 0;
}
/*
disjoint set의 초기값은 자기자신으로
maze_print는 다 1
*/
void init(DisjointSets *sets, DisjointSets *maze_print, int num) {
	sets->ptr_arr=(int*)malloc(sizeof(int)*(num*num));
	for (int i=0; i<num*num; i++) sets->ptr_arr[i]=i;
	maze_print->ptr_arr=(int*)malloc(sizeof(int)*(num*num*2));
	for (int i=0; i<num*num*2; i++) maze_print->ptr_arr[i]=1;
}

/*
작은값을 부모로
*/
void Union(DisjointSets *sets, int i, int j) {
	int a=find(sets,i);
	int b=find(sets,j);
	if (a<b) {
		sets->ptr_arr[b]=a;
	}
	else {
		sets->ptr_arr[a]=b;
	}
}
/*
부모를 return
동시에 부모가 갱신됐으면 반영해주기  
*/
int find(DisjointSets *sets, int i) {
	if (sets->ptr_arr[i]==i) return i;
	return sets->ptr_arr[i]=find(sets,sets->ptr_arr[i]);
}
/*
모두가 부모를 0으로 가질때 반복문 끝내기
find를 부모갱신형태로 바꾸서 반복문 돌다보면 결국 다 부모 0으로 가질때가 옴
*/
void createMaze(DisjointSets *sets, DisjointSets *maze_print, int num) {
	maze_print->ptr_arr[num*num*2-2]=0;//출구
	while (1){
		int flag=1;
		for (int i=0; i<num*num; i++) if (sets->ptr_arr[i]!=0) flag=0;
		if (flag) break;
		int r=rand();
		int a=(int)r%(num*num*2);
		if (a%(num*2)==(num-1)*2) continue; //가장오른쪽벽
		if (a/(num*2)==num-1&&a%2) continue; //가장아래쪽벽
		if (a%2==0){ //오른쪽벽
			if (find(sets,a/2)==find(sets,a/2+1)) continue; //이미 같은부모이진 않은지 확인 (사이클방지)
			Union(sets,a/2,a/2+1);
			maze_print->ptr_arr[a]=0; //벽제거
		}
		else { //아래쪽벽
			if (find(sets,a/2)==find(sets,a/2+num)) continue; //사이클 방지
			Union(sets,a/2,a/2+num);
			maze_print->ptr_arr[a]=0; //벽제거
		}
	}	
}

/*
가장위쪽벽과 가장왼쪽벽은 고정이기에 그냥출력
입구부분만 뚫려있는 것 유의
나머지는 maze_print에 저장된 값 활용
*/
void printMaze(DisjointSets *sets, int num) {
	fprintf(fout, "+");
	for (int i=0; i<num; i++) fprintf(fout, "---+");
	for (int i=0; i<num; i++){
		if (i==0) fprintf(fout, "\n "); //입구
		else fprintf(fout, "\n|");
		for (int j=0; j<num; j++){
			if(sets->ptr_arr[num*i*2+j*2]) fprintf(fout, "   |"); //오른쪽벽
			else fprintf(fout, "    "); //벽없음

		}
		fprintf(fout, "\n+");
		for (int j=0; j<num; j++){
			if (sets->ptr_arr[num*i*2+j*2+1]) fprintf(fout, "---+"); //아래쪽벽
			else fprintf(fout, "   +"); //벽없음
		}
	}
}
/*
할당된메모리 free
*/
void freeMaze(DisjointSets *sets, DisjointSets * maze_print) {
	free(sets->ptr_arr);
	free(maze_print->ptr_arr);
	free(sets);
	free(maze_print);
}
