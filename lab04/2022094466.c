
#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

#define FROMPARENT 0
#define FROMTHREAD 1

typedef struct ThreadedTree* ThreadedPtr;
typedef int ElementType;

struct ThreadedTree {
	int left_thread; // flag if ptr is thread, 1, if it is a thread or 0, if it points to a child node
	ThreadedPtr left_child;
	ElementType data;
	ThreadedPtr right_child;
	int right_thread; // flag if ptr is thread, 1, if it is a thread or 0, if it points to a child node
}ThreadedTree;

//새로운트리
ThreadedPtr CreateTree(){
    ThreadedPtr ret= malloc(sizeof(ThreadedTree));
    ret->left_thread=1;
    ret->right_thread=1; //일단자식노드없으니까1
    ret->left_child=ret;
    ret->right_child=ret;
    ret->data=-1; //root의 data는-1
    return ret;
}

//insert
int Insert(ThreadedPtr root, int root_idx, ElementType data, int idx){
	int height=0, tmp=idx;
    while(tmp/=2){
        height++;
    }
    ThreadedPtr par=root;
    ThreadedPtr ins=malloc(sizeof(ThreadedTree));
    ins->left_thread=1;
    ins->right_thread=1;
    ins->data=data;
    for (int i=height; i>=1; i--){
        if ((idx&(1<<i))==(1<<i)) {
            par=par->right_child;
        }
        else {
            par=par->left_child;
        }
    }
    if (idx%2==0){
        ins->left_child=par->left_child;
        ins->right_child=par;
        par->left_thread=0;
        par->left_child=ins;
    }
    else{
        ins->right_child=par->right_child;
        ins->left_child=par;
        par->right_thread=0;
        par->right_child=ins;
    }
    return 1;
}

//inorder
void printInorder(ThreadedPtr root){
    fprintf(fout, "inorder traversal : ");
    ThreadedPtr cur=root;
    while(cur->right_child!=root){
        if (cur->right_thread) cur=cur->right_child;
        else{
            cur=cur->right_child;
            while(!(cur->left_thread)){
                cur=cur->left_child;
            }
        }
        fprintf(fout, "%d ", cur->data);
    }
}

//트리삭제
void DeleteTree(ThreadedPtr root){
    if (root->left_thread==0) DeleteTree(root->left_child);  //여긴 재귀써도 되는거겠죠
    if (root->right_thread==0) DeleteTree(root->right_child);  //그렇다구해주세요
    free(root);
}

int main(int argc, char *argv[]){
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");

	ThreadedPtr root = CreateTree();

	int NUM_NODES;
	fscanf(fin, "%d", &NUM_NODES);

	int root_idx=0, idx=0;

	while(++idx <= NUM_NODES){
		ElementType data;
		fscanf(fin, "%d", &data);
		
		if(Insert(root, root_idx, data, idx) == 0){
			fprintf(fout, "Insertion failed!\n");
			return 0;
		}
	}

	printInorder(root);
	DeleteTree(root);
	
	
	fclose(fin);
	fclose(fout);

	return 0;
}
