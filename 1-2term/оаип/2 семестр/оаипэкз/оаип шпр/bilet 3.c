//bilet 3 Отсортировать текстовый файл методом отбора

void enterFile(FILE* file, const char* filename, int* arr, int size) {
	file = fopen(filename, "w");
	if (file == NULL) {
		printf("Error\n");
		exit(-1);
	}
	for (int i = 0; i < size; i++) {
		fprintf(file, "%5d", arr[i]);
	}
	fclose(file);
}

int* Arr(int* size) {

	int* array = (int*)malloc(*size * sizeof(int));
	if (array == NULL) {
		exit(-2);
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

void fileSorting_selective_(FILE* file, const char* filename, int (*cmp)(int, int)) {
	file = fopen(filename, "r+");
	if (file == NULL){
		printf("Error opening file!\n");
		exit(-1);
	}
	int buffer_to_compare;
	int current_buffer;
	int a, b;
	for (fpos_t i = 0; i < filesize(file); i += 5) {
		fsetpos(file, &i);
		fpos_t _position = ftell(file);						
		for (fpos_t j = i; j < filesize(file); j += 5) {	
			fsetpos(file, &_position);						
			fscanf(file, "%5d", &buffer_to_compare);		
			fsetpos(file, &j);
			fscanf(file, "%5d", &current_buffer);
			if (cmp(buffer_to_compare, current_buffer)) {	
				_position = j;
			}
		}
		fsetpos(file, &i);
		fsetpos(file, &_position);
		fscanf(file, "%5d", &b);
		fsetpos(file, &i);
		fprintf(file, "%5d", b);
		fsetpos(file, &_position);
		fprintf(file, "%5d", a);
	}
	fclose(file);
}
int main() {
	FILE* file;
	int size = 0;
	int* arr = Arr(&size);
	enterFile(file, "f.txt", arr, size);
	fileSorting_selective_(file, "f.txt", cmp_gt);
	fileSorting_selective_(file, "f.txt", cmp_less);
	free(arr);
}
