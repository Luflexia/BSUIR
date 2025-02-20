#include <stdio.h>
#include <locale.h>
#include <io.h>

// �������� � ��������� ���� ����� � ��������� �� ����������
// ����� "�������"

int main()
{ setlocale(LC_ALL,"Russian" ); 
  FILE *f;
  fpos_t n1,n2,n3;
  int i1,i2,i3, m[]={3,2,5,4,1};

  if (!(f=fopen("file_txt","w+")))
  { puts("���� �� ����� ���� ������");
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
  fseek(f,2,0);              // ������ ����� � �����
  while(1)
  { fgetpos(f,&n1);          // ������� ������������ ����� 
    fscanf(f,"%d",&i1);      // ������� ����� ��� ����������
    if(feof(f)) rewind(f);
    n2=n1-2;                 // ������� ����������� �����
    fsetpos(f,&n2);          // ����� � ����������� �����
    while(n2>=0)             // ���� �� ���������� ������ �����
    { fgetpos(f,&n2);        // ����� � ����� ' ' ��������������� �����
      fscanf(f,"%d",&i2);    // �������� ����� ���������� ��� ���������
      if(i1<i2)              // ����� �� i1 ������� ������� ��������
      { n2+=2;               // ���������� ������ ��� ������ i2
        fsetpos(f,&n2);      //
        fprintf(f,"%2d",i2); // ����� i2 ������
        n2-=2;
      }
      else break;
      n2-=2;
      if(n2>=0) fsetpos(f,&n2);  //
    }
    n2+=2;
    fsetpos(f,&n2);          //
    fprintf(f,"%2d",i1);     // ������� i1 � �������������� �������
    n1+=2;
    if(n1>=filelength(fileno(f))) break;
    fsetpos(f,&n1);          // ������� � ������ ������ �������� i1
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
// ����� "�������"

//int main()
//{
//	setlocale(LC_ALL, "Russian");
//	FILE *f;
//	fpos_t n1, n2, n3;
//	int i1, i2, i3, m[] = { 3, 2, 5, 4, 1 };
//	if (!(f = fopen("file_bin", "w+b")))
//	{
//		puts("���� �� ����� ���� ������");
//		return 0;
//	}
//	fwrite(m, sizeof(int), sizeof(m) / sizeof(int), f);
//	rewind(f);                          // ������� � ������ �����
//	while (1)
//	{
//		fread(&i1, sizeof(int), 1, f);
//		if (feof(f)) break;
//		printf("%3d", i1);
//	}
//	printf("\n");
//	rewind(f);
//    n1 = ftell(f);                      // ����� �����
//	fseek(f, sizeof(int), 0);           // ������ ����� � �����
//	while (1)
//	{
//		fgetpos(f, &n1);                // ������� ������������ ����� 
//		fread(&i1, sizeof(int), 1, f);  // ������� ����� ��� ����������
//		if (feof(f)) rewind(f);
//		n2 = n1 - sizeof(int);          // ������� ����������� �����
//		fsetpos(f, &n2);                // ����� � ����������� �����
//		while (n2 >= 0)                 // ���� �� ���������� ������ �����
//		{
//			fgetpos(f, &n2);            // ����� � ����� ' ' ��������������� �����
//			fread(&i2, sizeof(int), 1, f);   // �������� ����� ���������� ��� ���������
//			if (i1<i2)                  // ����� �� i1 ������� ������� ��������
//			{
//				n2 += sizeof(int);      // ���������� ������ ��� ������ i2
//				fsetpos(f, &n2);        
//				fwrite(&i2, sizeof(int), 1, f); // ����� i2 ������
//				n2 -= sizeof(int);
//			}
//			else break;
//			n2 -= sizeof(int);
//			if (n2 >= 0) fsetpos(f, &n2);  //
//		}
//		n2 += sizeof(int);
//		fsetpos(f, &n2);                   //
//		fwrite(&i1, sizeof(int), 1, f);    // ������� i1 � �������������� �������
//		n1 += sizeof(int);
//		if (n1 >= filelength(fileno(f))) break;
//		fsetpos(f, &n1);                   // ������� � ������ ������ �������� i1
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
