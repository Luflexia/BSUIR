//Билет 23 добавления русского символа, не нарушая упорядоченности.

int main() {
	system("chcp 1251>null"); 
	char* string;
	int flag =0;
	string = (char*)calloc(100, sizeof(char));
      if (!string) {
		   exit(error);
	}

	gets_s(string, 100);
	FILE* f;
	if (!(f = fopen("file.txt", "w+"))) {
		printf("Error\n");
		return 0;
	}
	rewind(f);

	char c,c1, c2;

	printf("\enter rus symb\n");
	char symbol;

      while (!(scanf("%c", &symbol) && n<0)) {
		printf("error\n");
		rewind(stdin);
	}

	rewind(stdin);
	rewind(f);
	while (1) {
		while (1) {
			
			fscanf(f, "%c", &c);
			
			if (feof(f)) {
				break;
			}
			if (symbol < c && c != ' ') {
				fseek(f, -1, 1);
				flag = 1;
				break;
			}
		}
		if (flag == 0){
			fprintf(f, "%c", symbol);
			break;
		} 
		else {
			c1 = c;
			fprintf(f, "%c", symbol);
			while (1) {
				fscanf(f, "%c", &c2);
				if (feof(f)) {
					
					fprintf(f, "%c", c1);
					break;
				}
				fseek(f, -1, 1);
				fprintf(f, "%c", c1);
				
				
				c1 = c2;
			}
			break;


		}
	}
	rewind(f);
	fclose(f);
	return 0;
}