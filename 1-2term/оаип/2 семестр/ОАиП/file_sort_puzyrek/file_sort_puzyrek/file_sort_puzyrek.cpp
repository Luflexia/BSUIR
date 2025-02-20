
#include <stdio.h>
#include <io.h>
#include <locale.h>
#include <iostream>


// записать в текстовый файл числа и выполнить их сортировку
// метод "пузырька"

int main()
{ setlocale(LC_ALL,"Russian" ); 
  FILE *f;
  fpos_t n1,n2,n3;
  int i1,i2,m[]={5,2,1,1,4,3,5,1};

  if (!(f=fopen("file_txt","w+")))
  { puts("файл не может быть создан");
    return 0;
  }
  for(i1=0;i1<sizeof(m)/sizeof(int);i1++) fprintf(f,"%2d",m[i1]);
  rewind(f);
  while(1)                     // while(feof(f)) 
  { fscanf(f,"%d",&i1);
    printf("%3d",i1);
	if(feof(f)) break;         // !!!!!!!!!!
  }
  n2 = ftell(f);
  printf("\n\n");
  rewind(f);                   //5 2 6 1 4 3 5 1 EOF
  n1=0;                        // позиция числа выбранного к сортировке
  while(n1<filelength(fileno(f))-2)
  { fseek(f,-4,2);             // сдвиг в позицию предпоследнего числа
    while(1)
    { fgetpos(f,&n2);          // запоминаем адрес в файле
      fscanf(f,"%d",&i1);      // значение первого числа для сравнения
      fscanf(f,"%d",&i2);      // значение второго числа для сравнения
      if(feof(f)) rewind(f);
      if(i1>i2)                // блок замены i1 с i2
      {  fsetpos(f,&n2);
         fprintf(f,"%2d",i2);
         fprintf(f,"%2d",i1);
      }
      n2-=2;                   // адрес следующей пары чисел
	  if(n1>n2) break;         // n2 дошло до позиции n1-сортируемого эл-та
      fsetpos(f,&n2);          // УТПФ на следующую пару
    }
    n1+=2;                     // предвигаем позицию исходного числа
  }
  rewind(f);
  while(1)
  { fscanf(f,"%d",&i1);
    printf("%3d",i1);
    if(feof(f)) break;
  }
  printf("\n\n");
  fclose(f);
  return 0;
}






// записать в бинарный файл числа и выполнить их сортировку
// метод "пузырька"

//int main()
//{
//	setlocale(LC_ALL, "Russian");
//	FILE *f;
//	fpos_t n1, n2, n3;
//	int i1, i2, i3, m[] = { 5, 2, 6, 1, 4, 3, 5, 1 };
//	if (!(f = fopen("file_bin", "w+b")))
//	{
//		puts("файл не может быть создан");
//		return 0;
//	}
//	fwrite(m, sizeof(int), sizeof(m) / sizeof(int), f);
//	rewind(f);                      // возврат в начало файла
//	while (1)
//	{
//		fread(&i1, sizeof(int), 1, f);
//		if (feof(f)) break;
//		printf("%3d", i1);
//	}
//	printf("\n");
//	rewind(f);
//	n3 = sizeof(int);
//	fseek(f, -n3, 2);                // переход на последнее число в файле
//	n3 = ftell(f);                   // позиция последнего числа
//	rewind(f);                       // возврат в начало файла
//	n1 = 0;
//	while (1)                        // 52614351 
//	{
//		if (n1 >= n3) break;         // n1 на последнем числе в файле
//		n2 = n3;                     // n2-позиция для движения в начало файла
//		while (n1<n2)
//		{
//			fsetpos(f, &n2);         // позиция 1 считываемого числа
//			fread(&i2, sizeof(int), 1, f); // первое число для сравнения
//			n2 -= sizeof(int);
//			fsetpos(f, &n2);         // позиция 2 считываемого числа
//			fread(&i1, sizeof(int), 1, f); // второе число для сравнения
//
//			if (i1>i2)               // сравнение и замена чисел в файле
//			{
//				fsetpos(f, &n2);     // позиция считываемого числа
//				fwrite(&i2, sizeof(int), 1, f); // исходное число
//				fwrite(&i1, sizeof(int), 1, f); // значение числа выбранного для сравнения
//			}
//		}
//		n1 += sizeof(int);
//	}
//	rewind(f);
//	fread(m, sizeof(m), 1, f);
//	for (i1 = 0; i1<sizeof(m) / sizeof(int); printf("%3d", m[i1++]));
//	printf("\n\n");
//	fclose(f);
//	return 0;
//}
