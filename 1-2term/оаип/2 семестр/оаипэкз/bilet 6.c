/*В командной строке задаётся имя файла и диапазон чисел. Удалить из бинарного файла числа из данного диапазона.*/

int* Arr(int number_of_values) {
	int* arr = (int*)malloc(number_of_values * sizeof(int));
	if (!arr) {
		exit(error);
	}
	return arr;
}

FILE* create_random_binary_file(const char* filename, int number_of_values, int maximum_possible_value) {
	FILE* binary_file_pointer = fopen(filename, "wb");
	if (!binary_file_pointer) {
		exit(ERROR_OPENING_FILE);
	}
	int* data_to_write = Arr(number_of_values, maximum_possible_value);
	for (size_t i = 0; i < number_of_values; i++) {
		fwrite(&data_to_write[i], sizeof(int), 1, binary_file_pointer);
	}
	fclose(binary_file_pointer);
}

// функция удаления значений из заданного диапазона
FILE* delete(const char* filename, int low_value, int max_value) {
	FILE* binary_file_pointer = fopen(filename, "rb");
	
	if (!binary_file_pointer) {
		exit(ERROR_OPENING_FILE);
	}
	
	if (low_value > max_value) {
		int temp = low_value;
		low_value = max_value;
		max_value = temp;
	}
	
	int* file_data = (int*)malloc(sizeof(int));
	if (!file_data) {
		exit(ALLOCATION_ERROR);
	}
	int current_buffer = 0;
	int i = 0;
	int array_size = 0;
	// суть проста: просто переписываем в массив значения, которые не входят в этот диапазон, а после записываем их обратно в файл :)
	while (!feof(binary_file_pointer)) {
		fread(&current_buffer, sizeof(int), 1, binary_file_pointer);
		if (current_buffer >= low_value && current_buffer <= max_value) {
			continue;
		} else {
			file_data = (int*)realloc(file_data, (i + 1) * sizeof(int));
			array_size++;
			if (!file_data) {
				exit(ALLOCATION_ERROR);
			}
			file_data[i++] = current_buffer;
		}
	}
	binary_file_pointer = fopen(filename, "wb");
	if (!binary_file_pointer) {
		exit(ERROR_OPENING_FILE);
	}
	for (int i = 0; i < array_size; i++) {
		fwrite(&file_data[i], sizeof(int), 1, binary_file_pointer);
	}
	fclose(binary_file_pointer);
	return binary_file_pointer;
}

int main(int argc, char* argv[]) {
	const char* filename = argv[0];
	int low_value = atoi(argv[1]);
	int max_value = atoi(argv[2]);
	FILE* binary_file_pointer = create_random_binary_file(filename, 10, 20);
	binary_file_pointer = delete(filename, low_value, max_value);
}
