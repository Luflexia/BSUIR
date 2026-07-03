#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct students
{
    struct person
    {
        char surname[20],name[20],patronymic[20];
    }fullname;
    union characteristics
    {
        struct info_1
        {
            int height,weight;
        }characteristics_1;
        struct info_2
        {
            int height,weight,age,pulse;
        }characteristics_2;
    }characteristics;
    int union_num;
};

/*
char* check_input(void)
{
    char *string=(char*)calloc(20,sizeof(char));
    do {
        fgets(string, 20, stdin);
        if(strlen(string)==19)
            while(getchar()!='\n');
    } while (string[0] == '\n');
    return string;
}
*/

void input(struct students *person, int records)
{
    char *string;
    do{
        string=(char*)calloc(20,sizeof(char));
    }while(!string);
    int number=0,num;
    puts("enter surname ");
    /*string = check_input();*/
    strcpy(person[records].fullname.surname);
    puts("enter name ");
    /*string = check_input();*/
    strcpy(person[records].fullname.name);
    puts("enter patronymic ");
    /*string = check_input();*/
    strcpy(person[records].fullname.patronymic);
    puts("1-height,weight 2-height,weight,other characteristics");
    /*num=atoi(check_input());*/
    if(num==1)
    {
        person[records].union_num=1;
        puts("enter height");
        /*number=atoi(check_input());
        person[records].characteristics.characteristics_1.height=number;
        puts("enter weight");
        /*number=atoi(check_input());*/
        person[records].characteristics.characteristics_1.weight=number;
    }
    else
    {
        person[records].union_num=2;
        puts("enter height");
        /*number=atoi(check_input());*/
        person[records].characteristics.characteristics_2.height=number;
        puts("enter weight");
        /*number=atoi(check_input());*/
        person[records].characteristics.characteristics_2.weight=number;
        puts("enter age");
        /*number=atoi(check_input());*/
        person[records].characteristics.characteristics_2.age=number;
        puts("enter pulse");
        /*number=atoi(check_input());*/
        person[records].characteristics.characteristics_2.pulse=number;
    }
}

show_students( struct students *person, int records )
{
    int i,num;
    puts("1-show people with height,weight/2-show people with other characteristics");
    /*num=atoi(check_input());*/
    if(num==1)
    {
        for(i=0;i<records;i++)
        {
            if(person[i].union_num==1)
            {
                printf("\n%s%s%s",person[i].fullname.surname, person[i].fullname.name, person[i].fullname.patronymic);
                printf("\n%dsm %dkg",person[i].characteristics.characteristics_1.height, person[i].characteristics.characteristics_1.weight);
            }
        }
    }
    else if(num==2)
    {
        for(i=0;i<records;i++)
        {
            if(person[i].union_num==2)
            {
                printf("\n%s%s%s",person[i].fullname.surname, person[i].fullname.name, person[i].fullname.patronymic);
                printf("\n%dsm %dkg",person[i].characteristics.characteristics_2.height, person[i].characteristics.characteristics_2.weight);
                printf("\n%dyears \npulse: %d ",person[i].characteristics.characteristics_2.age, person[i].characteristics.characteristics_2.pulse);
            }
        }
    }
}

int main(int argc,char *argv[])
{
    if(argc>1)
    {
        if(!strcmp(argv[1],"-h"))
            printf("\nEnter students' names and characteristics\n");;
    }
    int records = 0, i;
    char *string;
    struct students *person;
    do {
        string = (char *) calloc(5, sizeof(char));
        person = (struct students*) calloc(5, sizeof(struct students));
    } while (!person && !string);
    puts("ATTENCION: maximum length of record cannot exceed 19 symbols");
    while (records < 5) {
        input(person, records);
        records++;
        puts("enter stop to quit");
        fgets(string,5,stdin);
        if(strcmp(string,"stop")==0)
            break;
    }
    if (records == 5)
        puts("you can enter only 5 persons");
    show_students(person,records);
    return 0;
}
