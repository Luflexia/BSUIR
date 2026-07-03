#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define num 3

typedef struct Adress
{
    char City[30];
    char Street[30];
    int House;
    int Flat;
}adress;

typedef struct Info {
    char surname[30];
    char name[30];
    char patronymic[30];
    adress adress;
}info;

void surnameSort(info* arr)
{
    for (int i = 0; i < num; i++)
    {
        for (int j = 0; j < num-1; j++)
        {
            if (strncmp((arr + j)->surname, (arr + j + 1)->surname, 2) > 0)
            {
                info tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

void adressSort(info* arr)
{
    adress a1, a2;
    for (int i = 0; i < num; i++)
    {
        for (int j = 0; j < num; j++)
        {
            a1 = (arr + j)->adress;
            a2 = (arr + j + 1)->adress;
            if (strcmp(a1.City, a2.City) > 0)
            {
                info tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

void search(FILE* stream, char* name)
{
    info buf;
    fseek(stream, 0, SEEK_SET);
    while (fread(&buf, sizeof(info), 1, stream))
        if (strcmp(buf.name, name) == 0)
        {
            printf_s("%s %s %s live at %s %s %d %d\n", buf.surname, buf.name, buf.patronymic,
                     buf.adress.City, buf.adress.Street, buf.adress.House, buf.adress.Flat);
            return;
        }

    printf_s("Student arent exist");
}

void output(FILE* stream)
{
    info info;
    fseek(stream, 0, SEEK_SET);
    while (fread(&info, sizeof(info), 1, stream))
        printf_s("%s %s %s live at %s %s %d %d\n", info.surname, info.name, info.patronymic,
                 info.adress.City, info.adress.Street, info.adress.House, info.adress.Flat);
    puts("\n");
}

void del(FILE* stream, name)
{

    info st, stt;
	fpos_t l;

	do
	{	fread(&stt, sizeof(info), 1, stream);
		if (feof(stream)) break;
		if (!strcmp(st.in.tp, stt.in.tp) && st.in.pr == stt.in.pr &&
			st.hd == stt.hd && st.mem == stt.mem)
		{                  // найдена удаляемая структура   sst
			do
			{	fread(&stt, sizeof(info), 1, stream);          // чтение следующей стр-ры
				if (feof(stream)) break;
				l = ftell(stream);                            // позиция за считанной
				fseek(stream, l - (fpos_t)(2 * sizeof(str)), 0);// на 2 стр-ры назад к началу файла
				fwrite(&stt, sizeof(info), 1, stream);         // и запись считанной ст-ры
				fseek(stream, l, 0);                          // возврат к позиции L
			} while (!feof(stream));
		}
	} while(1);
	rewind(stream);
	fseek(stream, -sizeof(info), 2);
	l = ftell(stream);
    int handle = fileno(stream);
    int _chsize(handle, l);
	fclose(stream);
    }



int main(int argc, char* argv[])
{
    //Создание и заполнение файла
    FILE* f;
    info input[] = { {"Lerov", "Kan", "Jiesh", "Minsk", "Volokhnyanova", 8, 12},
                     {"Kuein", "Yenji", "Wihn", "Bucharest", "Buciumeni", 17, 94},
                     {"Veich", "Nael", "Cont", "Brazil", "Jarina", 35, 76} };

    fopen_s(&f, argv[1], "wb+"); //открываем и меняем название
    if (!f)
    {
        puts("Cant open file");
        return 1;
    }

    for (int i = 0; i < num; i++)
        fwrite(&input[i], sizeof(info), 1, f);

    fclose(f);

    //Запись в массив
    info arr[num] = { 0, 0, 0 };

    fopen_s(&f, argv[1], "ab+");
    if (!f)
    {
        puts("Cant open file");
        return 1;
    }

    fseek(f, 0, SEEK_SET);
    for (int i = 0; fread(&arr[i], sizeof(info), 1, f); i++);

    output(f);
    fclose(f);

    fopen_s(&f, argv[1], "wb+");
    if (!f)
    {
        puts("Can't open file");
        return 1;
    }

    del(f, "Lerov");
    output(f);

    fclose(f);

    return 0;
}






