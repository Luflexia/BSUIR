#include <stdio.h>
#include <io.h>
#include <locale.h>
#include <iostream>

// записать в текстовый файл числа и выполнить их сортировку
//  метод "через отбор"

int main()
{  setlocale(LC_ALL,"Russian" ); 
   FILE *f;
   fpos_t n1,n2,n3;
   int i1, i2, i3, m[] = { 2, 6, 4, 3, 1, 5 };
   if (!(f=fopen("file_txt","w+")))
   { puts("файл не может быть создан");
     return 0;
   }
   for(i1=0; i1<sizeof(m)/sizeof(int) ;i1++) fprintf(f,"%2d",m[i1]);
   rewind(f);
   while(1)                 // вывод на экран исходного файла
   {  fscanf(f,"%d",&i1);
      printf("%3d",i1);
      if(feof(f)) break;
   }
   printf("\n");
   rewind(f);
   while(1)
   { fgetpos(f,&n1);        // адрес исходного числа
     fscanf(f,"%d",&i1);    // исходное число
     if(feof(f)) break;
     n3=n1; i3=i1;          // для контроля найдено ли новое min значение
     while(1)
     { fgetpos(f,&n2);      // адрес в файле ' ' предшествующего числу
       fscanf(f,"%d",&i2);  // значение числа выбранного для сравнения
       if(i3>i2)            // найдено новое (меньшее) значение
       {  n3=n2;            // запоминаем позицию
          i3=i2;            // запоминаем значение
       }
       if(feof(f)) break;   // !!!!!!!!!!!!!!!!!!!!!!!
     }
     if(n1!=n3)
     {  fsetpos(f,&n3);      // замена значений в позициях n3 и n1
        fprintf(f,"%2d",i1); //
        fsetpos(f,&n1);      //
        fprintf(f,"%2d",i3); //
     }
	     //   fseek(f,0,1);
     n1+=2;
     fsetpos(f,&n1);         // переход в позицию для выбора нового числа к сортировке
   }
   rewind(f);
   while(1)
   {  fscanf(f,"%d",&i1);
      printf("%3d",i1);
      if(feof(f)) break;
   }
   printf("\n\n");
   fclose(f);
   return 0;
}





// записать в бинарный файл числа и выполнить их сортировку
//  метод "через отбор"

//int main()
//{
//	setlocale(LC_ALL, "Russian");
//	FILE *f;
//	fpos_t n1, n2, n3;
//	int i1, i2, i3, m[] = { 2, 1, 6, 4, 5, 3, 1, 5 };
//	if (!(f = fopen("file_bin", "w+b")))
//	{
//		puts("файл не может быть создан");
//		return 0;
//	}
//	fwrite(m, sizeof(int), sizeof(m) / sizeof(int), f);
//	rewind(f);       // возврат в начало файла;
//	while (1)
//	{
//		fread(&i1, sizeof(int), 1, f);
//		if (feof(f)) break;
//		printf("%3d", i1);
//	}
//	printf("\n");
//	rewind(f);
//	while (1)
//	{
//		fgetpos(f, &n1);        // адрес исходного числа
//		fread(&i1, sizeof(int), 1, f);    // исходное число
//		if (feof(f)) break;
//		n3 = n1; i3 = i1;          // для контроля найдено ли новое min значение
//		while (1)
//		{
//			fgetpos(f, &n2);      // адрес в файле ' ' предшествующего числу
//			fread(&i2, sizeof(int), 1, f);  // значение числа выбранного для сравнения 
//			if (feof(f)) break;   // 
//			if (i3>i2)            // найдено новое (меньшее) значение
//			{
//				n3 = n2;            // запоминаем позицию
//				i3 = i2;            // запоминаем значение
//			}
//		}
//		if (n1 != n3)
//		{
//			fsetpos(f, &n3);      // замена значений в позициях n3 и n1
//			fwrite(&i1, sizeof(int), 1, f); //
//			fsetpos(f, &n1);      //
//			fwrite(&i3, sizeof(int), 1, f); //
//		}
//		n1 += sizeof(int);
//		if (n1 >= filelength(fileno(f))) break;
//		fsetpos(f, &n1);         // выбор нового числа к сортировке
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


