//bilet 11 Даны 2 бинарных файла, отсортированных по убыванию. Сделать один

void sortfunc(FILE* first, FILE* second, FILE* third) {

	fpos_t end1 = 0, end2 = 0;
	int temp;
    
	while (!feof(first)) 
		fread(&temp, sizeof(int), 1, first);
	fgetpos(first, &end1);
	while (!feof(second))  
		fread(&temp, sizeof(int), 1, second);
	fgetpos(second, &end2);

	fpos_t pos1 = end1 - sizeof(int), pos2 = end2 - sizeof(int); 
																					  // возможно у тебя вопрос: зачем делать -sizeof(int)?
																					 // ответ прост - мы стоим на eof поэтому мы двигуаем на одну позицию назад
	int select[2];											   	 
	bool skip1 = false, skip2 = false;			    
	rewind(first); rewind(second);							   
	fsetpos(first, &pos1); fsetpos(second, &pos2); 
	while (pos1 >= 0 || pos2 >= 0) {              
		fsetpos(first, &pos1); fsetpos(second, &pos2);
		if (pos1 >= 0)                          
			fread(&select[0], sizeof(int), 1, first);
		else
			skip1 = 1;                     
		if (pos2 >= 0)
			fread(&select[1], sizeof(int), 1, second);
		else
			skip2 = 1;
		if (skip1 == 1) { 
			fwrite(&select[1], sizeof(int), 1, third);
			pos2 -= sizeof(int);
			continue;
		}
		else (skip2 == 1) {    
			fwrite(&select[0], sizeof(int), 1, third);
			pos1 -= sizeof(int);
			continue;
		}
		if (select[0] < select[1]) {     
			fwrite(&select[0], sizeof(int), 1, third);
			pos1 -= sizeof(int);
		}
		else {
			fwrite(&select[1], sizeof(int), 1, third);
			pos2 -= sizeof(int);
		}
	}
}

int main() {
	FILE* first, * second, * third;

    fopen_s(&first\second\third, first\second\third.bin, "w+"); 
    if (!bin)
    {
        puts("error");
        return 1;
    }
       sortfunc(first, second, third);
       return 0;
}