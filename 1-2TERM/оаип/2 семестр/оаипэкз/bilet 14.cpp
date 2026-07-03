//bilet 14

void ent(FILE* bin) {
	//для начала хотя бы ведем число
	//если файл изначально не отсортирован - виноваты вы
	system("cls");
	printf("entering number without loosing out sorting: ");
	int enter, checker;
	checker = scanf_s("%d", &enter);
	while (!checker)
		checker = scanf_s("%d", &enter);

	//теперь наша цель пробежать по файлу, чтобы найти место для числа, и всё сдвинуть к херам =)
	//но для начала найдём конец файла
	fpos_t end;
	rewind(bin);
	fseek(bin, 0, 2);
	fgetpos(bin, &end);
	end -= sizeof(int);

	//обрабатываем исключение
	if (end <= 0) {
		fwrite(&enter, sizeof(int), 1, bin);
		return;
	}
	rewind(bin);
	
	//теперь наш основной цикл
	fpos_t probeg; int temp;
	fgetpos(bin, &probeg);
	while (probeg <= end) {
		fread(&temp, sizeof(int), 1, bin);
		if (temp >= enter) { //когда мы нашли позицию
			while (probeg <= end+sizeof(int)) {
				int next;                            //нам нужно запомнить следующиее значение чтобы не потерять его
				fsetpos(bin, &probeg);              //становимся на позицию замеы
				fread(&next, sizeof(int), 1, bin); //запоминаем ее для дальнейшего помещения
				fsetpos(bin, &probeg);            //возвращаемся на позицию замены
				fwrite(&enter, sizeof(int), 1, bin);//вписывае наше значение
				enter = next;                  //для будущей записи
				probeg += sizeof(int);        //переходим на след позицию
			}
			return;
		}
		else {
			probeg += sizeof(int);
			continue;
		}
	}
	//если мы дошли до сюда, значин наш элемент должен быть самым последним
	fwrite(&enter, sizeof(int), 1, bin);
}

int main() {
	FILE* bin;
      fopen_s(&bin, "binary.bin", "r+")

       if (!bin\txt) {
        puts("Cant open file");
        return 1;
    }
	     ent(bin);
		fclose(bin);
		return 0;
}