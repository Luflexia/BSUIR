// Bilet 9 В бинарном файле поменять местами 1 и 2, 3 и 4

void swapfunc(FILE* bin) {
	
	int swap1[1], swap2[1];
	fpos_t pos1 = 0;
	rewind(bin);
	do {
		fgetpos(bin, &pos1);   
		fsetpos(bin, &pos1); 
 
		fread(&swap1[0], sizeof(int), 1, bin);
		if (feof(bin))  break;
		fread(&swap2[0], sizeof(int), 1, bin);
		if (feof(bin))  break;

		fsetpos(bin, &pos1); 
		fwrite(&swap2[0], sizeof(int), 1, bin);
		fwrite(&swap1[0], sizeof(int), 1, bin);
	} while (1);
}

int main() {
	FILE* bin;

	    fopen_s(&bin, my_bin.bin, "w+");
    if (!bin)
    {
        puts("Cant open file");
        return 1;
    }

    swapfunc(bin);
    return 0;
}