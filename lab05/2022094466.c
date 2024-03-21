#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct BST* Tree;
typedef struct BST{
	int value;
	struct BST* left;
	struct BST* right;
}BST;

Tree insertNode(Tree root, int key);
Tree deleteNode(Tree root, int key);
int findNode(Tree root, int key);
void printInorder(Tree root);
void deleteTree(Tree root);

void main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char cv;
	int key;

	Tree root = NULL;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d", &key);
				if(!findNode(root, key))
					root = insertNode(root, key);
				else
					fprintf(fout, "insertion error: %d is already in the tree\n", key);
				break;
			case 'f':
				fscanf(fin,"%d",&key);
				if(findNode(root, key))
					fprintf(fout, "%d is in the tree\n", key);
				else
					fprintf(fout, "finding error: %d is not in the tree\n", key);
				break;
			case 'd':
				fscanf(fin, "%d", &key);
				if(findNode(root, key)){
					root = deleteNode(root, key);
					fprintf(fout, "delete %d\n", key);
				}
				else{
					fprintf(fout, "deletion error: %d is not in the tree\n", key);
				}
				break;
			case 'p': //얘때문에 inpu.txt 마지막에 엔터 쳐줘야함
				fscanf(fin, "%c", &cv);
				if(cv == 'i'){
					if(root == NULL)
						fprintf(fout, "tree is empty");
					else
						printInorder(root);
				}
				fprintf(fout, "\n");
				break;
		}
	}
	deleteTree(root);
}

Tree insertNode(Tree root, int key){
	if (root == NULL){
		root=(Tree) malloc(sizeof(struct BST));
		root->value = key;
		root->left = root->right = NULL;
		fprintf(fout, "insert %d\n", key);
	}
	else if (key < root->value) root->left = insertNode(root->left,key);
	else if (key > root->value) root->right = insertNode(root->right,key);
	return root;
}

Tree deleteNode(Tree root, int key){
	if (key<root->value) root->left = deleteNode(root->left,key);
	else if (key>root->value) root->right = deleteNode(root->right,key);
	else if (root->left!=NULL && root->right!=NULL){ //key 찾았을때 실행됨
		Tree tmp=root->right;
		while (tmp->left!=NULL) tmp=tmp->left;
		root->value=tmp->value;
		root->right = deleteNode(root->right,root->value);
	}
	else{
		Tree tmp = root;
		if (root->left==NULL) root=root->right;
		else root=root->left;
		free(tmp);
	}
	return root;
}

int findNode(Tree root, int key){
	if (root==NULL) return 0;
	else if (key<root->value) return findNode(root->left,key);
	else if (key>root->value) return findNode(root->right,key);
	else return 1;
}

void printInorder(Tree root){
	if (root->left!=NULL) printInorder(root->left);
	fprintf(fout, "%d ", root->value);
	if (root->right!=NULL) printInorder(root->right);
}

void deleteTree(Tree root){
	if (root->left!=NULL) deleteTree(root->left);
    if (root->right!=NULL) deleteTree(root->right);
    free(root);
}
