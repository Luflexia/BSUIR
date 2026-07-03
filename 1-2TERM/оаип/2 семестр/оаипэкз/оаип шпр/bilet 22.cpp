//Ѕилет 22 организовать ввод числа с клавиатуры и удаление из бинарного файла.

void del(FILE* f, int n) {
	fpos_t start, end;
	int i,n1 = 0;
	int i1;
	int size = ftell(f);
	rewind(f);
	fread(&i, sizeof(int), 1, f);
	while (1) {
		if (n1 == n) {
			fseek(f, -4, 1);
			fgetpos(f, &start);
			fread(&i1, sizeof(int), 1, f);
			fgetpos(f, &end);
			while (1) {
				fsetpos(f, &end);
				fread(&i1, sizeof(int), 1, f);
				if (feof(f)) {
					break;
				}
				fsetpos(f, &start);
				fwrite(&i1, sizeof(int), 1, f);
				start++;
				end++;
			}
			chsize(fileno(f), size - 4);
			break;
		}
		fread(&i, sizeof(int), 1, f);
		if (feof(f)) {
			break;
		}
		n1++;
	}
}
int main() {
	FILE* f;
	if (!(f = fopen("file", "w+b"))) {
		printf("Error\n");
		return 0;
	}
	int n;
	int i1;
	
	printf("сколько чисел добавить?\n");
	while (!(scanf("%d", &n) && n<0)) {
		printf("error\n");
		rewind(stdin);
	}

	for (int i = 0; i < n; i++)
	{
		printf("¬ведите число\n");
		while ((!scanf("%d", &i1) && n<0)) {
			rewind(stdin);
		}
		fwrite(&i1, sizeof(int), 1, f);
	}
	
	printf("\n¬ведите номер числа которое хотите удалить\n");
	while (!(scanf("%d", &n) && n<0)) {
		rewind(stdin);
	}
	del(f, n-1);
	fclose(f);
	return 0;
}