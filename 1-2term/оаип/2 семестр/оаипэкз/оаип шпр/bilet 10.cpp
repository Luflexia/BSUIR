// bilet 10 Отсортировать бинарный файл целых длинных чисел методом отбора

void sort(FILE* bin) {

	int swap[2]; 
	fpos_t pos, pos_end;
	rewind(bin);
	
	while (1){ 
		fread(&swap[0], sizeof(int), 1, bin);
		if (feof(bin)) break;
	}
	fgetpos(bin, &pos_end); 
	rewind(bin); 
	pos = pos_end - sizeof(int)*2; 
	fsetpos(bin, &pos); 

	while (pos >= 0){
		fread(&swap[0], sizeof(int), 1, bin); 
		fread(&swap[1], sizeof(int), 1, bin); 
		while (swap[1] < swap[0] && pos < pos_end) { 
			fsetpos(bin, &pos);                       
			fwrite(&swap[1], sizeof(int), 1, bin);   
			fpos_t temp; fgetpos(bin, &temp);       
			fwrite(&swap[0], sizeof(int), 1, bin);  
			fsetpos(bin, &temp);                   
			fread(&swap[0], sizeof(int), 1, bin); 
			fread(&swap[1], sizeof(int), 1, bin);
			if (feof(bin))                     
				break;                         
			pos += sizeof(int);               
		}
		pos -= sizeof(int); //спускаемся к началу
		fsetpos(bin, &pos); //устанавливаем позицию
	}
}

int main() {
	FILE* bin; 
    
    fopen_s(&bin, my_bin.bin, "w+"); 
    if (!bin)
    {
        puts("error");
        return 1;
    }
		sort(bin);
    return 0;
}