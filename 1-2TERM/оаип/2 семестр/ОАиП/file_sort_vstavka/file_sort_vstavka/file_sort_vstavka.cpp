#include <stdio.h>
#include <locale.h>
#include <io.h>

// записать в текстовый файл числа и выполнить их сортировку
// метод "вставки"

int main()
{ setlocale(LC_ALL,"Russian" ); 
  FILE *f;
  fpos_t n1,n2,n3;
  int i1,i2,i3, m[]={3,2,5,4,1};

  if (!(f=fopen("file_txt","w+")))
  { puts("файл не может быть создан");
    return 0;
  }
  for(i1=0;i1<sizeof(m)/sizeof(int);i1++) fprintf(f,"%2d",m[i1]);
  rewind(f);
  while(1)
  {   fscanf(f,"%d",&i1);
      printf("%3d",i1);
      if(feof(f)) break;
  }
  printf("\n");
  rewind(f);
  n1=filelength(fileno(f));
  fseek(f,2,0);              // второе число в файле
  while(1)
  { fgetpos(f,&n1);          // позиция сортируемого числа 
    fscanf(f,"%d",&i1);      // выбрано число для сортировки
    if(feof(f)) rewind(f);
    n2=n1-2;                 // позиция предыдущего числа
    fsetpos(f,&n2);          // сдвиг к предыдущему числу
    while(n2>=0)             // пока не достигнуто начало файла
    { fgetpos(f,&n2);        // адрес в файле ' ' предшествующего числу
      fscanf(f,"%d",&i2);    // значение числа выбранного для сравнения
      if(i1<i2)              // слева от i1 найдено большее значение
      { n2+=2;               // вычисление адреса для записи i2
        fsetpos(f,&n2);      //
        fprintf(f,"%2d",i2); // сдвиг i2 вправо
        n2-=2;
      }
      else break;
      n2-=2;
      if(n2>=0) fsetpos(f,&n2);  //
    }
    n2+=2;
    fsetpos(f,&n2);          //
    fprintf(f,"%2d",i1);     // вставка i1 в освободившуюся позицию
    n1+=2;
    if(n1>=filelength(fileno(f))) break;
    fsetpos(f,&n1);          // переход к выбору нового значения i1
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
// метод "вставки"

//int main()
//{
//	setlocale(LC_ALL, "Russian");
//	FILE *f;
//	fpos_t n1, n2, n3;
//	int i1, i2, i3, m[] = { 3, 2, 5, 4, 1 };
//	if (!(f = fopen("file_bin", "w+b")))
//	{
//		puts("файл не может быть создан");
//		return 0;
//	}
//	fwrite(m, sizeof(int), sizeof(m) / sizeof(int), f);
//	rewind(f);                          // возврат в начало файла
//	while (1)
//	{
//		fread(&i1, sizeof(int), 1, f);
//		if (feof(f)) break;
//		printf("%3d", i1);
//	}
//	printf("\n");
//	rewind(f);
//    n1 = ftell(f);                      // длина файла
//	fseek(f, sizeof(int), 0);           // второе число в файле
//	while (1)
//	{
//		fgetpos(f, &n1);                // позиция сортируемого числа 
//		fread(&i1, sizeof(int), 1, f);  // выбрано число для сортировки
//		if (feof(f)) rewind(f);
//		n2 = n1 - sizeof(int);          // позиция предыдущего числа
//		fsetpos(f, &n2);                // сдвиг к предыдущему числу
//		while (n2 >= 0)                 // пока не достигнуто начало файла
//		{
//			fgetpos(f, &n2);            // адрес в файле ' ' предшествующего числу
//			fread(&i2, sizeof(int), 1, f);   // значение числа выбранного для сравнения
//			if (i1<i2)                  // слева от i1 найдено большее значение
//			{
//				n2 += sizeof(int);      // вычисление адреса для записи i2
//				fsetpos(f, &n2);        
//				fwrite(&i2, sizeof(int), 1, f); // сдвиг i2 вправо
//				n2 -= sizeof(int);
//			}
//			else break;
//			n2 -= sizeof(int);
//			if (n2 >= 0) fsetpos(f, &n2);  //
//		}
//		n2 += sizeof(int);
//		fsetpos(f, &n2);                   //
//		fwrite(&i1, sizeof(int), 1, f);    // вставка i1 в освободившуюся позицию
//		n1 += sizeof(int);
//		if (n1 >= filelength(fileno(f))) break;
//		fsetpos(f, &n1);                   // переход к выбору нового значения i1
//	}
//	rewind(f);
//	while (1)
//	{
//		fread(&i1, sizeof(int), 1, f);
//		if (feof(f)) break;
//		printf("%3d", i1);		
//	}
//	printf("\n\n");
//	fclose(f);
//	return 0;
//}
