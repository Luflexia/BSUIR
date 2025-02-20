//bilet 11

void sortfunc(FILE* first, FILE* second, FILE* third) {

	//в файле first and second мы будем идти с конца и до нуля (значения fpos_t)
	fpos_t end_first = 0, end_second = 0;
	int temp;
    
	while (!feof(first)) //находим конец первого файла
		fread(&temp, sizeof(int), 1, first);
	fgetpos(first, &end_first);
	while (!feof(second))   //находим конец второгой файла
		fread(&temp, sizeof(int), 1, second);
	fgetpos(second, &end_second);
	//ну а теперь начинаем как-то записывать в 3-ий файл
	fpos_t pos_first = end_first - sizeof(int), pos_second = end_second - sizeof(int); // берем наши последние значения
																					  // возможно у тебя вопрос: зачем делать -sizeof(int)?
																					 // ответ прост - мы стоим на eof поэтому мы двигуаем на одну позицию назад
	int select[2];											   	 //переменная для хранения чисел
	bool skip_first = false, skip_second = false;			    // флаг окончания файла
	rewind(first); rewind(second);							   //обновляем файлы
	fsetpos(first, &pos_first); fsetpos(second, &pos_second); //становимся на позиции
	while (pos_first >= 0 || pos_second >= 0) {              //пока есть еще цифры для считывания - мы работаем
		fsetpos(first, &pos_first); fsetpos(second, &pos_second);
		if (pos_first >= 0)                          //пока остались цифры - мы их считываем
			fread(&select[0], sizeof(int), 1, first);
		else
			skip_first = 1;                      // иначе ставим флаг пропуска
		if (pos_second >= 0)
			fread(&select[1], sizeof(int), 1, second);
		else
			skip_second = 1;
		if (skip_first == 1) { //если флаг пропуска стоит
			fwrite(&select[1], sizeof(int), 1, third);
			pos_second -= sizeof(int);
			continue;
		}
		else (skip_second == 1) {    // если флаг пропуска стоит
			fwrite(&select[0], sizeof(int), 1, third);
			pos_first -= sizeof(int);
			continue;
		}
		if (select[0] < select[1]) {     //просто сравниваем числа
			fwrite(&select[0], sizeof(int), 1, third);
			pos_first -= sizeof(int);
		}
		else {
			fwrite(&select[1], sizeof(int), 1, third);
			pos_second -= sizeof(int);
		}
	}
}

int main() {
	FILE* first, * second, * third;

    fopen_s(&first\second\third, first\second\third.bin, "w+"); //открываем и меняем название
    if (!bin)
    {
        puts("Cant open file");
        return 1;
    }
       sortfunc(first, second, third);
       return 0;
}