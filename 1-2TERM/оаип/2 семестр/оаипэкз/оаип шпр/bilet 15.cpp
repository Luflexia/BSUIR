//bilet 15 Дан текстовый файл, содержащий двузначные числа

void swap(FILE* txt) {
	fpos_t left = 0, right; 

	fseek(txt, 0, 2);   
	fgetpos(txt, &right);
	right -= 3; 

	if (left == right && left == 0) {
		printf("file is empty!\n");
		return;
	}

	rewind(txt);
	int left_num, right_num;
	while (left != right) {
		fsetpos(txt, &left);
		fscanf_s(txt, "%d", &left_num);
		fsetpos(txt, &right);
		fscanf_s(txt, "%d", &right_num);
		fsetpos(txt, &right);
		fprintf(txt,"%2d", left_num);
		fsetpos(txt, &left);
		fprintf(txt, "%2d", right_num);
		left += 3;
		right -= 3;
		if (left > right)
			break;
	}
	return;
}

int main() {
	FILE* txt;

	 fopen_s(&txt, "binary.bin", "w+")
       if (!txt) {
        puts("error");
        return 1;
    }
		swap(txt);
		fclose(txt);
		return 0;
}