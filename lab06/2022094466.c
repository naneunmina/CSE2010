#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

struct AVLNode;
typedef struct AVLNode *Position;
typedef struct AVLNode *AVLTree;
typedef int ElementType;

typedef struct AVLNode{
	ElementType element;
	AVLTree left, right;
	int height;
}AVLNode;

int Height(Position p);
AVLTree Insert(ElementType X, AVLTree T);
AVLTree Delete(ElementType X, AVLTree T);
void PrintPreorder(AVLTree T);
void DeleteTree(AVLTree T);
Position SingleRotateWithLeft(Position node);
Position SingleRotateWithRight(Position node);
Position DoubleRotateWithLeft(Position node);
Position DoubleRotateWithRight(Position node);

int Height(Position p) {
    if (p == NULL) return -1;
    return p->height;
}
/*
AVL tree에 X를 insert
이미 있다면 오류 출력
*/
AVLTree Insert(ElementType X, AVLTree T){
    if (T==NULL){
        T=malloc(sizeof(struct AVLNode));
        T->element=X;
        T->height=0;
        T->left=T->right=NULL;
    }
    else if (X<T->element){
        T->left=Insert(X,T->left);
        if (Height(T->left)-Height(T->right)>=2){ //양쪽 차이가 2 이상이면 맞춰주기
            if (X<T->left->element) T=SingleRotateWithLeft(T);
            else T=DoubleRotateWithLeft(T);
        }
    }
    else if (X>T->element){
        T->right=Insert(X,T->right);
        if (Height(T->right)-Height(T->left)>=2){ //마찬가지
            if (X>T->right->element) T=SingleRotateWithRight(T);
            else T=DoubleRotateWithRight(T);
        }
    }
    else {
        fprintf(fout, "insertion error : %d is already in the tree!\n", X); //에러 출력
        return T;
    }
    if (Height(T->left) > Height(T->right)) T->height=Height(T->left)+1;
    else T->height=Height(T->right)+1;
    return T;
}

/*
AVL Tree에서 X삭제
이미 없으면 에러 출력
*/
AVLTree Delete(ElementType X, AVLTree T){
    if (T==NULL){
        fprintf(fout, "deletion error : %d is not in the tree!\n", X); //에러 출력
        return T;
    }
    else if (X<T->element){
        T->left=Delete(X,T->left);
        if (Height(T->left) > Height(T->right)) T->height=Height(T->left)+1;
        else T->height=Height(T->right)+1;
        if (Height(T->right)-Height(T->left)>=2){ //양쪽 맞춰주기 insert와 동일
            if (Height(T->left->left)<Height(T->right->right)) T=SingleRotateWithLeft(T);
            else T=DoubleRotateWithLeft(T);
        }
    }
    else if (X>T->element){
        T->right=Delete(X,T->right);
        if (Height(T->left) > Height(T->right)) T->height=Height(T->left)+1;
        else T->height=Height(T->right)+1;
        if (Height(T->left)-Height(T->right)>=2){ //마찬가지
            if (Height(T->left->right)<Height(T->left->left)) T=SingleRotateWithRight(T);
            else T=DoubleRotateWithRight(T);
        }
    }
    else {
        if (T->left==NULL&&T->right==NULL){
            free(T);
            return NULL;
        }
        else if (T->left&&T->right){
            AVLTree tmp=T->right;
            int a=tmp->element;
            while (tmp->left){
                tmp=tmp->left;
                a=tmp->element;
            }
            T->element=a;
            T=Delete(a,T);
        }
    }
    if (Height(T->left) > Height(T->right)) T->height=Height(T->left)+1;
    else T->height=Height(T->right)+1;
    return T;
}
/*
Preorder Traversal -> 재귀로 구현함
*/
void PrintPreorder(AVLTree T){
    fprintf(fout, "%d(%d) ", T->element, T->height);
    if (T->left!=NULL) PrintPreorder(T->left);
    if (T->right!=NULL) PrintPreorder(T->right);
}
/*
트리전체 삭제
*/
void DeleteTree(AVLTree T){
    if (T->left!=NULL) DeleteTree(T->left);
    if (T->right!=NULL) DeleteTree(T->right);
    free(T);
}
/*
4가지 rotation function
*/
Position SingleRotateWithLeft(Position node){
    Position tmp;
    tmp=node->left;
    node->left=tmp->right;
    tmp->right=node;
    
    if (Height(node->left)> Height(node->right)) node->height=Height(node->left)+1;
    else node->height=Height(node->right)+1;
    if (Height(tmp->left) > node->height) tmp->height=Height(tmp->left)+1;
    else tmp->height=node->height+1;

    return tmp;
}
Position SingleRotateWithRight(Position node){
    Position tmp;
    tmp=node->right;
    node->right=tmp->left;
    tmp->left=node;
    
    if (Height(node->left)> Height(node->right)) node->height=Height(node->left)+1;
    else node->height=Height(node->right)+1;
    if (Height(tmp->right) > node->height) tmp->height=Height(tmp->right)+1;
    else tmp->height=node->height+1;

    return tmp;
}
Position DoubleRotateWithLeft(Position node){
    node->left=SingleRotateWithRight(node->left);
    return SingleRotateWithLeft(node);
}
Position DoubleRotateWithRight(Position node){
    node->right=SingleRotateWithLeft(node->left);
    return SingleRotateWithRight(node);
}

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	AVLTree Tree = NULL;
	char cv;
	int key;

	int i=0;

	while(!feof(fin)){
		printf("%d: ", ++i);
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d\n", &key);
				Tree = Insert(key, Tree);
				break;
			case 'd':
				fscanf(fin, "%d\n", &key);
				Tree = Delete(key, Tree);
				break;
		}
		PrintPreorder(Tree);
		fprintf(fout, "\n");
		printf("\n");
	}

	DeleteTree(Tree);
	fclose(fin);
	fclose(fout);
	return 0;
}
