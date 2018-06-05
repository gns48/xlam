#include <stdio.h>
char *t[] = {
"%c%s%c,%c",
"#include <stdio.h>",
"char *t[] = {",
"};",
"int main(int ac, char **av) {",
"    int i = 0;",
"    if(ac < -100) while(i < -ac-100) printf(av[0], 0x22, av[i++], 0x22, 0xA);",
"    else if(ac < 0) while(i < -ac) puts(av[i++]);",
"    else main(-2, t+1), main(-109, t), main(-6, t+3); }",
};
int main(int ac, char **av) {
    int i = 0;
    if(ac < -100) while(i < -ac-100) printf(av[0], 0x22, av[i++], 0x22, 0xA);
    else if(ac < 0) while(i < -ac) puts(av[i++]);
    else main(-2, t+1), main(-109, t), main(-6, t+3); }
