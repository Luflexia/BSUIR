//bilet 13

void do_the_task(FILE* txt, FILE* bin) {
	
	int* massiv,temp,counter = 0;
	massiv = (int*)malloc(sizeof(int));
	rewind(bin); rewind(txt);

	//забираем значения из бинарки
	while (1) {
		fread(&temp, sizeof(int), 1, bin);
		if (feof(bin))
			break;
		massiv[counter] = temp;
		counter++;
		massiv = (int*)realloc(massiv,(counter + 1) * sizeof(int));
	}

	//ищем лишние значения
	while (1) {
		fscanf_s(txt, "%d", &temp);
		if (feof(txt))
			break;
		for (int i = 0; i < counter; i++)
			if (temp == massiv[i]) {           //если находим
				for (int j = i; j < counter - 1; j++)  
					massiv[j] = massiv[j + 1];			//все смещаем
				counter--;								//уменьшаем длину
				massiv = (int*)realloc(massiv, (counter + 1) * sizeof(int));
			}
	}

	fclose(bin); fopen_s(&bin, "binary.bin", "w+"); //ресетаем бин
	//записываем все
	for (int i = 0; i < counter; i++)
		fwrite((massiv + i), sizeof(int), 1, bin);

	free(massiv);
}

int main() {
	FILE* bin, * txt;
    fopen_s(&bin\txt, bin\txt.bin, "w+");

    if (!bin || !txt){
        puts("Cant open file");
        return 1;
    }
		do_the_task(txt, bin);
		fclose(bin); 
            fclose(txt); 
	 return 0;
}