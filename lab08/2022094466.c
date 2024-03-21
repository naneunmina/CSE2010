#include<stdio.h>
#include<stdlib.h>
#define INF 1e9


FILE *fin;
FILE *fout;

typedef struct HeapStruct{
	int Capacity;
	int Size;
	int *Elements;
}Heap;

Heap* CreateHeap(int heapSize);
int Insert(Heap *heap, int value);
int Find(Heap *heap, int value);
int DeleteMax(Heap* heap);
int* GetElements(Heap* heap);
int IsFull(Heap *heap);
int IsEmpty(Heap *heap);
int Size(Heap *heap);

void swap(int* a, int* b){
	int t = *a;
	*a = *b;
	*b = t;
}

int main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	char cv;
	Heap* maxHeap;
	int heapSize, key, max_element;
	int ret;
	int* elements;
	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'n':
				fscanf(fin, "%d", &heapSize);
				maxHeap = CreateHeap(heapSize);
				break;
			case 'i':
				fscanf(fin, "%d", &key);
				ret = Insert(maxHeap, key);
				if(ret == 0) fprintf(fout, "insert %d\n", key);
				else if(ret == 1) fprintf(fout, "insert error : heap is full\n");
				else if(ret == 2) fprintf(fout, "insert error : %d is already in the heap\n", key);
				else {
					fprintf(fout, "error: unknown return type\n");
					return -1;
				} 

				break;
			case 'd':
				max_element = DeleteMax(maxHeap);
				if(max_element == 0){
					fprintf(fout, "delete error : heap is empty\n");

				}else if(max_element > 0){
					fprintf(fout, "max element : %d deleted\n", max_element);
				}
				break;
			case 'p':
				if(IsEmpty(maxHeap)){
					fprintf(fout, "print error : heap is empty\n");
				}else{
					elements = GetElements(maxHeap);
					int size = Size(maxHeap);
					for(int i = 0; elements[i] > -1 && i < size; i++){
						fprintf(fout, "%d ", elements[i]);
					}
					fprintf(fout, "\n");
					free(elements);
				}
				break;
			case 'f':
				fscanf(fin, "%d", &key);
				if(Find(maxHeap, key)) fprintf(fout, "%d is in the heap\n", key);
				else fprintf(fout, "finding error : %d is not in the heap\n", key);
				break;
		}
	}

	return 0;
}

Heap* CreateHeap(int heapSize){
    Heap* ret = (Heap*)malloc(sizeof(Heap));
    ret->Capacity=heapSize;
    ret->Size=0;
    ret->Elements=(int*)malloc(sizeof(int)*(heapSize+1)); //1부터 시작하니까 공간은 1더필요
    ret->Elements[0]=INF;
    return ret;
}

/*
0:성공
1:가득참
2:이미있음
*/
int Insert(Heap *heap, int value){
    if (IsFull(heap)) return 1; //가득참
    if (Find(heap,value)) return 2; //이미있음
    int i=++heap->Size;
    heap->Elements[i]=value;
    for (i; heap->Elements[i/2]<value; i/=2){
        swap(&heap->Elements[i],&heap->Elements[i/2]);
    }
    return 0;
}

/*
1:있음
0:없음 
*/
int Find(Heap* heap, int value){
    for (int i=1; i<=Size(heap); i++){
        if (heap->Elements[i]==value) return 1;
    }
    return 0;
}

int DeleteMax(Heap* heap){
    if (IsEmpty(heap)) return 0;
    int max_element = heap->Elements[1], last_element= heap->Elements[heap->Size--], child, i;

    for (i=1; i*2<=heap->Size; i=child){
        child=i*2;
        if (child < heap->Size && heap->Elements[child+1] > heap -> Elements[child]) child++;
        if (last_element < heap->Elements[child]) heap->Elements[i]=heap->Elements[child];
        else break;
    }
    heap->Elements[i]=last_element;
    return max_element;
}

/*
비었으면 NULL return
있으면 동적할당후 deep copy해서 포인터 return
*/
int* GetElements(Heap* heap){
    if (IsEmpty(heap)) return NULL;
    int* ret = (int*)malloc(sizeof(int)*(heap->Capacity));
    for (int i=0; i<heap->Capacity; i++){
        ret[i]=heap->Elements[i+1];
    }
    return ret;
}

/*
1:가득참
0:안참
*/
int IsFull(Heap* heap){
    if (heap->Capacity==Size(heap)) return 1;
    return 0;
}

/*
1:빔
2:안빔
*/
int IsEmpty(Heap *heap){
    if (Size(heap)==0) return 1;
    return 0;
}

/*
heap의 사이즈 return
*/
int Size(Heap *heap){
    return heap->Size;
}
