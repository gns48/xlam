#include <stdio.h>
char *t[] = {
"%c%s%c,%c",
"#include <stdio.h>",
"char *t[] = {",
"};",
"int main(int ac, char **av) {",
"    if(ac < -100) for(int i = 0; i < -ac-100; i++) printf(av[0], 0x22, av[i], 0x22, 0x0A);",
"    else if(ac < 0) for(int i = 0; i < -ac; i++) puts(av[i]);",
"    else main(-2, t+1), main(-108, t), main(-5, t+3); }",
};
int main(int ac, char **av) {
    if(ac < -100) for(int i = 0; i < -ac-100; i++) printf(av[0], 0x22, av[i], 0x22, 0x0A);
    else if(ac < 0) for(int i = 0; i < -ac; i++) puts(av[i]);
    else main(-2, t+1), main(-108, t), main(-5, t+3); }
