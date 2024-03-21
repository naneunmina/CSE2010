#include<stdio.h>
#include<stdlib.h>

FILE* fin;
FILE* fout;

typedef struct Array Array;

struct Array{
    int size;
    int* values;
};

Array* CreateArray(int size);
void QuickSort(Array* array, int left, int right);
int Partition(Array* array, int left, int right);
void PrintArray(Array* array, int left, int right);
void DeleteArray(Array* array);
void swap(int* a, int* b);

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

    int size, i;
    Array* array;

    fscanf(fin, "%d", &size);
    array = CreateArray(size);
    for(i = 0; i < size; i++){
        fscanf(fin, "%d", &array->values[i]);
    }

    QuickSort(array, 0, size - 1);

    fprintf(fout, "\nsorting result :\n");
    PrintArray(array, 0, size - 1);

    DeleteArray(array);


	return 0;
}

Array* CreateArray(int size){
    Array* ret = (Array*)malloc(sizeof(Array));
    ret->size=size;
    ret->values = (int*)malloc(sizeof(int)*size);
    return ret;
}

/*
재귀적
pivot보다 작은수만 왼쪽에 오는 지점을 p에 저장하고
p를 기준으로 다 시 왼쪽에서 정렬, 오른쪽에서 정렬
*/
void QuickSort(Array* array, int left, int right){
    if (left < right) {
        int p=Partition(array, left, right);
        QuickSort(array, left, p-1);
        QuickSort(array, p+1, right);
    }
}

/*
pivot은 가장 오른쪽 수, 즉 values[right]
swap 할때마다 프린트 함수 불러옴
pivot 인덱스 반환
*/
int Partition(Array* array, int left, int right){
    int pivot = array->values[right];
    fprintf(fout, "left : %d, right : %d, pivot : %d\n", left, right, pivot);
    PrintArray(array, left, right);
    int i=left-1, j=right;
    while (1){
        while (j>0&&array->values[--j]>pivot); //j범위는 outofbound 될까봐 했습니ㅏ
        while (array->values[++i]<=pivot);
        if (i<j){
            swap(&array->values[i], &array->values[j]);
            PrintArray(array, left, right);
        }
        else if (i>=right){
            return right;
        }
        else {
            swap (&array->values[i], &array->values[right]);
            PrintArray(array, left, right);
            return i;
        }
    }
    
}

/*
범위바같쪽은 -로 표시
*/
void PrintArray(Array* array, int left, int right){
    for (int i=0; i<left; i++) fprintf(fout, "%-3s", "-");
    for (int i=left; i<=right; i++) fprintf(fout, "%-3d", array->values[i]);
    for (int i=right+1; i<array->size; i++) fprintf(fout, "%-3s", "-");

    // fprintf(fout, "%-3s", "-");
    // fprintf(fout, "%-3d", array->values[i]);
    fprintf(fout, "\n");
}

void DeleteArray(Array* array){
    free(array->values);
    free(array);
}

void swap(int* a, int* b) {
    int temp =*a;
    *a=*b;
    *b=temp;
}
