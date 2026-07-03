//bilet 13 Из бинарного файла удалить все числа, встречающиеся

void delete(FILE* txt, FILE* bin) {
	
	int* massiv,temp,counter = 0;
	massiv = (int*)malloc(sizeof(int));
      if (!massiv) {
		   exit(error);
	      }
	rewind(bin); rewind(txt);

	while (1) {
		fread(&temp, sizeof(int), 1, bin);
		if (feof(bin))
			break;
		massiv[counter] = temp;
		counter++;
		massiv = (int*)realloc(massiv,(counter + 1) * sizeof(int));
	}

	while (1) {
		fscanf_s(txt, "%d", &temp);
		if (feof(txt))
			break;
		for (int i = 0; i < counter; i++)
			if (temp == massiv[i]) {         
				for (int j = i; j < counter - 1; j++)  
					massiv[j] = massiv[j + 1];		
				counter--;
				massiv = (int*)realloc(massiv, (counter + 1) * sizeof(int));
			}
	}

	fclose(bin); fopen_s(&bin, "binary.bin", "w+"); 

	for (int i = 0; i < counter; i++)
		fwrite((massiv + i), sizeof(int), 1, bin);

	free(massiv);
}

int main() {
	FILE* bin, * txt;
    fopen_s(&bin\txt, bin\txt.bin, "w+");

    if (!bin || !txt){
        puts("error");
        return 1;
    }
		delete(txt, bin);
		fclose(bin); 
            fclose(txt); 
	 return 0;
}