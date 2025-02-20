//Отсортировать бинарный файл методом отбора

// функция которая инициализирует массив рандомными числами
int* Arr(int* size) {
	int* array = (int*)malloc(*size * sizeof(int));
		if (!arr) {
		exit(error);
	}
	return array;
}
// определение размера файла
int filesize(FILE* file) {
	fseek(file, 0, SEEK_END);	// смещение в конце
	return ftell(file);			// возвращаем позицию конца файла - это и есть размер 
}

// две функции сравнения
int cmp_gt(int a, int b) {
	return a > b;
}

int cmp_less(int a, int b) {
	return a < b;
}

// заполнение файла данными из массива
void enterFile(FILE* file, const char* filename, int* data, int size) {
	file = fopen(filename, "wb");
	if (file == NULL) {
		printf("Error opening file!\n");
		exit(-101);
	}
	for (int i = 0; i < size; i++)
		fwrite(&data[i], sizeof(int), 1, file);
	fclose(file);
}

// функция которая возвращает данные из файла по индексу
int getFileData(FILE* file, fpos_t index) {
	int data;
	fsetpos(file, &index);				// устанавливаем указатель по нужному индексу
	fread(&data, sizeof(int), 1, file);	// считываем данные
	return data;						// возвращаем их
}

// свап двух чисел в файле
void swapValuesInFile(FILE* file, fpos_t index1, fpos_t index2) {
	int a, b;
	fsetpos(file, &index1);				// считываем два числа по их индексам
	fread(&a, sizeof(int), 1, file);
	fsetpos(file, &index2);
	fread(&b, sizeof(int), 1, file);
	fsetpos(file, &index1);			    // потом переписываем значения наоборот
	fwrite(&b, sizeof(int), 1, file);
	fsetpos(file, &index2);
	fwrite(&a, sizeof(int), 1, file);
}

// сортировка выбором
void binaryFIle_sort_selective(FILE* file, const char* filename, int (*cmp)(int, int)) {
	file = fopen(filename, "r+b");
	if (file == NULL) {
		printf("Error opening file!\n");
		exit(-101);
	}
	int buffer_to_compare;			// буффер для сравнения
	int current_buffer;				// текущий буффер
	int a, b;						// переменные для свапа
	for (fpos_t i = 0; i < filesize(file); i += sizeof(int)) {
		fsetpos(file, &i);									// устанавливаем позицию на i в каждой итерации цикла
		fpos_t _position = ftell(file);						// позиция с которой начинается поиск позиции минимального/максимального
		for (fpos_t j = i; j < filesize(file); j += sizeof(int)) {	
			buffer_to_compare = getFileData(file, _position);	// считываем два значения
			current_buffer = getFileData(file, j);
			if (cmp(buffer_to_compare, current_buffer)) {	// сравниваем их
				_position = j;								// если условие истинно, запоминаем позцию
			}
		}
		a = getFileData(file, i);				// считываем число по i
		b = getFileData(file, _position);		// и по позиции минимального/максимального 
		swapValuesInFile(file, i, _position);	// меняем их местами
	}
	fclose(file);
}

int main() {
	FILE* file = NULL;
	int size = 0;
	int* array = Arr(&size);
	enterFile(file, "f.bin", array, size);
	binaryFIle_sort_selective(file, "f.bin", cmp_less);
	binaryFIle_sort_selective(file, "f.bin", cmp_gt);
	free(array);
}
