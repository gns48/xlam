#include <stdio.h>
#include <string.h>

char Stack[1024];
int  sp;

char Infix[255];
char Postfix[255];

char push(char c) { Stack[sp++] = c; return c; }
char pop (void)   { return Stack[--sp]; }
char top(void)    { return Stack[sp-1]; }
int  empty(void)  { return sp == 0; }

int prty(int c) {
	int p;
	if(c == '(' || c == ')') return 0;
	else if(c == '+' || c == '-') return 1;
	else if(c == '*' || c == '/') return 2;
	else return -1;
}

void infix2postfix(char* Infix, char* Postfix) {
	char c;
	
	sp = 0;
	
	while(c = *Infix++) {
		switch(c) {
		case '(' :
			push(c);
			break;
		case ')' :
			while(top() != '(') *Postfix++ = pop();
			pop();
			break;
		case '+' :
		case '-' :
		case '*' :
		case '/' :
			while(prty(c) <= prty(top()))
				*Postfix++ = pop();
			push(c);
			break;
		default  :
			*Postfix++ = c;
			break;
		}
	}
	while(!empty()) *Postfix++ = pop();
	*Postfix = '\0';
}

void main() {
	while(1) {
		printf("Enter Infix Espression:> ");
		fgets(Infix, sizeof(Infix), stdin);
		Infix[strlen(Infix)-1] = '\0'; // skip \n
		infix2postfix(Infix, Postfix);
		printf("PostFix Form: %s\n", Postfix);
	}
}

