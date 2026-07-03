//bilet 14 Есть бинарный файл, упорядоченный по возрастанию

void ent(FILE* bin) {

	printf("entering number");
	int enter, checker;
	checker = scanf_s("%d", &enter);
	while (!checker)
		checker = scanf_s("%d", &enter);

	fpos_t end;
	rewind(bin);
	fseek(bin, 0, 2);
	fgetpos(bin, &end);
	end -= sizeof(int);

	if (end <= 0) {
		fwrite(&enter, sizeof(int), 1, bin);
		return;
	}
	rewind(bin);
	
	fpos_t probeg; int temp;
	fgetpos(bin, &probeg);
	while (probeg <= end) {
		fread(&temp, sizeof(int), 1, bin);
		if (temp >= enter) { 
			while (probeg <= end+sizeof(int)) {
				int next;                           
				fsetpos(bin, &probeg);             
				fread(&next, sizeof(int), 1, bin); 
				fsetpos(bin, &probeg);            
				fwrite(&enter, sizeof(int), 1, bin);
				enter = next;                  
				probeg += sizeof(int);        
			}
			return;
		}
		else {
			probeg += sizeof(int);
			continue;
		}
	}
	fwrite(&enter, sizeof(int), 1, bin);
}

int main() {
	FILE* bin;
      fopen_s(&bin, "binary.bin", "r+")

       if (!bin\txt) {
        puts("error");
        return 1;
    }
	     ent(bin);
		fclose(bin);
		return 0;
}