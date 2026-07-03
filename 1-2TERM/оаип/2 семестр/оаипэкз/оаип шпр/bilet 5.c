//bilet 5 вставить число в отсортированный по возрастанию бинарный файл не нарушая последовательности

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

void insertValue(const char* filename, int _valueToInsert) {
	FILE* filePointer = fopen(filename, "rb+");
	if (!filePointer) {
		printf("Error openig file!\n");
		exit(-101);
	}
	
	int currentBuffer;							
	int insertionMarker = 0;					
	int tempValue = 0;							
	int filelenght = filesize(filePointer);		

	for (fpos_t i = 0; i < filelenght; i += sizeof(int)) {	
				
		fsetpos(filePointer, &i);							
			
		fread(&currentBuffer, sizeof(int), 1, filePointer);	
		
		if (currentBuffer >= _valueToInsert) {				
			
			insertionMarker = 1;							

			for (fpos_t j = i; j < filelenght + sizeof(int); j += sizeof(int)) {	
			
				fsetpos(filePointer, &j);							
				fread(&tempValue, sizeof(int), 1, filePointer);				
				fsetpos(filePointer, &j);
				fwrite(&_valueToInsert, sizeof(int), 1, filePointer);		
				_valueToInsert = tempValue;						
			}											
			break;										
		}
	}
	if (!insertionMarker) {			
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
