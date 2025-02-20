#include <stdio.h>

int main(){
    char str[40];
    FILE* f = fopen("C:Users\\greg\\Desktop\\in.txt","r");
    FILE* g = fopen("C:Users\\greg\\Desktop\\out.txt","w");

    if(f==NULL || g==NULL)
    {
        printf("Error");
        return 1;
    }
    while(!feof(f)){
        fscanf(f,"%s",str);
        if(str[1] != '\0' && str[1] != '\n')fprintf(g,"%s ",str);
    }
    fclose(f);
    fclose(g);
    return 0;
}