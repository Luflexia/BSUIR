// вставить число в отсортированный по возрастанию бинарный файл не нарушая последовательности

int* Arr(size_t _сreatedArrayLenght) {
	
	int* arr = (int*)malloc(_сreatedArrayLenght * sizeof(int));
	if (!arr) {
		exit(error);
	}
	return arr;
}

int compare_less(int _firstValue, int _secondValue){
	return _firstValue < _secondValue; 
}

int compare_greater(int _firstValue, int _secondValue) {
	return _firstValue > _secondValue;
}

void sortArray(int* array, int _valuesNumber, int (*_functionToCompare)(int, int)) {
	for (size_t i = 0; i < _valuesNumber - 1; i++){
		for (size_t j = 0; j < _valuesNumber - i - 1; j++){
			if (_functionToCompare(array[j], array[j + 1])) {
				int temp = array[j];
				array[j] = array[j + 1];
				array[j + 1] = temp;
			}
		}
	}
}

void enterFile(const char* filename, int _valuesNumber) {	
	FILE* filePointer = fopen(filename, "wb");
	
	if (filePointer == NULL) {
		printf("Error opening file!\n");
		exit(-101);
	}
	
	int* arr = Arr(_valuesNumber);
	sortArray(arr, _valuesNumber, compare_greater);
	
	for (size_t i = 0; i < _valuesNumber; i++){
		fwrite(&arr[i], sizeof(int), 1, filePointer);
	}

	free(arr);
	fclose(filePointer);
}

int filesize(FILE* filePointer) {
	fseek(filePointer, 0, SEEK_END);
	return ftell(filePointer);
}

int getFileData(FILE* file, fpos_t _positionToRead) {
	int buffer = 0;
	fread(&buffer, sizeof(int), 1, file);
	return buffer;
}

// функция вставки нового значение в файл
void insertValue(const char* filename, int _valueToInsert) {
	FILE* filePointer = fopen(filename, "rb+");
	if (!filePointer) {
		printf("Error openig file!\n");
		exit(-101);
	}
	
	int currentBuffer;							// буфер текущего числа в файле
	int insertionMarker = 0;					// маркер того, вставлено ли число в файл
	int tempValue = 0;							// временная переменная для вставки в файл
	int filelenght = filesize(filePointer);		// размер файла

	for (fpos_t i = 0; i < filelenght; i += sizeof(int)) {	// внешний цикл от нуля до конца
				
		fsetpos(filePointer, &i);							// становимся на текущую позицию
			
		fread(&currentBuffer, sizeof(int), 1, filePointer);	// считываем текущее значение
		
		if (currentBuffer >= _valueToInsert) {				// если найдено место для вставки
			
			insertionMarker = 1;							// маркер вставки становится единицкй

			for (fpos_t j = i; j < filelenght + sizeof(int); j += sizeof(int)) {	// смещение всех чисел вправо
			
				fsetpos(filePointer, &j);							// устанавливается позиция на j
				fread(&tempValue, sizeof(int), 1, filePointer);				// считывается текущее значение
				fsetpos(filePointer, &j);
				fwrite(&_valueToInsert, sizeof(int), 1, filePointer);		// записвается значение, которое нужно вставить
				_valueToInsert = tempValue;						// присваиваем значению для вставки текущее, 
			}											// чтобы продолжить циклический перенос =
			break;										// после вставки выходим из цикла
		}
	}
	if (!insertionMarker) {			// если вставки не произошло, то значение записывается в конец =
		fwrite(&_valueToInsert, sizeof(int), 1, filePointer);
	}
	fclose(filePointer);
}

int main() {
	int fileLenght = 10;
	enterFile("a.txt", 10);
	insertValue("a.txt", 7);
	insertValue("a.txt", 3);
	insertValue("a.txt", 6);
}
