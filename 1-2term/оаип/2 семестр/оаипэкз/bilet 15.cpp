//bilet 15

void swap(FILE* txt) {
	//здесь есть огромный плюс в том, что все числа двузначные, поэтому сортировать будет очень легко
	fpos_t left = 0, right; //позиции

	//ищем позицию конца
	fseek(txt, 0, 2);   
	fgetpos(txt, &right);
	right -= 3;  //отнимаем т.к. мы получаем позицию feof, а нам нужна позиция числа (3 потому что два числа + пробел)

	//обрабатываем исключение
	if (left == right && left == 0) {
		printf("file is empty!\n");
		return;
	}

	//сортировка
	rewind(txt);
	int left_num, right_num;
	while (left != right) {
		fsetpos(txt, &left);
		fscanf_s(txt, "%d", &left_num);
		fsetpos(txt, &right);
		fscanf_s(txt, "%d", &right_num);
		fsetpos(txt, &right);
		fprintf(txt,"%2d", left_num);
		fsetpos(txt, &left);
		fprintf(txt, "%2d", right_num);
		left += 3;
		right -= 3;
		if (left > right) //если мы видим такую штуку, то ясное дело надо заканчивать
			break;
	}
	return;
}

int main() {
	FILE* txt;

	 fopen_s(&txt, "binary.bin", "w+")
       if (!txt) {
        puts("Cant open file");
        return 1;
    }
		swap(txt);
		fclose(txt);
		return 0;
}