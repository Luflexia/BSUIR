
#include <stdio.h>
#include <io.h>
#include <locale.h>
#include <iostream>


// �������� � ��������� ���� ����� � ��������� �� ����������
// ����� "��������"

int main()
{ setlocale(LC_ALL,"Russian" ); 
  FILE *f;
  fpos_t n1,n2,n3;
  int i1,i2,m[]={5,2,1,1,4,3,5,1};

  if (!(f=fopen("file_txt","w+")))
  { puts("���� �� ����� ���� ������");
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
  n1=0;                        // ������� ����� ���������� � ����������
  while(n1<filelength(fileno(f))-2)
  { fseek(f,-4,2);             // ����� � ������� �������������� �����
    while(1)
    { fgetpos(f,&n2);          // ���������� ����� � �����
      fscanf(f,"%d",&i1);      // �������� ������� ����� ��� ���������
      fscanf(f,"%d",&i2);      // �������� ������� ����� ��� ���������
      if(feof(f)) rewind(f);
      if(i1>i2)                // ���� ������ i1 � i2
      {  fsetpos(f,&n2);
         fprintf(f,"%2d",i2);
         fprintf(f,"%2d",i1);
      }
      n2-=2;                   // ����� ��������� ���� �����
	  if(n1>n2) break;         // n2 ����� �� ������� n1-������������ ��-��
      fsetpos(f,&n2);          // ���� �� ��������� ����
    }
    n1+=2;                     // ���������� ������� ��������� �����
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






// �������� � �������� ���� ����� � ��������� �� ����������
// ����� "��������"

//int main()
//{
//	setlocale(LC_ALL, "Russian");
//	FILE *f;
//	fpos_t n1, n2, n3;
//	int i1, i2, i3, m[] = { 5, 2, 6, 1, 4, 3, 5, 1 };
//	if (!(f = fopen("file_bin", "w+b")))
//	{
//		puts("���� �� ����� ���� ������");
//		return 0;
//	}
//	fwrite(m, sizeof(int), sizeof(m) / sizeof(int), f);
//	rewind(f);                      // ������� � ������ �����
//	while (1)
//	{
//		fread(&i1, sizeof(int), 1, f);
//		if (feof(f)) break;
//		printf("%3d", i1);
//	}
//	printf("\n");
//	rewind(f);
//	n3 = sizeof(int);
//	fseek(f, -n3, 2);                // ������� �� ��������� ����� � �����
//	n3 = ftell(f);                   // ������� ���������� �����
//	rewind(f);                       // ������� � ������ �����
//	n1 = 0;
//	while (1)                        // 52614351 
//	{
//		if (n1 >= n3) break;         // n1 �� ��������� ����� � �����
//		n2 = n3;                     // n2-������� ��� �������� � ������ �����
//		while (n1<n2)
//		{
//			fsetpos(f, &n2);         // ������� 1 ������������ �����
//			fread(&i2, sizeof(int), 1, f); // ������ ����� ��� ���������
//			n2 -= sizeof(int);
//			fsetpos(f, &n2);         // ������� 2 ������������ �����
//			fread(&i1, sizeof(int), 1, f); // ������ ����� ��� ���������
//
//			if (i1>i2)               // ��������� � ������ ����� � �����
//			{
//				fsetpos(f, &n2);     // ������� ������������ �����
//				fwrite(&i2, sizeof(int), 1, f); // �������� �����
//				fwrite(&i1, sizeof(int), 1, f); // �������� ����� ���������� ��� ���������
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
