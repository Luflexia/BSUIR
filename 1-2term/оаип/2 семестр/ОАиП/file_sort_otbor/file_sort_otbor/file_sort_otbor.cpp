#include <stdio.h>
#include <io.h>
#include <locale.h>
#include <iostream>

// �������� � ��������� ���� ����� � ��������� �� ����������
//  ����� "����� �����"

int main()
{  setlocale(LC_ALL,"Russian" ); 
   FILE *f;
   fpos_t n1,n2,n3;
   int i1, i2, i3, m[] = { 2, 6, 4, 3, 1, 5 };
   if (!(f=fopen("file_txt","w+")))
   { puts("���� �� ����� ���� ������");
     return 0;
   }
   for(i1=0; i1<sizeof(m)/sizeof(int) ;i1++) fprintf(f,"%2d",m[i1]);
   rewind(f);
   while(1)                 // ����� �� ����� ��������� �����
   {  fscanf(f,"%d",&i1);
      printf("%3d",i1);
      if(feof(f)) break;
   }
   printf("\n");
   rewind(f);
   while(1)
   { fgetpos(f,&n1);        // ����� ��������� �����
     fscanf(f,"%d",&i1);    // �������� �����
     if(feof(f)) break;
     n3=n1; i3=i1;          // ��� �������� ������� �� ����� min ��������
     while(1)
     { fgetpos(f,&n2);      // ����� � ����� ' ' ��������������� �����
       fscanf(f,"%d",&i2);  // �������� ����� ���������� ��� ���������
       if(i3>i2)            // ������� ����� (�������) ��������
       {  n3=n2;            // ���������� �������
          i3=i2;            // ���������� ��������
       }
       if(feof(f)) break;   // !!!!!!!!!!!!!!!!!!!!!!!
     }
     if(n1!=n3)
     {  fsetpos(f,&n3);      // ������ �������� � �������� n3 � n1
        fprintf(f,"%2d",i1); //
        fsetpos(f,&n1);      //
        fprintf(f,"%2d",i3); //
     }
	     //   fseek(f,0,1);
     n1+=2;
     fsetpos(f,&n1);         // ������� � ������� ��� ������ ������ ����� � ����������
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





// �������� � �������� ���� ����� � ��������� �� ����������
//  ����� "����� �����"

//int main()
//{
//	setlocale(LC_ALL, "Russian");
//	FILE *f;
//	fpos_t n1, n2, n3;
//	int i1, i2, i3, m[] = { 2, 1, 6, 4, 5, 3, 1, 5 };
//	if (!(f = fopen("file_bin", "w+b")))
//	{
//		puts("���� �� ����� ���� ������");
//		return 0;
//	}
//	fwrite(m, sizeof(int), sizeof(m) / sizeof(int), f);
//	rewind(f);       // ������� � ������ �����;
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
//		fgetpos(f, &n1);        // ����� ��������� �����
//		fread(&i1, sizeof(int), 1, f);    // �������� �����
//		if (feof(f)) break;
//		n3 = n1; i3 = i1;          // ��� �������� ������� �� ����� min ��������
//		while (1)
//		{
//			fgetpos(f, &n2);      // ����� � ����� ' ' ��������������� �����
//			fread(&i2, sizeof(int), 1, f);  // �������� ����� ���������� ��� ��������� 
//			if (feof(f)) break;   // 
//			if (i3>i2)            // ������� ����� (�������) ��������
//			{
//				n3 = n2;            // ���������� �������
//				i3 = i2;            // ���������� ��������
//			}
//		}
//		if (n1 != n3)
//		{
//			fsetpos(f, &n3);      // ������ �������� � �������� n3 � n1
//			fwrite(&i1, sizeof(int), 1, f); //
//			fsetpos(f, &n1);      //
//			fwrite(&i3, sizeof(int), 1, f); //
//		}
//		n1 += sizeof(int);
//		if (n1 >= filelength(fileno(f))) break;
//		fsetpos(f, &n1);         // ����� ������ ����� � ����������
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


