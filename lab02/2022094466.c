#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;
typedef int ElementType;

struct Node{
	ElementType element;
	Position next;
};

List MakeEmpty(List L);
int IsEmpty(List L);
int IsLast(Position P, List L);
void Insert(ElementType X, List L, Position P);
Position FindPrevious(ElementType X, List L);
Position Find(ElementType X, List L);
void Delete(ElementType X, List L);
void DeleteList(List L);
int* GetElements(List L);
int ListSize=0;

int main(int argc, char **argv){
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");
	char x;
	int* elements;

	Position header=NULL, tmp=NULL;
	header = MakeEmpty(header);
	while(fscanf(fin,"%c",&x)!=EOF){
		if(x=='i'){
			int a,b;	fscanf(fin,"%d%d",&a,&b);
			tmp = Find(a, header);
			if(tmp!=NULL){
				fprintf(fout, "insertion(%d) failed : the key already exists in the list\n", a);
				continue;
			}
			tmp = Find(b, header);
			if(tmp==NULL){
				fprintf(fout, "insertion(%d) failed : can not find the location to be inserted\n", a);
				continue;
			}
			Insert(a, header, tmp);
		}
		else if(x=='d'){
			int a;	fscanf(fin,"%d",&a);
			tmp = Find(a, header);
			if(tmp==NULL){
				fprintf(fout, "deletion(%d) failed : element %d is not in the list\n", a, a);
				continue;
			}
			Delete(a, header);
		}
		else if(x=='f'){
			int a;	fscanf(fin,"%d",&a);
			tmp = FindPrevious(a, header);
			if(IsLast(tmp, header)) fprintf(fout, "finding(%d) failed : element %d is not in the list\n",a,a);
			else{
				if(tmp->element>0) fprintf(fout, "key of the previous node of %d is %d\n", a, tmp->element);
				else fprintf(fout, "key of the previous node of %d is head\n",a);
			}
		}
		else if(x=='p') {
			if(IsEmpty(header))
				fprintf(fout, "empty list!\n");
			else
			{
				elements = GetElements(header);
				for(int i = 0; i < ListSize; i++){
						fprintf(fout, "key:%d ", elements[i]);
					}
					fprintf(fout, "\n");
				free(elements);
			}
		}
	}
	DeleteList(header);
	fclose(fin);
	fclose(fout);

	return 0;
}

List MakeEmpty(List L){
    List list;
    list=(List)malloc(sizeof(struct Node));
    list->element = -1;
    list->next=NULL;
    ListSize=0;   //리스트개수 0
    return list;
}

int IsEmpty(List L){
    return L->next==NULL;
}

int IsLast(Position P, List L){
    return P->next==NULL;
}

void Insert(ElementType X, List L, Position P){
    List list;
    list=(List)malloc(sizeof(struct Node));
    list->element=X;
    list->next=P->next;
    P->next=list;
    ListSize++;  //리스트 개수 증가
}

int* GetElements(List L){
    int* tmp=(int*)malloc(sizeof(int*)*ListSize);   //동적할당
    Position p;
    p=L->next;
    for (int i=0; i<ListSize; i++){
        tmp[i]=p->element;
        p=p->next;
    }
    return tmp;
}

Position FindPrevious(ElementType X, List L){
    Position p;
    p=L;
    while(p->next!=NULL&&p->next->element!=X){
        p=p->next;
    }
    return p;
}

Position Find(ElementType X, List L){
    Position p;
    p=L;  //Head가 -1인것도 확인하기 위해 L부터 시작
    while(p!=NULL&&p->element!=X){
        p=p->next;
    }
    return p;
}

void Delete(ElementType X, List L){
    Position p, tmp;
    p=FindPrevious(X,L);
    if (!IsLast(p,L)){
        tmp=p->next;
        p->next=tmp->next;
        free(tmp);
        ListSize--;   //리스트 개수 감소
    }
}

void DeleteList(List L){
    Position p, tmp;
    p=L->next;
    L->next=NULL;
    while(p!=NULL){
        tmp=p->next;
        free(p);
        p=tmp;
    }
    ListSize=0;   //리스트개수 0
}

//성공!~! 신이나