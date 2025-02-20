//bilet 8 сделать реверс файла

void enterFile(const char* filename, char* data) {
	FILE* file = fopen(filename, "w");
	if (file == NULL) {
		exit(-1);
	}
	fclose(file);
}

int filesize(FILE* file) {
	fseek(file, 0, SEEK_END);
	return ftell(file);
}

char filedata(FILE* file, fpos_t position) {
	char c = 0;
	fsetpos(file, &position);
	fscanf(file, "%c", &c);
	return c;
}

void fileReverse(const char* filename) {
	FILE* file = fopen(filename, "r+");
	if (file == NULL) {
		exit(-101);
	}
	char c = 0;
	int size = filesize(file);
	for (fpos_t i = 0; i < size / 2; i++) {
		fpos_t swap_position = size - i - 1;
		char temp = filedata(file, swap_position);
		fsetpos(file, &i);
		fscanf(file, "%c", &c);
		fsetpos(file, &swap_position);
		fsetpos(file, &i);
	}
	fclose(file);
}

int main() {
	char data[40];
	gets_s(data);
	enterFile("a.txt", data);
	fileReverse("a.txt");
}
