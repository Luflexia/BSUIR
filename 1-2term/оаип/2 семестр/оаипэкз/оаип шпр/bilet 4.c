//bilet 4 Отсортировать бинарный файл методом отбора

int* Arr(int* size) {
	int* array = (int*)malloc(*size * sizeof(int));
		if (!arr) {
		exit(error);
	}
	return array;
}

int filesize(FILE* file) {
	fseek(file, 0, SEEK_END);	
	return ftell(file);			
}

int cmp_gt(int a, int b) {
	return a > b;
}

int cmp_less(int a, int b) {
	return a < b;
}

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

int getFileData(FILE* file, fpos_t index) {
	int data;
	fsetpos(file, &index);				
	fread(&data, sizeof(int), 1, file);	
	return data;
}

void swapValuesInFile(FILE* file, fpos_t index1, fpos_t index2) {
	int a, b;
	fsetpos(file, &index1);				
	fread(&a, sizeof(int), 1, file);
	fsetpos(file, &index2);
	fread(&b, sizeof(int), 1, file);
	fsetpos(file, &index1);			    
	fwrite(&b, sizeof(int), 1, file);
	fsetpos(file, &index2);
	fwrite(&a, sizeof(int), 1, file);
}

void binaryFIle_sort_selective(FILE* file, const char* filename, int (*cmp)(int, int)) {
	file = fopen(filename, "r+b");
	if (file == NULL) {
		printf("Error opening file!\n");
		exit(-101);
	}
	int buffer_to_compare;			
	int current_buffer;				
	int a, b;						
	for (fpos_t i = 0; i < filesize(file); i += sizeof(int)) {
		fsetpos(file, &i);									
		fpos_t _position = ftell(file);						
		for (fpos_t j = i; j < filesize(file); j += sizeof(int)) {	
			buffer_to_compare = getFileData(file, _position);	
			current_buffer = getFileData(file, j);
			if (cmp(buffer_to_compare, current_buffer)) {	
				_position = j;								
			}
		}
		a = getFileData(file, i);				
		b = getFileData(file, _position);		
		swapValuesInFile(file, i, _position);
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
