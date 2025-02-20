//bilet 12

void delfunc(FILE *txt, FILE *bin) {
	int binary_number, * future_numbers, num = 0;
    
	future_numbers = (int*)malloc(2 * sizeof(int));

	//просто возьму числа их текстового файла в массив, и потом удалю из них всё лишнее и пересоздам файл
	while (!feof(txt)) {
		fscanf_s(txt, "%d", &future_numbers[num]);
		if (feof(txt))
			break;
		num++;
		future_numbers = (int*)realloc(future_numbers, (num+1) * sizeof(int));
 	}
	num--;
	while (!feof(bin)) {
		fread(&binary_number, 1, sizeof(int), bin);
		if (feof(bin))
			break;
		for (int i = 0; i < num+1; i++) {
			if (future_numbers[i] == binary_number)
				future_numbers[i] = -1;
		}
	}
	fclose(txt);
	fopen_s(&txt, "text.txt", "w+");
	rewind(txt);
	for (int i = 0; i < num+1; i++) {
		if (future_numbers[i] == -1)
			continue;
		fprintf(txt, "%3d ", future_numbers[i]);
	}
	free(future_numbers);
	return;
}

int main() {
	FILE* bin, * txt;

    fopen_s(&bin\txt, my_bin.bin, "w+"); //открываем и меняем название
    if (!bin\txt)
    {
        puts("Cant open file");
        return 1;
    }

    delfunc(txt, bin);
    return 0;
}