#include <stdio.h>
#include <string.h>

 char Stack[1024];
 int  sp;

 char Infix[255];
 char Postfix[255];
 char Prefix[255];

 char push(char c) { Stack[sp++] = c; return c; };
 char pop (void)   { return Stack[--sp]; };
 char top(void)    { return Stack[sp-1]; };
 int  empty(void)  { return sp == 0; };

 int  Prty(int c) {
      int p;
      switch(c) {
	case '(' :
	case ')' : p =	0; break;
	case '+' :
	case '-' : p =	1; break;
	case '*' :
	case '/' : p =	2; break;
	default  : p = -1; break;
      }
      return p;
 };

 void infix2postfix(char* Infix, char* Postfix) {
      char c;

      sp = 0;

      while(*Infix) {
	c = *Infix;
	switch(c) {
	  case '(' : push(c);
		     break;
	  case ')' : while(top() != '(') *Postfix++ = pop();
		     pop();
		     break;
	  case '+' :
	  case '-' :
	  case '*' :
	  case '/' : while(Prty(c) <= Prty(top())) *Postfix++ = pop();
		     push(c);
		     break;
	  default  : *Postfix++ = c;
		     break;
	};
	Infix++;
      };
      while(!empty()) *Postfix++ = pop();
      *Postfix = '\0';
 };

 void main() {
      while(1) {
	printf("Enter Infix Espression : > ");
	gets(Infix);
	infix2postfix(Infix, Postfix);
	printf("PostFix Form: %s\n", Postfix);
      };
 };
