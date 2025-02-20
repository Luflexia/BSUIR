#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *f1, *f2;
    char text[100], blank[100];
    int c = 0, d = 0;


    f1 = fopen("C:Users\\mrxop\\Desktop\\in.txt", "r");
    f2 = fopen("C:Users\\mrxop\\Desktop\\out.txt", "w");

    printf("Source\n%s\n");

    if ( NULL != fgets ( text, 100, f1 ) )
    {  printf("%s ",text); };

    while (text[c] != '\0')
    {
        if (!(text[c] == ' ' && text[c+1] == ' ')) {
            blank[d] = text[c];
            d++;
        }
        c++;
    }

    blank[d] = '\0';

    printf("Result\n%s\n", blank);

    fclose(f1);
    fclose(f2);
    return 0;
}

