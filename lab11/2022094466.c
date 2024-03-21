#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FILE *fin;
FILE *fout;

typedef int ElementType;
typedef ElementType List;
typedef struct HashTbl* HashTable;

typedef struct HashTbl{
	int TableSize;
	List *TheLists;
}HashTbl;

int h1(int Key, HashTable H, int num, int solution);
HashTable createTable(int TableSize);
void Insert(HashTable H, ElementType Key, int solution);
void Delete(HashTable H, ElementType Key, int solution);
int Find(HashTable H, ElementType Key, int solution);
void printTable(HashTable H);
void deleteTable(HashTable H);

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");


	char solution_str[20];
	int solution, TableSize;

	fscanf(fin, "%s", solution_str);
	if(!strcmp(solution_str, "linear")) //명세에선 대문자이던데 일단 스켈레톤 따라가겠습니다
		solution = 1;
	else if(!strcmp(solution_str, "quadratic"))
		solution = 2;
	else{
		fprintf(fout, "Error: Invalid hashing solution!");
		return 0;
	}
	
	fscanf(fin, "%d", &TableSize);

	HashTable H = createTable(TableSize);

	char cv;
	int key;
	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d", &key);
				Insert(H, key, solution);
				break;

			case 'f':
				fscanf(fin, "%d", &key);
				int result = Find(H, key, solution);
				if(result)
					fprintf(fout, "%d is in the table\n", key);
				else
					fprintf(fout, "%d is not in the table\n", key);
				break;

			case 'd':
				fscanf(fin, "%d", &key);
				Delete(H, key, solution);
				break;

			case 'p':
				printTable(H);
				break;
		}
	}

	deleteTable(H);

	return 0;
}

//hi 구하는 식 매번쓰기가 힘들것같아 따로 만들었음
int h1(int Key, HashTable H, int num, int solution){
    if (solution==1) return (Key%H->TableSize+num)%H->TableSize;
    else return (Key%H->TableSize+num*num)%H->TableSize;
}

HashTable createTable(int TableSize){
    HashTable ret = (HashTable)malloc(sizeof(HashTbl));
    ret->TableSize=TableSize;
    ret->TheLists = (List*)malloc(sizeof(List)*TableSize);
    return ret;
}

/*
Insert
이미 있으면 오류출력
없으면 hi값 구해서 자리 있을때까지 반복문 돌다가 자리있으면 넣기
다 돌아도 없으면 다 찬거니까 오류출력
*/
void Insert(HashTable H, ElementType Key, int solution){
    int idx=Find(H,Key,solution);
    if (idx){
        fprintf(fout,"insertion error : %d already exists at address %d\n",Key,idx-1);
        return;
    }
    for (int i=0; i<H->TableSize; i++){
        int h=h1(Key,H,i,solution);
        if (!H->TheLists[h]){
            H->TheLists[h]=Key;
            fprintf(fout,"Insert %d into address %d\n",Key,h);
            return;
        }
    }
    fprintf(fout,"Insertion Error : table is full\n");
    return;
}
/*
Delete
그냥 한바퀴 돌고 있으면 지우기
*/
void Delete(HashTable H, ElementType Key, int solution){
    int idx=Find(H,Key,solution);
    if (idx==0){
        fprintf(fout,"Deletion Error : %d is not in the table\n",Key);
        return;
    }
    H->TheLists[idx-1]=0;
    fprintf(fout,"Delete %d\n",Key);
}

/*
0이면 없는거고 1이면 있는걸로 하라적혀있었지만 인덱스 찾기 쉽게하기위해서
있으면 인덱스+1을 반환하게끔 썼음
실제 인덱스로 쓸때는 여기서 -1하면됨
*/
int Find(HashTable H, ElementType Key, int solution){
    for (int i=0; i<H->TableSize; i++){
        int h=h1(Key,H,i,solution);
        if (H->TheLists[h]==Key) return h+1;
    }
    return 0;
}

void printTable(HashTable H){
    for(int i=0;i<H->TableSize;i++) fprintf(fout,"%d ",H->TheLists[i]);
    fprintf(fout,"\n");
}

void deleteTable(HashTable H){
    free(H->TheLists);
    free(H);
}
