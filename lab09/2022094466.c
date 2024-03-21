#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct BNode* BNodePtr;

struct BNode{
    int order;
    int size;           /* number of children */
    BNodePtr *child;    /* children pointers */
    int *key;           /* keys */
    int is_leaf;
}BNode;

BNodePtr CreateTree(int order);

void Insert(BNodePtr *root, int key);

BNodePtr Insert_re(BNodePtr par, BNodePtr node, int idx, int key);

void pushkey(BNodePtr node, int key);

BNodePtr split(BNodePtr par, BNodePtr node, int idx);

int Find(BNodePtr root, int key);

void PrintTree(BNodePtr root);

void DeleteTree(BNodePtr root);

int main(int argc, char* argv[]){
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    int order;
    fscanf(fin, "%d", &order);
    BNodePtr root = CreateTree(order);

    char cv;
    int key;
    while(!feof(fin)){
        fscanf(fin, "%c", &cv);
        switch(cv){
            case 'i':
                fscanf(fin, "%d", &key);
                if(Find(root, key))
                    fprintf(fout, "insert error : key %d is already in the tree!\n", key);
                else
                    Insert(&root, key);
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                if(Find(root, key))
                    fprintf(fout, "key %d found\n", key);
                else
                    fprintf(fout, "finding error : key %d is not in the tree!\n", key);
                break;
            case 'p':
                if (root->size == 1)
                    fprintf(fout, "print error : tree is empty!");
                else
                    PrintTree(root);
                fprintf(fout, "\n");
                break;
        }
    }
   
    DeleteTree(root);
    fclose(fin);
    fclose(fout);

    return 0;
}

/*
BTree 제작+새로운 노드 만드는 용도로 쓸 예정
 */
BNodePtr CreateTree(int order){
    BNodePtr ret=(BNodePtr)malloc(sizeof(BNode));
    ret->order=order;
    ret->size=1;
    ret->child=(BNodePtr*)malloc(sizeof(BNodePtr)*(order+2));
    ret->key=(int*)malloc(sizeof(int)*(order+1));
    ret->is_leaf=1;
    for(int i = 0; i <= order+1; i++){
        ret->child[i] = NULL;
    }
    return ret;
}

/*
재귀적으로 insert하는 함수 부를거
*/
void Insert(BNodePtr *root, int key){
    *root=Insert_re(*root,*root,0,key);
}

BNodePtr Insert_re(BNodePtr par, BNodePtr node, int idx, int key){ //재귀적으로 Insert
    if (node->is_leaf==0){ //리프가 아니라면 재귀로 다음으로 넘기기
        int f=0;
        for(int i=0;i<node->size-1; i++){
            if(node->key[i]>key){
                f=1;
                node=Insert_re(node,node->child[i], i, key);
                break;
            }
        }
        if (f==0) node=Insert_re(node,node->child[node->size - 1], node->size -1, key); //for 도는동안 없었으면 마지막 child인거임
    }
    else {
        pushkey(node, key); //리프가 1이면 key 넣기
    }
    if (node->size==node->order+1){ //key 넣고보니 over라면 split
        return split(par,node,idx);
    }
    return par;
}

void pushkey(BNodePtr node, int key){ //단순히 지정한 노드에 key 넣는 함수
    int i=0;
    for(;i<node->size-1; i++){
        if(node->key[i]>key) break;
    }
    for(int j = node->size-1; j>=i; j--){ //뒤로밀기
        node->key[j+1] = node->key[j];
        node->child[j+1] = node->child[j];
    }
    node->key[i]=key;
    node->size++;
}

/*
가장 어려웠던 split.. 최대한 간단하게 짜보려 하였으나
안되면 코드추가하고 또 안되면 코드 추가하고를 반복하다보니 많이 길어졌습니다 ㅠ
*/
BNodePtr split(BNodePtr par, BNodePtr node, int idx){
    int mid=(node->order-1)/2;
    if (par==node){ //루트라면
        BNodePtr right=CreateTree(node->order);
        BNodePtr middle=CreateTree(node->order);
        middle->key[0]=node->key[mid];
        for (int i=mid+1; i<=node->size-1; i++) {
            if(i<node->size-1) right->key[i-mid-1]=node->key[i];
            right->child[i-mid-1]=node->child[i];
        }
        for (int i=mid; i<=node->size-2; i++) {
            node->key[i]=-987654321;
            if (i<node->size-2) node->child[i+1]=NULL;
        }
        middle->child[0]=node;
        middle->child[1]=right;
        middle->size=2;

        if(node->order%2 == 0){ //짝
            node->size = mid+1;
            right->size = mid+2;
        }
        else{ //홀
            node->size = mid+1;
            right->size = mid+1;
        }
        if(node->is_leaf){ //리프반영
            node->is_leaf = 1;
            right->is_leaf = 1;
            middle->is_leaf = 0;
        }
        else{
            node->is_leaf = 0;
            right->is_leaf = 0;
            middle->is_leaf = 0;
        }
        return middle;
    }
    //루트가 아닌부분에서 상승
    pushkey(par,node->key[mid]);
    BNodePtr right = CreateTree(node->order);

    for (int i=mid+1; i<=node->size-1; i++) {
        if(i<node->size-1) right->key[i-mid-1]=node->key[i];
        right->child[i-mid-1]=node->child[i];
    }
    for (int i=mid; i<=node->size-2; i++) {
        node->key[i]=-987654321;
        if (i<node->size-2) node->child[i+1]=NULL;
    }
    
    if(node->order%2 == 0){
        node->size = mid+1;
        right->size = mid+2;
    }
    else{
        node->size = mid+1;
        right->size = mid+1;
    }
    if(node->is_leaf){
        node->is_leaf = 1;
        right->is_leaf = 1;
    }
    else{
        node->is_leaf = 0;
        right->is_leaf = 0;
    }
    par->child[idx+1]=right;
    return par;
}

int Find(BNodePtr root, int key){
    if (root==NULL) return 0;
    int i=0;
    while(i<root->size-1){
        if (root->key[i]==key) return 1;
        else if (root->key[i]<key) i++;
        else return Find(root->child[i],key);
    }
    return Find(root->child[root->size-1],key); //앞에 없으면 마지막 child
}

void PrintTree(BNodePtr root){
    if (root==NULL) return;
    PrintTree(root->child[0]);
    for (int i=0; i<root->size-1; i++){
        fprintf(fout, "%d ", root->key[i]);
        PrintTree(root->child[i+1]);
    }   
}

void DeleteTree(BNodePtr root){
    BNodePtr tmp=root;
    if(tmp==NULL) return;
    for (int i=0; i<tmp->size; i++){
        DeleteTree(tmp->child[i]);
    }
    free(tmp->key);
    free(tmp->child);
    free(tmp);
}
