// bilet 10

void sort(FILE* bin) {
	 // бинарка отказывается воспринимать отрицательный сдвиг (дохера захотел)
	// поэтому мы кое как выкрутися
	int swap[2]; //для переменных
	fpos_t pos, pos_end;
	rewind(bin);
	//подготовка к сортировки
	while (1){ //ищем конец файла
		fread(&swap[0], sizeof(int), 1, bin);
		if (feof(bin)) break;
	}
	fgetpos(bin, &pos_end); //еще не понял? будем идти с конца
	rewind(bin); //возвращаемся на место
	pos = pos_end - sizeof(int)*2; // допустим у нас комбинация 4 7 6, для моего способа нам нужно стать на 7
	fsetpos(bin, &pos); //установили положение

	//сортировка
	while (pos >= 0){
		fread(&swap[0], sizeof(int), 1, bin); //записываем 7
		fread(&swap[1], sizeof(int), 1, bin); //записываем 6
		while (swap[1] < swap[0] && pos < pos_end) { //если мы выловили, что наш порядок нарушен
			fsetpos(bin, &pos);                        //устанавлиаем положение семерки
			fwrite(&swap[1], sizeof(int), 1, bin);    //ставим на ее место 6
			fpos_t temp; fgetpos(bin, &temp);        //запоминаем временную позицию второй щестерки ( 4 6 (6) )
			fwrite(&swap[0], sizeof(int), 1, bin);  // перепесиываем ее семеркой (4 6 7)
			fsetpos(bin, &temp);                   // возвращаемся на положение 6 чтобы проверить нарушена ли комбинация перед нами
			fread(&swap[0], sizeof(int), 1, bin); // записываем значения
			fread(&swap[1], sizeof(int), 1, bin);// все еще записываем
			if (feof(bin))                      // если мы в конце файла, Значит всё уже отсортировано
				break;                         //выход из цикла
			pos += sizeof(int);               //ставим на позицию вперед
		}
		pos -= sizeof(int); //спускаемся к началу
		fsetpos(bin, &pos); //устанавливаем позицию
	}
}

int main() {
	FILE* bin; // мой указатель на файлик
    
    fopen_s(&bin, my_bin.bin, "w+"); //открываем и меняем название
    if (!bin)
    {
        puts("Cant open file");
        return 1;
    }
		sort(bin);
    return 0;
}