#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ISFULL 1
#define ISEMPTY 2
#define DIVIDEZERO 3

FILE *fin;
FILE *fout;

typedef struct Stack{
	int *key; //array
	int top; //가장 위쪽 인덱스
	int max_stack_size; //capacity
}Stack;

Stack* CreateStack(int max);
int Push(Stack* S, int X);
int Pop(Stack* S);
int Top(Stack* S);

void DeleteStack(Stack* S);
int IsEmpty(Stack* S);
int IsFull(Stack* S);

void main(int argc, char *argv[]){
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");

	Stack* stack;
	char input_str[101];
	int max=20, i=0,a,b,result, error_flag=0;
	int ret;
	fgets(input_str,101,fin);
	stack = CreateStack(max);
	
	fprintf(fout, "top numbers : ");
	while(input_str[i]!='#'){
		if('1'<=input_str[i] && input_str[i]<='9'){
			ret = Push(stack,input_str[i]-'0'); //push못하면 true 반환
			if(ret){
				error_flag = ISFULL;
				break;
			}
		}
		else{
			if(IsEmpty(stack)){
				error_flag = ISEMPTY;
				break;
			}
			int b = Pop(stack);
			if(IsEmpty(stack)){
				error_flag = ISEMPTY;
				break;
			}
			int a = Pop(stack);
			
			if(IsFull(stack)){
				error_flag = ISFULL;
				break;
			}
			switch (input_str[i]){
				case '+' :
					ret = Push(stack, a+b);
					if(ret){
						error_flag = ISFULL;
					}
					break;
				case '-' :
					ret = Push(stack, a-b);
					if(ret){
						error_flag = ISFULL;
					}
					break;
				case '*' :
					ret = Push(stack, a*b);
					if(ret){
						error_flag = ISFULL;
					}
					break;
				case '/' : 
					if(b==0){
						error_flag = DIVIDEZERO;
						break;
					}
					ret = Push(stack, a/b);
					if(ret){
						error_flag = ISFULL;
					} 
					break;
				case '%' : 
					if(b==0){
						error_flag = DIVIDEZERO;
						break;
					}
					ret = Push(stack, a%b);
					if(ret){
						error_flag = ISFULL;
					} 
					break;
				default : break;
			}
		}

		if(error_flag) break;

		int t = Top(stack);
		fprintf(fout, "%d ",t);
		result=t;
		i++;
	}

	if(error_flag == ISFULL){
		fprintf(fout, "\nerror : invalid postfix expression, stack is full!\n");
	}
	else if(error_flag == ISEMPTY){
		fprintf(fout, "\nerror : invalid postfix expression, stack is empty!\n");
	}
	else if(error_flag == DIVIDEZERO){
		fprintf(fout, "\nerror : invalid postfix expression, divide by zero!\n");
	}

	else{
		if(stack->top+1 > 1){
			fprintf(fout, "\nerror : invalid postfix expression, %d elements are left!\n", stack->top+1);
		}
		else{
			fprintf(fout, "\nevaluation result : %d\n",result);
		}
	}
	fclose(fin);
	fclose(fout);
	DeleteStack(stack);
}


/*
max는 스택 최대크기. 20고정
return: Stack*: the pointer of new stack 
*/
Stack* CreateStack(int max){
    Stack* s;
    s = malloc(sizeof(Stack));
    s->key=malloc(sizeof(int)*max);
    s->max_stack_size=max;
    s->top=-1;
    return s;
}
/*
key먼저 free 후 stack free
Stack* S: the pointer of the Stack 
*/
void DeleteStack(Stack* S){
    free(S->key);
    free(S);
}

/*
성공시 return 0
스택 가득차서 실패시 return 1
Push the value into Stack
*/
int Push(Stack* S, int X){
    if (IsFull(S)) return 1;
    else {
        S->key[++(S->top)]=X;
        return 0;
    }
}

/*
팝팝팝팝파
top값 return하는 동시에 top하나 줄이기
*/
int Pop(Stack* S){
    return S->key[(S->top)--];
}
/*
top값 return만 하기
*/
int Top(Stack* S){
    return S->key[S->top];
}
/*
return:
	1, stack is full
	0, stack is not full
*/
int IsFull(Stack* S){
    return S->top==S->max_stack_size-1;
}
/*
return:
	1, stack is empty
	0, stack is not empty
*/
int IsEmpty(Stack* S){
    return S->top==-1;
}