//bilet 6 В командной строке задаётся имя файла и диапазон чисел. Удалить из бинарного файла числа из данного диапазона

int* Arr(int numval) {
	int* arr = (int*)malloc(numval * sizeof(int));
	if (!arr) {
		exit(error);
	}
	return arr;
}

FILE* createbin(const char* filename, int numval, int maxval) {
	FILE* bfp = fopen(filename, "wb+");
	if (!bfp) {
		exit(error);
	}
	fclose(bfp);
}

FILE* delete(const char* filename, int min, int max) {
	FILE* bfp = fopen(filename, "rb");
	
	if (!bfp) {
		exit(error);
	}
	
	if (min > max) {
		int temp = min;
		min = max;
		max = temp;
	}
	
	int* file_data = (int*)malloc(sizeof(int));
	if (!file_data) {
		exit(error);
	}
	int buf = 0;
	int i = 0;
	int array_size = 0;

	while (!feof(bfp)) {
		fread(&buf, sizeof(int), 1, bfp);
		if (buf >= min && buf <= max) {
			continue;
		} else {
			file_data = (int*)realloc(file_data, (i + 1) * sizeof(int));
			array_size++;
			if (!file_data) {
				exit(error);
			}
			file_data[i++] = buf;
		}
	}
	bfp = fopen(filename, "wb");
	if (!bfp) {
		exit(error);
	}
	for (int i = 0; i < array_size; i++) {
		fwrite(&file_data[i], sizeof(int), 1, bfp);
	}
	fclose(bfp);
	return bfp;
}

int main(int argc, char* argv[]) {
	const char* filename = argv[0];
	int min = atoi(argv[1]);
	int max = atoi(argv[2]);
	FILE* bfp = createbin(filename, 10, 20);
	bfp = delete(filename, min, max);
}
